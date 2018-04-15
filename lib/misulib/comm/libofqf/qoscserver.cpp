/*
 * Copyright ( C ) 2007 Arnold Krille <arnold@arnoldarts.de>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or ( at your option ) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 */

#include "qoscserver.h"

#include <QtCore/QRegExp>
#include <QtNetwork/QUdpSocket>

QOscServer::QOscServer( quint16 port, QObject* p )
	: QOscBase( p )
{
        decodeState=0;
        socket()->bind( QHostAddress::Any, port , QUdpSocket::ShareAddress);
        connect( socket(), SIGNAL( readyRead() ), this, SLOT( readyRead() ) );
}
QOscServer::QOscServer( QHostAddress address, quint16 port, QObject* p )
	: QOscBase( p )
{
        decodeState=0;
        socket()->bind( address, port, QUdpSocket::ShareAddress );
        connect( socket(), SIGNAL( readyRead() ), this, SLOT( readyRead() ) );
}

QOscServer::~QOscServer() {
        //qDebug() << "QOscServer::~QOscServer()";
}

void QOscServer::registerPathObject( PathObject* p ) {
//        qDebug() << "QOscServer::registerPathObject(" << p->_path << ")";
        paths.push_back( p );
}
void QOscServer::unregisterPathObject( PathObject* p ) {
	paths.removeAll( p );
}

#define BUFFERSIZE 4096

void QOscServer::readyRead() {
    QHostAddress * src_adr = new QHostAddress;
    quint16 * src_port = new quint16;

    while ( socket()->hasPendingDatagrams() ) {
            QByteArray data( BUFFERSIZE, char( 0 ) );
            qint64 size = socket()->readDatagram( data.data(), BUFFERSIZE, src_adr, src_port );
            //qDebug() << " read" << size << "(" << data.size() << ") bytes:" << data;

            int i=0;
            decodeState=0;  // in case of UDP we expect one osc packet per call
            QList<QVariant> list;
            QString parseString;
            while(i<size) {
                if(decodeState==0 && data[i] == '#' && data[i+1] == 'b' && data[i+2] == 'u') {
                    inBundle=true;
                    i+=19;
                    decodeState=1;  // looking for path begin
                } else if(decodeState==0 || decodeState==1) {
                    if(data[i] == '/' ) {
                        path="/";
                        decodeState=2;  // parsing path
                    }
                } else if(decodeState==2) {
                    if(data[i]!=char(0)) {
                        path += data[ i ];
                    } else {
                        decodeState=3;  // looking for osc type tag string begin
                    }
                } else if(decodeState==3) {
                    if(data[i] == ',') {
                        args="";
                        nargs=0;
                        carg=0;
                        decodeState=4; // parsing osc type tag string
                    }
                } else if(decodeState==4) {
                    if(data[i]!=char(0)) {
                        args += data[ i ];
                        nargs++;
                    } else {
                        decodeState=5;  // parsing args
                        int align=(i+1)%4;
                        if(align>0) {
                            i+=4-align;     // make sure the 4 bit alignment fits
                        }
                    }
                } else if(decodeState==5) {
                    if(args.at(carg)=='i') {
                        QByteArray tmp=data.mid(i,4);
                        list.append(toInt32(tmp));
                        i+=3;
                        carg++;
                    } else if(args.at(carg)=='f') {
                        QByteArray tmp=data.mid(i,4);
                        list.append(toFloat(tmp));
                        i+=3;
                        carg++;
                    } else if(args.at(carg)=='s') {
                        parseString=data[i];
                        decodeState=6; // parsing string argument
                    }
                } else if(decodeState==6) {
                    if(data[i]!=char(0)) {
                        parseString+=data[i];
                    } else {
                        list.append(parseString);
                        carg++;
                        decodeState=5;
                        int align=(i+1)%4;
                        if(align>0) {
                            i+=4-align;     // make sure the 4 bit alignment fits
                        }
                    }
                }
                if(decodeState>4 && carg==nargs) {
                    foreach( PathObject* obj, paths ) {
                        obj->signalData( path, list, src_adr, *src_port );
                    }

                    emit oscData(path,list);

                    decodeState=0;
                    list.clear();
                }
                i++;
            }
    }
}


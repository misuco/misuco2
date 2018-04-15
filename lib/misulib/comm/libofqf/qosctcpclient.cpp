/*
 * Copyright ( C ) 2007 Arnold Krille <arnold@arnoldarts.de>
 * TCP Part: Copyright ( C ) 2012 Claudio Zopfi <z1@x21.ch>
 *
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

#include "qosctcpclient.h"
#include <QtCore/QByteArray>
#include <QtNetwork/QTcpSocket>
#include <QtCore/QRegExp>

QOscTcpClient::QOscTcpClient(QObject *parent) :
    QOscBase(parent,true)
{
}

QOscTcpClient::QOscTcpClient( const QHostAddress& address, quint16 port, QObject* p )
        : QOscBase( p , true)
        , _address( address )
        , _port( port )
        , _tcp_socket (0)
{
        qDebug() << "QOscClient::QOscClient(" << address << "," << port << "," << p << ")";
        setupTcpSocket();
        QObject::connect( _tcp_socket, SIGNAL( readyRead() ), this, SLOT( readyRead() ) );
}

QOscTcpClient::QOscTcpClient( const QHostAddress& address, quint16 source_port, quint16 dst_port, QObject* p )
        : QOscBase( p , true)
        , _address( address )
        , _source_port( source_port )
        , _port( dst_port )
        , _tcp_socket (0)
{
    /*
     *  Bind to set source port
     */
    setupTcpSocket();
    QObject::connect( _tcp_socket, SIGNAL( readyRead() ), this, SLOT( readyRead() ) );
}


QOscTcpClient::~QOscTcpClient() {
//        qDebug() << "QOscClient::~QOscClient()";
    _tcp_socket->disconnectFromHost();
}

void QOscTcpClient::setAddress( char * adr, quint16 port ) {
    if(_tcp_socket->state()==QAbstractSocket::ConnectedState)
        _tcp_socket->disconnectFromHost();
    _address = QHostAddress(adr);
    _port = port;
    setupTcpSocket();
}

void QOscTcpClient::setupTcpSocket() {
    if ( !_tcp_socket ) {
        _tcp_socket = new QTcpSocket( this );
        _tcp_socket->setSocketOption(QAbstractSocket::LowDelayOption,1);
        QObject::connect(_tcp_socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(tcpStateChanged()));
    }

    if(_tcp_socket->state()!=QAbstractSocket::ConnectedState &&
       _tcp_socket->state()!=QAbstractSocket::HostLookupState  &&
       _tcp_socket->state()!=QAbstractSocket::ConnectingState   )
        _tcp_socket->connectToHost(_address,_port);
}

void QOscTcpClient::sendData( QString path, QVariant data ) {
    //qDebug() << "QOscClient::sendData(" << path << "," << data << ")";

    quint64 timeTag=getTimeTag();
    QByteArray msg = oscMessage( path, data );
    qint32 len = msg.length();

//    len preamble: supercollider
    QByteArray out(QOscBase::fromInt32(len+20)+QOscBase::fromString("#bundle")+QOscBase::fromInt64(timeTag)+QOscBase::fromInt32(len)+msg);

//      no len: pure data
//    QByteArray out(QOscBase::fromString("#bundle")+QOscBase::fromInt64(timeTag)+QOscBase::fromInt32(len)+msg);

    if(!(_tcp_socket->state()==QAbstractSocket::ConnectedState)) {
        setupTcpSocket();
//        qDebug() << "sending while not connected, droped one packet...";
    } else {
        _tcp_socket->write( out );
        _tcp_socket->flush();
    }

}

void QOscTcpClient::tcpStateChanged()
{
    switch(_tcp_socket->state()) {
        case QAbstractSocket::BoundState:
            //qDebug() << " bound ";
            break;
        case QAbstractSocket::ListeningState:
            //qDebug() << " listening ";
            break;
        case QAbstractSocket::UnconnectedState:
            //qDebug() << " unconnected ";
            break;
        case QAbstractSocket::HostLookupState:
            //qDebug() << " host lookup ";
            break;
        case QAbstractSocket::ConnectingState:
            qDebug() << " connecting " << _address << " " << _port;
            break;
        case QAbstractSocket::ConnectedState:
            qDebug() << " connected " << _address << " " << _port;
            _tcp_socket->setSocketOption(QAbstractSocket::LowDelayOption,1);
            emit connected();
            break;
        case QAbstractSocket::ClosingState:
            //qDebug() << " closing ";
            break;
    }
}

void QOscTcpClient::registerPathObject( PathObject* p ) {
//        qDebug() << "QOscServer::registerPathObject(" << p->_path << ")";
        paths.push_back( p );
}
void QOscTcpClient::unregisterPathObject( PathObject* p ) {
        paths.removeAll( p );
}

#define BUFFERSIZE 255

void QOscTcpClient::readyRead() {
//        qDebug() << "QOscTcpClient::readyRead()";
        while ( _tcp_socket->bytesAvailable()>0 ) {
                QByteArray data( BUFFERSIZE, char( 0 ) );
                int size = _tcp_socket->read( data.data(), BUFFERSIZE );
//                qDebug() << " read " << size << "(" << data.size() << ") bytes:" << data;

                QString path;
                QString args;
                QVariant arguments;

                int i=0;

                while(i<size) {
    //                if ( data[ i ] == '/' ) {

                            while (  i<size && data[ i ] != '/' ) ++i;

                            for ( ; i<size && data[ i ] != char( 0 ); ++i )
                                    path += data[ i ];

    //                        qDebug() << path;

                            while ( i<size && data[ i ] != ',' ) ++i;
                            ++i;
                            while ( i<size && data[ i ] != char( 0 ) )
                                    args += data[ i++ ];

                            if ( ! args.isEmpty() ) {
                                    QList<QVariant> list;

                                    foreach( QChar type, args ) {
                                            while ( i<size && i%4 != 0 ) ++i;
                                            //qDebug() << i << "\ttrying to convert to" << type;

                                            QByteArray tmp = data.right( data.size()-i );
                                            QVariant value;
                                            if ( type == 's' ) {
                                                    QString s = toString( tmp );
                                                    value = s;
                                                    i += s.size();
                                            }
                                            if ( type == 'i' ) {
                                                    value = toInt32( tmp );
                                                    i+=4;
                                            }
                                            if ( type == 'f' ) {
                                                    value = toFloat( tmp );
                                                    i+=4;
                                            }
                                            //qDebug() << " got" << value;

                                            if ( args.size() > 1 )
                                                    list.append( value );
                                            else
                                                    arguments = value;
                                    }

                                    if ( args.size() > 1 )
                                            arguments = list;
    //                        }
                    }
                    //qDebug() << "path seems to be" << path << "args are" << args << ":" << arguments;
                    /*

                    QMap<QString,QString> replacements;
                    replacements[ "!" ] = "^";
                    replacements[ "{" ] = "(";
                    replacements[ "}" ] = ")";
                    replacements[ "," ] = "|";
                    replacements[ "*" ] = ".*";
                    replacements[ "?" ] = ".";

                    foreach( QString rep, replacements.keys() )
                            path.replace( rep, replacements[ rep ] );

                    //qDebug() << " after transformation to OSC-RegExp path is" << path;

                    QRegExp exp( path );
                    foreach( PathObject* obj, paths ) {
                            if ( exp.exactMatch( obj->_path ) )
                                    obj->signalData( arguments );
                    } */
                }
        }
}

int QOscTcpClient::state() {return _tcp_socket->state();}

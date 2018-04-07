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

#include <sys/time.h>

#include <QtCore/QByteArray>
#include <QtCore/QString>
#include <QtNetwork/QUdpSocket>

#include "qosctypes.h"

QOscBase::QOscBase( QObject* p )
        : QObject( p )
        , _socket( 0 )
{
//    qDebug() << "QOscBase::QOscBase";
    setSocket( 0 );
}

QOscBase::QOscBase( QObject* p , bool no_socket)
        : QObject( p )
        , _socket( 0 )
{
    if(!no_socket)     setSocket( 0 );
}

void QOscBase::setSocket( QUdpSocket* s ) {
	if ( _socket ) {
		delete _socket;
		_socket = 0;
	}

	if ( !_socket ) {
		if ( s )
			_socket = s;
		else
			_socket = new QUdpSocket( this );
	}
}
QUdpSocket* QOscBase::socket() const { return _socket; }

void QOscBase::fillQByteArrayUp( QByteArray& in ) {
	while ( in.length() % 4 != 0 )
		in.append( char( 0 ) );
}
QByteArray QOscBase::reverseQByteArray( QByteArray in ) {
	QByteArray out;
	for ( int i=0; i<in.size(); ++i )
		out.prepend( in[ i ] );
	return out;
}
QByteArray QOscBase::fromString( QString str ) {
	QByteArray out = str.toUtf8();
	out.append( char( 0 ) );
	while ( out.length() % 4 != 0 )
		out.append( char( 0 ) );
	return out;
}
QByteArray QOscBase::fromInt32( qint32 i ) {
        QByteArray tmp( ( char* )static_cast<void*>( &i ), 4 );
        return reverseQByteArray( tmp );
}
QByteArray QOscBase::fromInt64( quint64 i ) {
        QByteArray tmp( ( char* )static_cast<void*>( &i ), 8 );
        return reverseQByteArray( tmp );
}
QByteArray QOscBase::fromFloat( float f ) {
	QByteArray tmp( ( char* )static_cast<void*>( &f ), 4 );
	return reverseQByteArray( tmp );
}
QString QOscBase::toString( const QByteArray& b ) {
	return QString( b.data() );
}
qint32 QOscBase::toInt32( const QByteArray& b ) {
	QByteArray tmp = b.left( 4 );
        tmp = reverseQByteArray( tmp );
	qint32* ret = ( qint32* )static_cast<void*>( tmp.data() );
	return *ret;
}
float QOscBase::toFloat( const QByteArray& b ) {
	QByteArray tmp = b.left( 4 );
        tmp = reverseQByteArray( tmp );
	float* ret = ( float* )static_cast<void*>( tmp.data() );
	return *ret;
}

void QOscBase::oscMessageParseArgs( const QVariant& data, QString& argtypes, QByteArray& arguments ) {
	if ( data.type() == QVariant::Int ) {
		argtypes += "i";
		arguments = arguments + fromInt32( data.toInt() );
	} else if ( data.type() == QVariant::Double ) {
        argtypes += "f";
        arguments += fromFloat( data.toFloat() );
    } else if ( data.type() == QVariant::String ) {
		argtypes += "s";
		arguments += fromString( data.toString() );
	} else if ( data.type() == QVariant::List ) {
		QList<QVariant> list = data.toList();
		foreach( QVariant v, list )
			oscMessageParseArgs( v, argtypes, arguments );
    } else {
        argtypes += "f";
        arguments += fromFloat( data.toFloat() );
    }
}

QByteArray QOscBase::oscMessage( QString path, QVariant arg ) {
	QString argtypes( "," );
	QByteArray arguments;

	oscMessageParseArgs( arg, argtypes, arguments );

	QByteArray out = fromString( path );
	out.append( fromString( argtypes ) );
	out.append( arguments );
	return out;
}

/*
PathObject::PathObject( QString path, QVariant::Type type, QOscClient* parent )
	: QObject( parent )
	, _path( path )
	, _type( type )
	, _client( parent )
	, _server( 0 )
{
}
PathObject::PathObject( QString path, QVariant::Type type, QOscServer* parent )
        : QObject( parent )
        , _path( path )
        , _type( type )
        , _client( 0 )
        , _server( parent )
{
        _server->registerPathObject( this );
}
PathObject::PathObject( QString path, QVariant::Type type, QOscServerInterface* parent )
        : QObject( 0 )
        , _path( path )
        , _type( type )
        , _client( 0 )
        , _server( parent )
{
        _server->registerPathObject( this );
}
PathObject::~PathObject() {
	if ( _server )
		_server->unregisterPathObject( this );
}

void PathObject::send( QVariant v ) {
	if ( v.type() == _type && _client )
		_client->sendData( _path, v );
}
void PathObject::send( int i ) { send( QVariant( i ) ); }
void PathObject::send( QString s ) { send( QVariant( s ) ); }
void PathObject::send( float d ) { send( QVariant( d ) ); }
void PathObject::send() { send( QVariant() ); }

void PathObject::signalData( QVariant v ) {

   //qDebug() << "signalData" << v.type();
    if ( v.type() == _type ) {
        if ( _type == QVariant::Invalid )
                emit data();
        if ( _type == QVariant::Int )
                emit data( v.toInt() );
        if ( _type == QVariant::float )
                emit data( v.tofloat() );
        if ( _type == QVariant::String )
                emit data( v.toString() );
        emit data( v );
    }
}
*/

unsigned long long QOscBase::getTimeTag()
{
    const unsigned long long EPOCH = 2208988800ULL;
    const unsigned long long NTP_SCALE_FRAC = 4294967295ULL;
    timeval tim;

    gettimeofday(&tim, NULL);

    unsigned long long tv_ntp, tv_usecs;

    tv_ntp = tim.tv_sec + EPOCH;
    tv_usecs = (NTP_SCALE_FRAC * tim.tv_usec) / 1000000UL;

    return (tv_ntp << 32) | tv_usecs;

}


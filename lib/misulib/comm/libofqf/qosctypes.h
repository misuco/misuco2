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

#ifndef QOSCTYPES_H
#define QOSCTYPES_H

#include <QtCore/QObject>
#include <QtCore/QByteArray>
#include <QtCore/QString>
#include <QtCore/QVariant>
#include <QtNetwork/QHostAddress>

/*
 * Will contain the conversions from Qt-types to OSC-types
 */

class QUdpSocket;

/**
 * @internal
 */
class QOscBase : public QObject
{
	Q_OBJECT
        protected:
                QOscBase( QObject* );
                QOscBase( QObject*, bool );
                QOscBase() { _socket=0; setSocket( 0 );}

		static void fillQByteArrayUp( QByteArray& in );
		static QByteArray reverseQByteArray( QByteArray in );

		static QByteArray fromString( QString );
                static QByteArray fromInt32( qint32 );
                static QByteArray fromInt64( quint64 );
                static QByteArray fromFloat( float );

		static QString toString( const QByteArray& );
		static qint32 toInt32( const QByteArray& );
		static float toFloat( const QByteArray& );

		static void oscMessageParseArgs( const QVariant&, QString&, QByteArray& );
		static QByteArray oscMessage( QString path, QVariant );

		virtual void setSocket( QUdpSocket* );
		QUdpSocket* socket() const;

                unsigned long long getTimeTag();

        private:
		QUdpSocket* _socket;
};

class QOscClient;
class QOscServer;
class QOscServerInterface;

/**
 * @brief Connect osc-messages with QObjects
 */
class PathObject // : public QObject
{
	public:
/*		PathObject( QString path, QVariant::Type type, QOscClient* parent );
                PathObject( QString path, QVariant::Type type, QOscServer* parent );
                PathObject( QString path, QVariant::Type type, QOscServerInterface * parent ); */
//            PathObject();
            virtual ~PathObject() {}
            virtual void signalData( QString, QVariant, QHostAddress *, quint16 )=0;
/*
	public slots:
		//@{
		void send( QVariant );
		void send( int );
		void send( QString );
		void send( float );
		void send();
		//@}

	signals:
		// @{
		void data( QVariant );
		void data( int );
		void data( float );
		void data( QString );
		void data();
		// @}
	private:
		// called by QOscServer:
		void signalData( QVariant );

		QString _path;
		QVariant::Type _type;
		QOscClient* _client;
                QOscServerInterface* _server;
*/
};

#endif // QOSCTYPES_H


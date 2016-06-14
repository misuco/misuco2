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

#ifndef QOSCSERVER_H
#define QOSCSERVER_H

#include <qosctypes.h>
#include "qoscserverinterface.h"

/*
 * will contain the definitions/classes of osc servers
 */

#include <QtNetwork/QHostAddress>

/**
 * @brief Server-object for incoming OSC-Datagrams
 *
 * Unless you have own objects/code to handle the paths of the incoming
 * packages, you might want to take a look at the PathObject to get the
 * OSC-messages translated into Qt signals.
 */
class QOscServer :  public QOscBase, public QOscServerInterface
{
	Q_OBJECT
        friend class PathObject;
        friend class QOscProxyInterface;
        friend class QOscProxy;
        public:
		/**
		 * Creates an OSC-server that listens on all interfaces on the specified
		 * port for incoming datagrams.
		 */
		QOscServer( quint16 port, QObject* p );
                /**
		 * Creates an OSC-server that listens on the specified address and port
		 * for incoming datagrams.
		 */
		QOscServer( QHostAddress address, quint16 port, QObject* p );
                QOscServer() {qDebug() << "QOscServer::QOscServer()";}
                /// destructor
                ~QOscServer();

		/**
		 * @brief Don't allow changing the socket.
		 */
                // virtual void setSocket( QUdpSocket*) {}
                void registerPathObject( PathObject* );
                void unregisterPathObject( PathObject* );

        signals:
                void data( QString path, QVariant data );

	private slots:
		void readyRead();

	private:
		QList<PathObject*> paths;
                int decodeState;
                bool inBundle;
                QString path;       // osc-path
                QString args;       // argument string
                int nargs;          // number of arguments
                int carg;           // currently parsed argument
                QVariant arguments; // the parsed arguments
};

#endif // QOSCSERVER_H


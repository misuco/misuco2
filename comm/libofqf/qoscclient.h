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

#ifndef QOSCCLIENT_H
#define QOSCCLIENT_H

#include "qosctypes.h"
#include "qoscclientinterface.h"

/**
 * will contain the definitions/classes of osc clients
 */

/**
 * @brief Client-object for outgoing OSC-Datagrams
 *
 * This class allows to send OSC-messages to a specified host/port.
 */
class QOscClient : public QOscBase, public QOscClientInterface
{
	Q_OBJECT
	public:
		/**
		 * @brief An OSC-connection to a specific host/port.
		 *
		 * This creates a new OSC-connection to the specified host/port. Later
		 * the servers host/port can be changed via setAddress();
		 */
        QOscClient(QObject* = nullptr );
        QOscClient(const QHostAddress&, quint16 port, QObject* = nullptr);
        QOscClient(const QHostAddress&, quint16 source_port, quint16 dst_port, QObject* = nullptr);
        ~QOscClient();

		/**
		 * @brief ( Re- )Set the host/port to send messages to
		 */
        void setAddress( char *, quint16 port );

        QHostAddress getIP();


	public slots:
		void sendData( QString, QVariant =QVariant::Invalid );
	
	private:
		QHostAddress _address;
		quint16 _port;
};

#endif // QOSCCLIENT_H

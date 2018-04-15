#ifndef QOSCTCPCLIENT_H
#define QOSCTCPCLIENT_H

#include <QTcpSocket>
#include "qosctypes.h"
#include "qoscclientinterface.h"

class QOscTcpClient : public QOscBase, public QOscClientInterface, public QOscServerInterface
{
    Q_OBJECT
public:
    explicit QOscTcpClient(QObject *parent = nullptr );
    /**
     * @brief An OSC-connection to a specific host/port.
     *
     * This creates a new OSC-connection to the specified host/port. Later
     * the servers host/port can be changed via setAddress();
     */
    QOscTcpClient( const QHostAddress&, quint16 port, QObject* = nullptr );
    QOscTcpClient( const QHostAddress&, quint16 source_port, quint16 dst_port, QObject* = nullptr );
    QOscTcpClient() {
//        qDebug() << "QOscTcpClient::QOscTcpClient()";
    }
    /// destructor
    ~QOscTcpClient();

    /**
     * @brief ( Re- )Set the host/port to send messages to
     */
    void setAddress( char * adr, quint16 port );

    void registerPathObject( PathObject* );
    void unregisterPathObject( PathObject* );

    int state();

signals:
    void data( QString, QVariant );
    void connected();

public slots:
    void sendData( QString, QVariant =QVariant::Invalid );

private slots:
    void tcpStateChanged();
    void readyRead();

private:
    QHostAddress _address;
    quint16 _source_port;
    quint16 _port;
    QTcpSocket * _tcp_socket;
    void setupTcpSocket();
    QList<PathObject*> paths;

};

#endif // QOSCTCPCLIENT_H

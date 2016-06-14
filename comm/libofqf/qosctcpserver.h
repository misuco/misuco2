#ifndef QOSCTCPSERVER_H
#define QOSCTCPSERVER_H

#include <QTcpServer>
#include <qoscserverinterface.h>
#include <qosctypes.h>

class QOscTcpServer : public QOscBase, public QOscServerInterface
{
    Q_OBJECT
public:
    explicit QOscTcpServer(QObject *parent = 0);

protected:
    void incomingConnection(int socketDescriptor);

private:
    QTcpServer * _tcp_server;

};

#endif // QOSCTCPSERVER_H

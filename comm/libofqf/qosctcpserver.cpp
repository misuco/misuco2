#include "qosctcpserver.h"
#include "qosctcpserverthread.h"

QOscTcpServer::QOscTcpServer(QObject *parent) :
    QOscBase(parent,true)
{
}

void QOscTcpServer::incomingConnection(int socketDescriptor)
{
    QOscTcpServerThread * thread = new QOscTcpServerThread(socketDescriptor, this);
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();

}

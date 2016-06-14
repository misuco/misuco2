#include "qoscproxy.h"
#include <QVariant>

QOscProxy::QOscProxy(QObject * p, QHostAddress adr_server, QHostAddress adr_client, quint16 port_server, quint16 port_client) : QObject( p )
{
    Client = new QOscClient(adr_client,port_server,port_client,this); // client has source port of server
    Server = new QOscServer(adr_server,port_server,this);
}

QOscProxy::QOscProxy(QObject * p, QHostAddress adr, quint16 port) : QObject( p )
{
    QOscProxy(p,adr,adr,port,port);
}

void QOscProxy::setAddress(char *, quint16)
{
}

void QOscProxy::setAnswerAddress(QOscServer *)
{
}

void QOscProxy::setSocket(QUdpSocket *)
{
}

void QOscProxy::sendData(QString path, QVariant dat)
{
    Client->sendData(path,dat);
}

void QOscProxy::sendData(QString path, QList<QVariant> dat)
{
    Client->sendData(path,dat);
}

void QOscProxy::registerPathObject(PathObject * po)
{
    Server->registerPathObject(po);
}

void QOscProxy::unregisterPathObject(PathObject *)
{
}

void QOscProxy::readyRead()
{
    emit readyRead();
}



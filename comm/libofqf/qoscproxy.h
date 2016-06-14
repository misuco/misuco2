#ifndef _QOSCPROXY_H
#define _QOSCPROXY_H

#include "qoscproxyinterface.h"
#include "qoscclient.h"

class QOscProxy : public QObject, public QOscProxyInterface  {

    Q_OBJECT
    public:
        QOscProxy(QObject *, QHostAddress adr_srv, QHostAddress adr_cli, quint16 port_srv, quint16 port_cli);
        QOscProxy(QObject *, QHostAddress a, quint16 port);

        virtual void setAddress( char * adr, quint16 port );
        virtual void setAnswerAddress( QOscServer* );
        virtual void setSocket( QUdpSocket* );
        virtual void registerPathObject( PathObject* );
        virtual void unregisterPathObject( PathObject* );

    signals:
        void data( QString path, QVariant dat );
        void readyReadFwd();

    public slots:
        void sendData( QString, QVariant );
        void sendData( QString, QList<QVariant> );

    private slots:
        void readyRead();

    private:
        QOscClient * Client;
        QOscServer * Server;
//        QUdpSocket * Socket;
};
#endif

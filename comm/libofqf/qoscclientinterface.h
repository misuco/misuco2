#ifndef QOSCCLIENTINTERFACE_H
#define QOSCCLIENTINTERFACE_H

#include <QtNetwork/QHostAddress>
#include "qoscserver.h"

class QOscClientInterface
{
public:
    virtual ~QOscClientInterface() {}
    virtual void setAddress( char * adr, quint16 port ) = 0;

public slots:
    virtual void sendData( QString, QVariant ) = 0;

};

#endif // QOSCCLIENTINTERFACE_H

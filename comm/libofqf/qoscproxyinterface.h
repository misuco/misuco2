#ifndef QOSCPROXYINTERFACE_H
#define QOSCPROXYINTERFACE_H

#include "qoscclientinterface.h"
#include "qoscserverinterface.h"

class QOscProxyInterface: public QOscClientInterface, public QOscServerInterface
{
public:
    virtual ~QOscProxyInterface() {}
};

#endif // QOSCPROXYINTERFACE_H

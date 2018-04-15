#ifndef QOSCSERVERINTERFACE_H
#define QOSCSERVERINTERFACE_H

#include "qosctypes.h"

class QOscServerInterface
{
public:
    virtual ~QOscServerInterface() {}
    virtual void registerPathObject( PathObject* ) = 0;
    virtual void unregisterPathObject( PathObject* ) = 0;

signals:
    virtual void oscData( QString, QList<QVariant> ) = 0;

};

#endif // QOSCSERVERINTERFACE_H

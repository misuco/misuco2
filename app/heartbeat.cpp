#include <QTimerEvent>
#include "heartbeat.h"

Heartbeat::Heartbeat(QObject *parent) : QObject(parent),
    _beatNo(1),
    _scaleNo(1),
    _scaleChangePropagated(1)
{
    _out = new QOscClient(QHostAddress::Broadcast,3150,this);
    _timerId = startTimer(100);
}

void Heartbeat::timerEvent(QTimerEvent * event)
{
    // on scale change increase frequency of heartbeat
    // for the next ten heartbeats
    // to make sure, the scale switch arrives
    // even if some udp packets are lost
    if(_scaleChangePropagated<10) {
        _scaleChangePropagated++;
    } else if(_scaleChangePropagated == 10) {
        _scaleChangePropagated++;
        killTimer(event->timerId());
        startTimer(5000);
    }

    QList<QVariant> list;
    list.append(_beatNo);
    list.append(_scaleNo);
    list.append(_scale.rootNote);
    for(int i=0;i<BSCALE_SIZE;i++) {
        if(_scale.bscale[i]) {
            list.append(i);
        }
    }
    _out->sendData("/hb",list);
    _beatNo++;
}

void Heartbeat::onSetRootNote(int p)
{
    _scale.rootNote = p;
    _scaleChangePropagated = 0;
    _scaleNo++;
    killTimer(_timerId);
    _timerId = startTimer(100);
    propagateScale();
}


void Heartbeat::onSetBscale(int n, bool v)
{
    _scale.bscale[n-1]=v;
    propagateScale();
}

void Heartbeat::onScaleSet(MWScale * scale)
{
    _scale.baseoct=scale->baseoct;
    _scale.topoct=scale->topoct;
    for(int i=0;i<BSCALE_SIZE;i++) {
        _scale.bscale[i]=scale->bscale[i];
    }
    _scale.rootNote=scale->rootNote;
    propagateScale();
}

void Heartbeat::propagateScale()
{
    _scaleChangePropagated = 0;
    _scaleNo++;
    killTimer(_timerId);
    _timerId = startTimer(100);
}

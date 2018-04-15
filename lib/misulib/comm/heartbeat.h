#ifndef HEARTBEAT_H
#define HEARTBEAT_H

#include <QObject>
#include "lib/misulib/models/types.h"
#include "lib/misulib/comm/libofqf/qoscclient.h"

class Heartbeat : public QObject
{
    Q_OBJECT
public:
    explicit Heartbeat(QObject *parent = nullptr);

protected:
    void timerEvent(QTimerEvent *event);

signals:

public slots:    
    void onSetRootNote(int p);
    void onSetScale(int n, bool v);
    void onSetScale(int rootNote, QList<bool>);

private:
    int _rootNote;
    QList<bool> _scale;
    QOscClient * _out;
    int _beatNo;
    int _scaleNo;
    int _scaleChangePropagated;
    int _timerId;

    void propagateScale();
};

#endif // HEARTBEAT_H

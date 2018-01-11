#ifndef MWFADERPITCH_H
#define MWFADERPITCH_H

#include <QObject>
#include <QWidget>
#include <widgets/mwfadder.h>
#include <conf/freqtriple.h>
#include "comm/isender.h"

class MWFaderPitch : public MWFadder
{
    Q_OBJECT

public:
    MWFaderPitch(QWidget *parent, Pitch * p);
    ~MWFaderPitch();
    virtual void processTouchEvent(misuTouchEvent e);
    void setOut(ISender *value);

public slots:
    void setOctMid(int o);
    void pitchChange();

private:
    ISender * out;

    Pitch * p;
    FreqTriple * f;

    int vId;
    int pressed;
};

#endif // MWFADERPITCH_H

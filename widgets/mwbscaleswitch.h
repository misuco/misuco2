#ifndef MWBSCALESWITCH_H
#define MWBSCALESWITCH_H

#include "misuwidget.h"
#include "freqtriple.h"
#include "comm/isender.h"

class MWBScaleSwitch : public MisuWidget
{
    Q_OBJECT

public:
    MWBScaleSwitch(int i, Pitch ** MWP);
    ~MWBScaleSwitch();
    void setOut(ISender *value);

signals:
    void setBscale(int,bool);

public slots:
    void setBaseNote(Pitch * p);
    void setOctMid(int o);

protected:
    void paintEvent(QPaintEvent *);
    void processTouchEvent(misuTouchEvent e);

private:
    ISender * out;
    FreqTriple * f;    
    Pitch * MWPitch[12];
    int bscaleId;
    int basenote;
    int vId;
    int pressed;
    bool value;
    int chan;
};

#endif // MWBSCALESWITCH_H

#ifndef MWBSCALESWITCH_H
#define MWBSCALESWITCH_H

#include "misuwidget.h"
#include "freqtriple.h"
#include "comm/isender.h"

class MWBScaleSwitch : public MisuWidget
{
    Q_OBJECT

public:
    MWBScaleSwitch(int Id);
    ~MWBScaleSwitch();
    void setOut(ISender *value);

signals:
    void setBscale(int,bool);

public slots:
    void setBasenote(int b);

protected:
    void paintEvent(QPaintEvent *);
    void processTouchEvent(misuTouchEvent e);

private:
    ISender * out;
    FreqTriple * f;
    int bscaleId;
    int basenote;
    int vId;
    int pressed;
    bool value;
    int chan;
};

#endif // MWBSCALESWITCH_H

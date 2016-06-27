#ifndef MWBASENOTESETTER_H
#define MWBASENOTESETTER_H

#include "misuwidget.h"
#include "comm/isender.h"
#include "conf/freqtriple.h"

class MWBaseNoteSetter : public MisuWidget
{
    Q_OBJECT

public:
    MWBaseNoteSetter(FreqTriple * note);
    ~MWBaseNoteSetter();
    virtual void processTouchEvent(misuTouchEvent e);
    void setOut(ISender *value);

protected:
    void paintEvent(QPaintEvent *E);

signals:
    void setBaseNote(int b);

private:
    ISender * out;
    FreqTriple * f;
    int vId=0;
    int pressed=0;
    int chan=1;
    int oct=4;
};

#endif // MWBASENOTESETTER_H

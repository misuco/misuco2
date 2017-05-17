#ifndef MWBASENOTESETTER_H
#define MWBASENOTESETTER_H

#include "misuwidget.h"
#include "comm/isender.h"
#include "conf/freqtriple.h"

class MWBaseNoteSetter : public MisuWidget
{
    Q_OBJECT

public:
    MWBaseNoteSetter(Pitch *pitch, QWidget *parent);
    ~MWBaseNoteSetter();
    virtual void processTouchEvent(misuTouchEvent e);
    void setOut(ISender *value);

protected:
    void paintEvent(QPaintEvent *E);
    void resizeEvent(QResizeEvent *E);

public slots:
    void setOctMid(int o);
    void setChan(int c);
    void pitchChange();

signals:
    void setBaseNote(Pitch *);

private:
    ISender * out;
    Pitch * p;
    FreqTriple * f;
    int vId;
    int pressed;
    int chan;
    int oct;
};

#endif // MWBASENOTESETTER_H

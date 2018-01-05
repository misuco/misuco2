#ifndef MWFaderParamCtl_H
#define MWFaderParamCtl_H

#include <QObject>
#include <QWidget>
#include <widgets/mwfadder.h>
#include <conf/freqtriple.h>
#include "comm/isender.h"

class MWFaderParamCtl : public MWFadder
{
    Q_OBJECT

public:
    MWFaderParamCtl(QWidget *parent, Color *col, int cc, int c);
    ~MWFaderParamCtl();
    virtual void processTouchEvent(misuTouchEvent e);
    void setOut(ISender *value);
    void setValue(int v);

public slots:
    void setChan(int c);

private:
    ISender * out;

    int cc;
    int pressed;
    int chan;
    void propagateValueChange();
};

#endif // MWFaderParamCtl_H

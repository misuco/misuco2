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
    MWFaderParamCtl(QWidget *parent, Color *col, int cc);
    ~MWFaderParamCtl();
    virtual void processTouchEvent(misuTouchEvent e);
    void setOut(ISender *value);
    void setValue(int v);

protected:
    void paintEvent(QPaintEvent * E);

private:
    ISender * out;

    int cc;
    int pressed;
    void propagateValueChange();
};

#endif // MWFaderParamCtl_H

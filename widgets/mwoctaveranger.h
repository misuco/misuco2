#ifndef MWOCTAVERANGER_H
#define MWOCTAVERANGER_H

#include "misuwidget.h"

class MWOctaveRanger : public MisuWidget
{
    Q_OBJECT

public:
    MWOctaveRanger(QWidget *parent);
    ~MWOctaveRanger();
    virtual void processTouchEvent(misuTouchEvent e);

signals:
    void setOctConf(int bottom,int mid,int top);

protected:
    void paintEvent(QPaintEvent *E);
    void resizeEvent(QResizeEvent *E);

private:
    enum border {none,topPad,bottomPad};
    int top=4;
    int mid=3;
    int bottom=2;
    int segwidth;
    int noct=10;
    border grabed=none;
};

#endif // MWOCTAVERANGER_H

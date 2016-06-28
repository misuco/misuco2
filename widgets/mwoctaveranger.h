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
    int top;
    int mid;
    int bottom;
    int segwidth;
    int noct;
    border grabed;
};

#endif // MWOCTAVERANGER_H

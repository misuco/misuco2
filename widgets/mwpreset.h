#ifndef MWPRESET_H
#define MWPRESET_H

#include "misuwidget.h"
#include "conf/types.h"

class MWPreset : public MisuWidget
{
    Q_OBJECT

public:
    MWPreset(Pitch * p[], QWidget *parent);
    virtual void processTouchEvent(misuTouchEvent e);
    MWScale Scale;

protected:
    void paintEvent(QPaintEvent *E);
    void resizeEvent(QResizeEvent *E);

signals:
    void setScale(MWScale *);

private:
    Pitch ** MWPitch;
    int pressed=0;
    void setBrush(Pitch *p, int s, int l, QPainter &painter);
};

#endif // MWPRESET_H

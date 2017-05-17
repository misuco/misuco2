#include "mwfadder.h"
#include <QPainter>
#include <QDebug>

MWFadder::MWFadder(QWidget *parent, Color *c) : MisuWidget(parent)
{
    qDebug() << "MWFadder::MWFadder";
    orient=vertical;
    value=0;
    pressed=0;
    knobSize=50;
    setMinValue(-100);
    setMaxValue(100);
    fineness=5;
    color=c;
    calcGeo();
    update();
}

void MWFadder::processTouchEvent(misuTouchEvent e)
{
    switch(e.state) {
    case Qt::TouchPointPressed:
        if(0==pressed) {
            xTouchBegin=e.x;
            yTouchBegin=e.y;
            valTouchBegin=value;
            if(e.y>=fadderY && e.y<=fadderY+2*knobSize) {
                fadeMode=coarse;
            } else {
                fadeMode=fine;
            }
            emit valueChange(value);
        }
        pressed++;
        update();
        break;
    case Qt::TouchPointMoved:
        if(vertical==orient) {
            if(coarse==fadeMode) {
                value=valTouchBegin+(e.y-yTouchBegin)*valRange/(height()-2*knobSize);
            } else {
                value=valTouchBegin+(e.y-yTouchBegin)/fineness;
            }
            if(value>maxValue) value=maxValue;
            if(value<minValue) value=minValue;
            emit valueChange(value);
            calcGeo();
        }
        update();
        break;
    case Qt::TouchPointReleased:
        pressed--;
        update();
        break;
    }
    qDebug() << "value " << value << " touch begin v: " << valTouchBegin << " y: " << yTouchBegin;
}

void MWFadder::resizeEvent(QResizeEvent *)
{
    calcGeo();
}

void MWFadder::calcGeo()
{
    fadderY=(value-minValue)*(height()-2*knobSize)/valRange;
}

void MWFadder::setMaxValue(int value)
{
    maxValue = value;
    valRange=maxValue-minValue;
}

void MWFadder::setMinValue(int value)
{
    minValue = value;
    valRange=maxValue-minValue;
}

void MWFadder::paintEvent(QPaintEvent *E)
{
    QPainter painter(this);
    QString cap;

    if(pressed>0) painter.setBrush(Qt::white);
    else painter.setBrush(Qt::gray);
    painter.drawRect(0,0,width(),height());

    painter.setBrush(QColor::fromHsl(color->getHue(),150,150));
    //painter.drawRect(0,fadderY-knobSize,width(),knobSize*2);
    painter.drawRect(0,fadderY,width(),knobSize*2);

    painter.setBrush(Qt::black);
    cap.sprintf("%d",value);
    painter.drawText(0,0,width(),height(),Qt::AlignTop|Qt::AlignLeft,cap);
    qDebug() << "MWFadder::paintEvent hue " << color->getHue() << " value " << value;
}


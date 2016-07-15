#include "mwfadder.h"
#include <QPainter>
#include <QDebug>

MWFadder::MWFadder(QWidget *parent) : MisuWidget(parent)
{
    orient=vertical;
    value=0;
    knobSize=10;
    setMinValue(-1000);
    setMaxValue(1000);
    fineness=5;
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
            if(e.y>=fadderY-knobSize && e.y<=fadderY+knobSize) {
                fadeMode=coarse;
            } else {
                fadeMode=fine;
            }
        }
        pressed++;
        update();
        break;
    case Qt::TouchPointMoved:
        if(vertical==orient) {
            if(coarse==fadeMode) {
                value=valTouchBegin+(e.y-yTouchBegin)*valRange/height();
            } else {
                value=valTouchBegin+(e.y-yTouchBegin)/fineness;
            }
            if(value>maxValue) value=maxValue;
            if(value<minValue) value=minValue;
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

void MWFadder::calcGeo()
{
    fadderY=(value-minValue)*height()/valRange;
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
    painter.setBrush(Qt::black);
    cap.sprintf("%d",value);
    painter.drawText(0,0,width(),height(),Qt::AlignTop|Qt::AlignLeft,cap);
    painter.drawRect(0,fadderY-knobSize,width(),knobSize*2);
}


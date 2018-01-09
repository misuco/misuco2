#include "mwfadder.h"
#include <QPainter>
#include <QDebug>

MWFadder::MWFadder(QWidget *parent, Color *c) : MisuWidget(parent)
{
    //qDebug() << "MWFadder::MWFadder";
    orient=vertical;
    value=0;
    inverted=false;
    valueDisplay=value;
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
            emit valueChange(valueDisplay);
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
            if(inverted) {
                valueDisplay = maxValue - value;
            } else {
                valueDisplay = value;
            }
            emit valueChange(valueDisplay);
            calcGeo();
        }
        update();
        break;
    case Qt::TouchPointReleased:
        pressed--;
        update();
        break;
    }
    //qDebug() << "value " << value << " touch begin v: " << valTouchBegin << " y: " << yTouchBegin;
}

void MWFadder::resizeEvent(QResizeEvent *)
{
    calcGeo();
}

int MWFadder::getValue()
{
    return valueDisplay;
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

void MWFadder::setInverted(bool v)
{
    inverted = v;
    if(inverted) {
        value=maxValue;
    }
}

void MWFadder::setValue(int v)
{
    if(inverted) {
        value = maxValue - v;
        valueDisplay = v;
    } else {
        valueDisplay = v;
        value = v;
    }
    calcGeo();
    update();
}

void MWFadder::setMinValue(int value)
{
    minValue = value;
    valRange=maxValue-minValue;
}

void MWFadder::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QString cap;

    painter.setBrush(bgcolor);
    painter.setPen(fgcolor);

    painter.drawRect(0,0,width(),height());

    if(pressed>0) painter.setBrush(QColor::fromHsl(color->getHue(),sOn,lOn));
    else painter.setBrush(QColor::fromHsl(color->getHue(),sOff,lOff));
    //painter.drawRect(0,fadderY-knobSize,width(),knobSize*2);
    painter.drawRect(0,fadderY,width(),knobSize*2);

    painter.setBrush(Qt::black);
    QFont font("font1");
    font.setPixelSize(font1size);
    painter.setFont(font);
    cap.sprintf("%d",valueDisplay);
    painter.drawText(0,0,width(),height(),Qt::AlignTop|Qt::AlignLeft,cap);
    //qDebug() << "MWFadder::paintEvent hue " << color->getHue() << " value " << value;
}


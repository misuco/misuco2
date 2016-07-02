#include "mwheadersetter.h"
#include <QPainter>

MWHeaderSetter::MWHeaderSetter(int headerId, QWidget *parent) : MisuWidget(parent)
{
    this->headerId=headerId;
    pressed=0;
}

void MWHeaderSetter::processTouchEvent(misuTouchEvent e)
{
    switch(e.state) {
    case Qt::TouchPointPressed:
        emit currentHeader(headerId);
        pressed++;
        update();
        break;
    case Qt::TouchPointReleased:
        pressed--;
        update();
        break;
    }
}

void MWHeaderSetter::paintEvent(QPaintEvent *E)
{
    QPainter painter(this);
    QString cap;
    if(pressed>0) painter.setBrush(Qt::white);
    else painter.setBrush(Qt::gray);
    painter.drawRect(0,0,width(),height());
    cap.sprintf("%d",headerId);
    painter.drawText(0,0,width(),height(),Qt::AlignTop|Qt::AlignLeft,cap);
}

void MWHeaderSetter::resizeEvent(QResizeEvent *E)
{

}

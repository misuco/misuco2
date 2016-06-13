#include "wnote.h"
#include <QPainter>

#include <QDebug>


wNote::wNote(QWidget *parent) : MisuWidget(parent)
{
    color.setHsl(id*10,180,120);
    cap.sprintf("%d",id);
    //setMinimumSize(100,100);
    //setVisible(true);
    qDebug() << "created wNote " << id;
}

wNote::~wNote()
{
    qDebug() << "destroyed wNote " << id;
}

void wNote::paintEvent(QPaintEvent *)
{
    qDebug() << "wNote::paintEvent()" << id;
    QPainter painter(this);
    painter.setBrush(color);
    painter.fillRect(0,0,width(),height(),color);
    painter.drawText(0,0,width(),height(),0,cap);
}

void wNote::processTouchEvent(misuTouchEvent e)
{

}


#include "mwoctaveranger.h"
#include <QPainter>
#include <QDebug>

MWOctaveRanger::MWOctaveRanger(QWidget *parent) : MisuWidget(parent)
{
    //qDebug() << "MWOctaveRanger::MWOctaveRanger()";
    //setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    top=4;
    mid=3;
    bottom=3;
    noct=10;
    grabed=none;
}

MWOctaveRanger::~MWOctaveRanger()
{

}

void MWOctaveRanger::processTouchEvent(MisuWidget::misuTouchEvent e)
{
    int touchOct;
    switch(e.state) {
    case Qt::TouchPointPressed:
    case Qt::TouchPointMoved:
        touchOct=e.x/segwidth;
        if(touchOct==bottom) {
            grabed=bottomPad;
        } else if( touchOct==top) {
            grabed=topPad;
        } else {
            if(touchOct<bottom) {
                bottom=touchOct;
            } else if(touchOct>top) {
                top=touchOct;
            } else {
                if(grabed==topPad) {
                    top=touchOct;
                } else if(grabed==bottomPad) {
                    bottom=touchOct;
                } else {
                    int distTop=top-touchOct;
                    int distBottom=touchOct-bottom;
                    if(distTop>distBottom) {
                        bottom=touchOct;
                    }
                    if(distTop<distBottom) {
                        top=touchOct;
                    }
                }
            }
            emit setOctConf(bottom,top);
            int midnew=bottom+(top-bottom)/2;
            if(midnew!=mid) {
                mid=midnew;
                emit setOctMid(mid);
            }
            update();
        }
        break;
    case Qt::TouchPointReleased:
        grabed=none;
        break;
    }

}

void MWOctaveRanger::paintEvent(QPaintEvent *E)
{
    //qDebug() << "MWOctaveRanger::paintEvent";
    QPainter painter(this);
    QString cap;
    painter.setBrush(Qt::gray);
    painter.drawRect(bottom*segwidth,0,(1+top-bottom)*segwidth,height());
    for(int i=0;i<noct;i++) {
        cap.sprintf("%d",i);
        painter.drawText(i*segwidth,0,segwidth,height(),Qt::AlignCenter|Qt::AlignHCenter,cap);
    }
}

void MWOctaveRanger::resizeEvent(QResizeEvent *E)
{
    //qDebug() << "MWOctaveRanger::resizeEvent";
    segwidth=width()/noct;
}


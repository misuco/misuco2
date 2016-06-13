#include "misuwidget.h"
#include <QEvent>
#include <QTouchEvent>
#include <QDateTime>
#include <QDebug>

MisuWidget::MisuWidget(QWidget *parent) : QWidget(parent)
{
    static int nextId=0;
    id=nextId++;
}

MisuWidget::~MisuWidget()
{

}

bool MisuWidget::event(QEvent *event)
{
    misuTouchEvent e;
    if( event->type()==QEvent::TouchEnd ||
        event->type()==QEvent::TouchUpdate ||
        event->type()==QEvent::TouchBegin ) {

        e.t=QDateTime::currentMSecsSinceEpoch();
        QList<QTouchEvent::TouchPoint> touchPoints = static_cast<QTouchEvent *>(event)->touchPoints();
        foreach (const QTouchEvent::TouchPoint &touchPoint, touchPoints) {
            //qDebug() << "touch wnote:" << id << " tid:" << touchPoint.id() <<  " x:" << touchPoint.pos().x() << " y:" << touchPoint.pos().y() << " t: " << t ;
            e.id=touchPoint.id();
            e.state=event->type();
            e.x=touchPoint.pos().x();
            e.y=touchPoint.pos().y();
            emit touchEvent(e);
        }
        return true;
    } else if(  event->type()==QEvent::MouseMove ||
                event->type()==QEvent::MouseButtonPress ||
                event->type()==QEvent::MouseButtonRelease ) {

        const QMouseEvent * meve = static_cast<QMouseEvent *>(event);
        e.t=QDateTime::currentMSecsSinceEpoch();
        e.state=Qt::TouchPointMoved;
        if(event->type()==QEvent::MouseMove) {
            e.state=Qt::TouchPointMoved;
        } else if(event->type()==QEvent::MouseButtonPress) {
            e.state=Qt::TouchPointPressed;
        } else if(event->type()==QEvent::MouseButtonRelease) {
            e.state=Qt::TouchPointReleased;
        }
        //qDebug() << "mouse wnote " << id  << " x:" << meve->pos().x() << " y:" << meve->pos().y() << " t: " << t;
        e.id=9999;
        e.x=meve->pos().x();
        e.y=meve->pos().y();
        emit touchEvent(e);
        return true;
    }
    return QWidget::event(event);
}

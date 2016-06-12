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
    if( event->type()==QEvent::TouchEnd ||
        event->type()==QEvent::TouchUpdate ||
        event->type()==QEvent::TouchBegin ) {

        long t=QDateTime::currentMSecsSinceEpoch();

        QList<QTouchEvent::TouchPoint> touchPoints = static_cast<QTouchEvent *>(event)->touchPoints();
        foreach (const QTouchEvent::TouchPoint &touchPoint, touchPoints) {
            qDebug() << "touch wnote:" << id << " tid:" << touchPoint.id() <<  " x:" << touchPoint.pos().x() << " y:" << touchPoint.pos().y() << " t: " << t ;
            emit touchEvent(touchPoint.id(),event->type(),touchPoint.pos().x(),touchPoint.pos().y(),t);
        }
        return true;
    } else if(  event->type()==QEvent::MouseMove ||
                event->type()==QEvent::MouseButtonPress ||
                event->type()==QEvent::MouseButtonRelease ) {

        long t=QDateTime::currentMSecsSinceEpoch();

        const QMouseEvent * meve = static_cast<QMouseEvent *>(event);

        qDebug() << "mouse wnote " << id  << " x:" << meve->pos().x() << " y:" << meve->pos().y() << " t: " << t;
        emit touchEvent(9999, event->type(), meve->pos().x(),meve->pos().y(),t);
        return true;
    }
    return QWidget::event(event);
}

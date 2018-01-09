#include "misuwidget.h"
#include <QEvent>
#include <QTouchEvent>
#include <QDateTime>
#include <QDebug>

QColor MisuWidget::fgcolor(QColor::fromRgb(200,200,200));
QColor MisuWidget::bgcolor(QColor::fromRgb(25,25,25));
QColor MisuWidget::highlightcolor("Red");
QString MisuWidget::font1("Sans");
int MisuWidget::font1size(10);

bool MisuWidget::bwmode = true;
QColor MisuWidget::bkeycolor("Black");
QColor MisuWidget::wkeycolor("White");
QColor MisuWidget::hlbkeycolor(QColor::fromHsl(1,200,20));
QColor MisuWidget::hlwkeycolor(QColor::fromHsl(1,200,200));

int MisuWidget::lOff = 80;
int MisuWidget::lOn = 180;
int MisuWidget::sOff = 180;
int MisuWidget::sOn = 180;

MWScale MisuWidget::Scale;
MWSound MisuWidget::Sound;
bool MisuWidget::overwrite = false;
int MisuWidget::lang = 0;

MisuWidget::MisuWidget(QWidget *parent) : QWidget(parent)
{
    //qDebug() << "MisuWidget::MisuWidget";
    static int nextId=0;
    id=nextId++;
    setAttribute(Qt::WA_AcceptTouchEvents,true);
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

        //qDebug() << "Touch event:";

        e.t=QDateTime::currentMSecsSinceEpoch();
        QList<QTouchEvent::TouchPoint> touchPoints = static_cast<QTouchEvent *>(event)->touchPoints();
        foreach (const QTouchEvent::TouchPoint &touchPoint, touchPoints) {
            //qDebug() << "-- touch wnote:" << id <<  " state " <<  touchPoint.state() << " tid:" << touchPoint.id() <<  " x:" << touchPoint.pos().x() << " y:" << touchPoint.pos().y() ;
            e.id=touchPoint.id();
            e.state=touchPoint.state();
            e.x=touchPoint.pos().x();
            e.y=touchPoint.pos().y();
            processTouchEvent(e);
        }
        return true;
    } else if(  event->type()==QEvent::MouseMove ||
                event->type()==QEvent::MouseButtonPress ||
                event->type()==QEvent::MouseButtonRelease ) {

        //qDebug() << "Mouse event:";

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
        //qDebug() << "mouse wnote " << id  << " x:" << meve->pos().x() << " y:" << meve->pos().y() ;
        e.id=9999;
        e.x=meve->pos().x();
        e.y=meve->pos().y();
        processTouchEvent(e);
        return true;
    }
    return QWidget::event(event);
}

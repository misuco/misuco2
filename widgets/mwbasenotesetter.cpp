#include "mwbasenotesetter.h"
#include "comm/senderdebug.h"
#include <QPainter>
#include <QDebug>

MWBaseNoteSetter::MWBaseNoteSetter(int note)
{
    out=new SenderDebug();
    f.setMidinote(note);
    f.setOct(4);
    setMinimumHeight(50);
    setMinimumWidth(50);
}

MWBaseNoteSetter::~MWBaseNoteSetter()
{

}

void MWBaseNoteSetter::processTouchEvent(misuTouchEvent e)
{
    switch(e.state) {
    case Qt::TouchPointPressed:
        vId=out->noteOn(chan,f.getFreq(),f.getMidinote(),f.getPitch(),127);
        qDebug() << "MWBaseNoteSetter::processTouchEvent TouchPointPressed " << out << " vId:" << vId;
        emit setBaseNote(f.getBasenote());
        pressed++;
        update();
        break;
    case Qt::TouchPointReleased:
        qDebug() << "MWBaseNoteSetter::processTouchEvent TouchPointReleased vId:" << vId;
        out->noteOff(vId);
        pressed--;
        update();
        break;
    }

}

void MWBaseNoteSetter::paintEvent(QPaintEvent *E)
{
    QPainter painter(this);
    QString cap;
    int l=127;
    if(pressed>0) l=200;
    painter.setBrush(QColor::fromHsl(f.getHue(),127,l));
    painter.drawRect(0,0,width(),height());
    cap.sprintf("%d",f.getBasenote());
    painter.drawText(0,0,width(),height(),Qt::AlignCenter|Qt::AlignHCenter,cap);
}

void MWBaseNoteSetter::setOut(ISender *value)
{
    out = value;
    qDebug() << "MWBaseNoteSetter::setOut:" << out;
}


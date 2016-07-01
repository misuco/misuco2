#include "mwbasenotesetter.h"
#include "comm/senderdebug.h"
#include <QPainter>
#include <QDebug>

MWBaseNoteSetter::MWBaseNoteSetter(int note)
{
    out=new SenderDebug();
    f=new FreqTriple();
    f->setBasenote(note);
    vId=0;
    pressed=0;
    chan=1;
    oct=4;
}

MWBaseNoteSetter::~MWBaseNoteSetter()
{
    delete(f);
}

void MWBaseNoteSetter::processTouchEvent(misuTouchEvent e)
{
    switch(e.state) {
    case Qt::TouchPointPressed:
        vId=out->noteOn(chan,f->getFreq(),f->getMidinote(),f->getPitch(),127);
        //qDebug() << "MWBaseNoteSetter::processTouchEvent TouchPointPressed " << out << " vId:" << vId;
        emit setBaseNote(f->getBasenote());
        qDebug() << "MWBaseNoteSetter::processTouchEvent emit setBaseNote " << f->getBasenote();
        pressed++;
        update();
        break;
    case Qt::TouchPointReleased:
        //qDebug() << "MWBaseNoteSetter::processTouchEvent TouchPointReleased vId:" << vId;
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
    painter.setBrush(QColor::fromHsl(f->getHue(),127,l));
    painter.drawRect(0,0,width(),height());
    cap.sprintf("%d",f->getBasenote());
    painter.drawText(0,0,width(),height(),Qt::AlignTop|Qt::AlignLeft,cap);
    painter.drawText(0,0,width(),height(),Qt::AlignTop|Qt::AlignRight,"*");
    painter.drawText(0,0,width(),height(),Qt::AlignBottom|Qt::AlignRight,"*");
}

void MWBaseNoteSetter::resizeEvent(QResizeEvent *E)
{
    qDebug() << "MWBaseNoteSetter::resizeEvent" << width();
}

void MWBaseNoteSetter::setOctMid(int o)
{
    f->setOct(o);
}

void MWBaseNoteSetter::setChan(int c)
{
    chan=c;
}

void MWBaseNoteSetter::setOut(ISender *value)
{
    out = value;
    //qDebug() << "MWBaseNoteSetter::setOut:" << out;
}


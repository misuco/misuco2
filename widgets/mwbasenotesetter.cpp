#include "mwbasenotesetter.h"
#include "comm/senderdebug.h"
#include <QPainter>
#include <QDebug>

MWBaseNoteSetter::MWBaseNoteSetter(Pitch * pitch, QWidget *parent) : MisuWidget(parent)
{
    qDebug() << "MWBaseNoteSetter::MWBaseNoteSetter " << pitch->pitch << " pitch basenote " << pitch->basenote;
    out=new SenderDebug();
    oct=4;
    p=pitch;
    f=new FreqTriple(p);
    f->setOct(oct);
    qDebug() << "f: " << f->getFreq() << " " << f->getPitch() << " " << f->getHue();
    vId=0;
    pressed=0;
    chan=1;
}

MWBaseNoteSetter::~MWBaseNoteSetter()
{
    f->deleteLater();
}

void MWBaseNoteSetter::processTouchEvent(misuTouchEvent e)
{
    switch(e.state) {
    case Qt::TouchPointPressed:
        vId=out->noteOn(chan,f->getFreq(),f->getMidinote(),f->getPitch(),127);
        qDebug() << "MWBaseNoteSetter::processTouchEvent TouchPointPressed " << out << " vId:" << vId;
        emit setBaseNote(p);
        qDebug() << "MWBaseNoteSetter::processTouchEvent emit setBaseNote " << f->getBasenote();
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
    painter.setBrush(QColor::fromHsl(f->getHue(),127,l));
    painter.drawRect(0,0,width(),height());
    cap.sprintf("%d %5.2f",f->getBasenote(), f->getFreq());
    painter.drawText(0,0,width(),height(),Qt::AlignTop|Qt::AlignLeft,cap);
    painter.drawText(0,0,width(),height(),Qt::AlignTop|Qt::AlignRight,"*");
    painter.drawText(0,0,width(),height(),Qt::AlignBottom|Qt::AlignRight,"*");
}

void MWBaseNoteSetter::resizeEvent(QResizeEvent *E)
{
    //qDebug() << "MWBaseNoteSetter::resizeEvent" << width();
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


#include "mwbasenotesetter.h"
#include "comm/senderdebug.h"
#include <QPainter>
#include <QDebug>

MWBaseNoteSetter::MWBaseNoteSetter(Pitch * pitch, QWidget *parent) : MisuWidget(parent)
{
    //qDebug() << "MWBaseNoteSetter::MWBaseNoteSetter " << pitch->pitch << " pitch basenote " << pitch->basenote;
    out=new SenderDebug();
    p=pitch;
    f=new FreqTriple(p);
    f->setOct(4);
    //qDebug() << "f: " << f->getFreq() << " " << f->getPitch() << " " << f->getHue();
    vId=0;
    pressed=0;
    chan=1;
    selected=false;
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
        //qDebug() << "MWBaseNoteSetter::processTouchEvent TouchPointPressed " << out << " vId:" << vId;
        emit setBaseNote(p);
        //qDebug() << "MWBaseNoteSetter::processTouchEvent emit setBaseNote " << f->getBasenote();
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
    painter.setFont(font1);
    int l=lOff;
    int s=sOff;
    if(pressed>0 || selected) {
        l=lOn;
        s=sOff;
    }
    if(bwmode) {
        if(selected) {
            painter.setBrush(highlightcolor);
        } else if(f->getBW()) {
            painter.setBrush(wkeycolor);
        } else {
            painter.setBrush(bkeycolor);
        }
    } else {
        painter.setBrush(QColor::fromHsl(f->getHue(),s,l));
    }
    painter.setPen(fgcolor);
    painter.drawRect(0,0,width(),height());
    //cap.sprintf("%d %5.2f",f->getBasenote(), f->getFreq());
    cap.sprintf("%s",f->getBasenoteString().toStdString().c_str());
    painter.drawText(0,0,width(),height(),Qt::AlignVCenter|Qt::AlignCenter,cap);
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

void MWBaseNoteSetter::pitchChange()
{
    qDebug() << "MWBaseNoteSetter::pitchChange "  << f->getPitch();
    f->pitchChange();
    update();
}

void MWBaseNoteSetter::onSetBaseNote(Pitch * pitch)
{
    if(pitch == p) {
        selected = true;
    } else {
        selected = false;
    }
    update();
}

void MWBaseNoteSetter::setOut(ISender *value)
{
    out = value;
    //qDebug() << "MWBaseNoteSetter::setOut:" << out;
}


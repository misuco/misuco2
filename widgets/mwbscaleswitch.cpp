#include "mwbscaleswitch.h"
#include <QPainter>
#include "comm/senderdebug.h"
#include <QDebug>

MWBScaleSwitch::MWBScaleSwitch(int i, Pitch **MWP)
{
    qDebug() << "MWBScaleSwitch::MWBScaleSwitch " << sizeof(Pitch);

    for(int i=0;i<12;i++) {
        MWPitch[i]=*(MWP+i);
    }
    // TODO: how to type cast this?
    //MWPitch=MWP;
    f=new FreqTriple(MWP[i],this);
    value=false;
    pressed=0;
    chan=0;
    bscaleId=i;
    setBaseNote(MWP[0]);
    out=new SenderDebug();
}

MWBScaleSwitch::~MWBScaleSwitch()
{
    delete(f);
}

void MWBScaleSwitch::processTouchEvent(misuTouchEvent e)
{
    switch(e.state) {
    case Qt::TouchPointPressed:
        vId=out->noteOn(chan,f->getFreq(),f->getMidinote(),f->getPitch(),127);
        value=!value;
        emit setBscale(bscaleId,value);
        pressed++;
        update();
        break;
    case Qt::TouchPointReleased:
        out->noteOff(vId);
        pressed--;
        update();
        break;
    }
}

void MWBScaleSwitch::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QString cap;
    int l=127;
    if(value) l=200;
    painter.setBrush(QColor::fromHsl(f->getHue(),127,l));
    painter.drawRect(0,0,width(),height());
    cap.sprintf("%d",bscaleId);
    painter.drawText(0,0,width(),height(),Qt::AlignCenter|Qt::AlignHCenter,cap);
}

void MWBScaleSwitch::setOut(ISender *value)
{
    out = value;
}

void MWBScaleSwitch::setBaseNote(Pitch *p)
{
    basenote=p->basenote;
    int newBaseNote=(basenote+bscaleId)%12;
    f->setBasenote(MWPitch[newBaseNote]);
    qDebug() << "MWBScaleSwitch::setBaseNote " << newBaseNote << " bscaleId " << bscaleId << " basenote " << basenote;
    //f->setBasenote((p+bscaleId)%11);
    update();
}

void MWBScaleSwitch::setOctMid(int o)
{
    f->setOct(o);
}

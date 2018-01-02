#include "mwfaderpitch.h"
#include <QDebug>

MWFaderPitch::MWFaderPitch(QWidget *parent, Pitch *p, int c) : MWFadder(parent, p), chan(c)
{
    f = new FreqTriple(p);
    f->setOct(4);
}

MWFaderPitch::~MWFaderPitch()
{
    f->deleteLater();
}

void MWFaderPitch::setOut(ISender *value)
{
    out = value;
}

void MWFaderPitch::setOctMid(int o)
{
    f->setOct(o);
}

void MWFaderPitch::setChan(int c)
{
    chan=c;
}

void MWFaderPitch::pitchChange()
{
    f->pitchChange();
    update();
}

void MWFaderPitch::processTouchEvent(misuTouchEvent e)
{
    switch(e.state) {
    case Qt::TouchPointPressed:
        vId=out->noteOn(chan,f->getFreq(),f->getMidinote(),f->getPitch(),127);
        qDebug() << "MWFaderPitch::processTouchEvent TouchPointPressed " << out << " vId:" << vId;
        pressed++;
        update();
        break;
    case Qt::TouchPointMoved:
        out->pitch(chan,vId,f->getFreq(),f->getMidinote(),f->getPitch());
        update();
        break;
    case Qt::TouchPointReleased:
        qDebug() << "MWFaderPitch::processTouchEvent TouchPointReleased vId:" << vId;
        out->noteOff(vId);
        pressed--;
        update();
        break;
    }
    MWFadder::processTouchEvent(e);
}

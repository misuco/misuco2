#include "mwfaderpitch.h"
#include <QDebug>

MWFaderPitch::MWFaderPitch(QWidget *parent, Pitch *p) : MWFadder(parent, p)
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

void MWFaderPitch::pitchChange()
{
    f->pitchChange();
    update();
}

void MWFaderPitch::processTouchEvent(misuTouchEvent e)
{
    switch(e.state) {
    case Qt::TouchPointPressed:
        vId=out->noteOn(channel,f->getFreq(),f->getMidinote(),f->getPitch(),127);
        //qDebug() << "MWFaderPitch::processTouchEvent TouchPointPressed " << out << " vId:" << vId;
        pressed++;
        update();
        break;
    case Qt::TouchPointMoved:
        out->pitch(channel,vId,f->getFreq(),f->getMidinote(),f->getPitch());
        Microtune.tuning[f->getBasenote()] = getValue();
        update();
        break;
    case Qt::TouchPointReleased:
        //qDebug() << "MWFaderPitch::processTouchEvent TouchPointReleased vId:" << vId;
        out->noteOff(vId);
        pressed--;
        update();
        break;
    }
    MWFadder::processTouchEvent(e);
}

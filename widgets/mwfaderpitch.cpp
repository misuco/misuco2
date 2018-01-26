/*
 * Copyright (c) 2018 by misuco.org, Claudio Zopfi, Zurich, Switzerland, c@misuco.org
 *
 * This file is part of MISUCO2.
 *
 * MISUCO2 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * MISUCO2 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with MISUCO2.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "mwfaderpitch.h"
#include <QDebug>

MWFaderPitch::MWFaderPitch(QObject *parent, Pitch *p) : MWFadder(parent)
{
    _freq = new FreqTriple(p);
    _freq->setOct(4);
    calcColor();
}

MWFaderPitch::~MWFaderPitch()
{
    _freq->deleteLater();
}

void MWFaderPitch::setOut(ISender *value)
{
    out = value;
}

void MWFaderPitch::onPressedPitch(int id)
{
    //qDebug() << "MWFaderPitch::onPressedPitch " << id << " pressed " << pressed << " eventId " << eventId;
    if(pressed < 2) {
        eventId=id;
        vId=out->noteOn(channel,_freq->getFreq(),_freq->getMidinote(),_freq->getPitch(),127);
        calcColor();
    }
}

void MWFaderPitch::onUpdatedPitch(int id)
{
    //qDebug() << "MWFaderPitch::onUpdatedPitch " << id << " pressed " << pressed << " eventId " << eventId;
    if(id == eventId) {
        Microtune.tuning[_freq->getrootNote()] = getValue();
        out->pitch(channel,vId,_freq->getFreq(),_freq->getMidinote(),_freq->getPitch());
    }
}

void MWFaderPitch::onReleasedPitch(int id)
{
    //qDebug() << "MWFaderPitch::onReleasedPitch " << id << " pressed " << pressed << " eventId " << eventId;
    if(id == eventId) {
        out->noteOff(vId);
        calcColor();
    }
}

void MWFaderPitch::setOctMid(int o)
{
    _freq->setOct(o);
}

void MWFaderPitch::pitchChange()
{
    _freq->pitchChange();
    calcColor();
}

void MWFaderPitch::calcColor()
{
    float l=lOff;
    float s=sOff;
    if(pressed>0) {
        l=lOn;
        s=sOn;
    }

    int pitch = _freq->getrootNote();
    if(bwmode) {
        if(pressed>0) {
            if(MWPitch[pitch]->getBW()) {
                _pitchColor = hlwkeycolor;
            } else {
                _pitchColor = hlbkeycolor;
            }
        } else if(MWPitch[pitch]->getBW()) {
            _pitchColor = wkeycolor;
        } else {
            _pitchColor = bkeycolor;
        }
    } else {
        _pitchColor = QColor::fromHslF(MWPitch[pitch]->getHue(),s,l);
    }
    emit colorChanged();
}

void MWFaderPitch::processTouchEvent(misuTouchEvent e)
{
    switch(e.state) {
    case Qt::TouchPointPressed:
        //qDebug() << "MWFaderPitch::processTouchEvent TouchPointPressed " << out << " vId:" << vId;
        //update();
        break;
    case Qt::TouchPointMoved:
        //update();
        break;
    case Qt::TouchPointReleased:
        //qDebug() << "MWFaderPitch::processTouchEvent TouchPointReleased vId:" << vId;
        //update();
        break;
    }
    //MWFadder::processTouchEvent(e);
}

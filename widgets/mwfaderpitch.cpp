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
}

MWFaderPitch::~MWFaderPitch()
{
    _freq->deleteLater();
}

void MWFaderPitch::setOut(ISender *value)
{
    out = value;
}

int MWFaderPitch::pitchId()
{
    return _freq->getRootNote();
}

bool MWFaderPitch::selected()
{
    return pressed>0;
}

void MWFaderPitch::onPressedPitch(int id)
{
    //qDebug() << "MWFaderPitch::onPressedPitch " << id << " pressed " << pressed << " eventId " << eventId;
    if(pressed < 2) {
        eventId=id;
        vId=out->noteOn(MGlob::channel,_freq->getFreq(),_freq->getMidinote(),_freq->getPitch(),127);
        emit selectedChanged();
    }
}

void MWFaderPitch::onUpdatedPitch(int id)
{
    //qDebug() << "MWFaderPitch::onUpdatedPitch " << id << " pressed " << pressed << " eventId " << eventId;
    if(id == eventId) {
        MGlob::Microtune.tuning[_freq->getRootNote()] = getValue();
        out->pitch(MGlob::channel,vId,_freq->getFreq(),_freq->getMidinote(),_freq->getPitch());
    }
}

void MWFaderPitch::onReleasedPitch(int id)
{
    //qDebug() << "MWFaderPitch::onReleasedPitch " << id << " pressed " << pressed << " eventId " << eventId;
    if(id == eventId) {
        out->noteOff(vId);
        emit selectedChanged();
    }
}

void MWFaderPitch::setOctMid(int o)
{
    _freq->setOct(o);
}

void MWFaderPitch::pitchChange()
{
    _freq->pitchChange();
}


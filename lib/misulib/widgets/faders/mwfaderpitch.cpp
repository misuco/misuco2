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

MWFaderPitch::MWFaderPitch(int rootNote, MasterSender *ms, QObject *parent) : MWFader(parent),
    _out(ms)
{
    _rootNote = rootNote;
    _freq = new FreqTriple(rootNote,this);
    _freq->setOct(4);
}

MWFaderPitch::~MWFaderPitch()
{
    _freq->deleteLater();
}

bool MWFaderPitch::selected()
{
    return pressed>0;
}

void MWFaderPitch::onPressed(int id, int x, int y, int h, int w)
{
    //qDebug() << "MWFaderPitch::onPressedPitch " << id << " pressed " << pressed << " eventId " << eventId;
    MWFader::onPressed(id,x,y,h,w);
    if(pressed < 2) {
        eventId=id;
        vId=_out->noteOn(_freq->getFreq(),_freq->getMidinote(),_freq->getPitch(),127);
        emit selectedChanged();
    }
}

void MWFaderPitch::onUpdated(int id, int y, int h, int w)
{
    //qDebug() << "MWFaderPitch::onUpdatedPitch " << id << " pressed " << pressed << " eventId " << eventId;
    MWFader::onUpdated(id,y,h,w);
    if(id == eventId) {
        _out->pitch(vId,_freq->getFreq(),_freq->getMidinote(),_freq->getPitch());
    }
}

void MWFaderPitch::onReleased(int id)
{
    //qDebug() << "MWFaderPitch::onReleasedPitch " << id << " pressed " << pressed << " eventId " << eventId;
    MWFader::onReleased(id);
    if(id == eventId) {
        _out->noteOff(vId);
        emit selectedChanged();
    }
}

void MWFaderPitch::setOctMid(int o)
{
    _freq->setOct(o);
}

void MWFaderPitch::onPitchChange(int rootNote, int value)
{
    _freq->onPitchChange(rootNote, value);
}

void MWFaderPitch::valueChange()
{
    _freq->onPitchChange(_rootNote, _valueDisplay);
    emit pitchChange(_freq->getRootNote(),_valueDisplay);
}


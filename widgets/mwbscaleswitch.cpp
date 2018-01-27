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

#include "mwbscaleswitch.h"
#include <QPainter>
#include "comm/senderdebug.h"
#include <QDebug>

MWBScaleSwitch::MWBScaleSwitch(int i)
{
    //qDebug() << "MWBScaleSwitch::MWBScaleSwitch " << sizeof(Pitch);
    _freq=new FreqTriple(MWPitch[i],this);
    _value=false;
    _pressed=0;
    _bscaleId=i;
    _freq->setOct(4);
    _out=nullptr;
    _vid=0;
}

MWBScaleSwitch::~MWBScaleSwitch()
{
    _freq->deleteLater();
}

void MWBScaleSwitch::calcText()
{
    _text1=_freq->getrootNoteString(noteSymbols);

    if(showFreqs) {
        _text2.sprintf("%5.1f",_freq->getFreq());
    } else {
        _text2="";
    }

    emit textChanged();
}

void MWBScaleSwitch::setOut(ISender *value)
{
    _out = value;
}

int MWBScaleSwitch::pitchId()
{
    //qDebug() << "MWBScaleSwitch::pitchId " << _freq->getPitch() << " bscaleId" << bscaleId;
    return _freq->getrootNote();
}

bool MWBScaleSwitch::selected()
{
    return (_pressed>0 || _value);
}

void MWBScaleSwitch::onPressed()
{
    if(_out) _vid=_out->noteOn(channel,_freq->getFreq(),_freq->getMidinote(),_freq->getPitch(),127);
    _value=!_value;
    emit setBscale(_bscaleId,_value);
    _pressed++;
    emit selectedChanged();
}

void MWBScaleSwitch::onReleased()
{
    if(_out) _out->noteOff(_vid);
    _pressed--;
    emit selectedChanged();
}

void MWBScaleSwitch::setrootNote(Pitch *p)
{
    setrootNote(p->getrootNote());
}

void MWBScaleSwitch::setrootNote(int rootNote)
{
    //qDebug() << "MWBScaleSwitch::setrootNote " << newrootNote << " bscaleId " << bscaleId << " rootNote " << rootNote;
    int newrootNote=(rootNote+_bscaleId)%12;
    if(newrootNote != _freq->getPitch()) {
        _freq->setrootNote(MWPitch[newrootNote]);
        emit pitchIdChanged();
        calcText();
    }
}

void MWBScaleSwitch::setOctMid(int o)
{
    _freq->setOct(o);
}

void MWBScaleSwitch::onScaleSet(MWScale * scale)
{
    //qDebug() << "MWBScaleSwitch::onScaleSet" << bscaleId;
    if(scale->bscale[_bscaleId-1]) {
        if(!_value) {
            _value=true;
            emit selectedChanged();
        }
    } else {
        if(_value) {
            _value=false;
            emit selectedChanged();
        }
    }
    setrootNote(scale->rootNote);
}

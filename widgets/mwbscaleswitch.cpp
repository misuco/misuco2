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
#include "comm/senderdebug.h"
#include <QDebug>

MWBScaleSwitch::MWBScaleSwitch(int i, MasterSender *ms, QObject * parent) : QObject(parent),
    _out(ms)
{
    //qDebug() << "MWBScaleSwitch::MWBScaleSwitch " << sizeof(Pitch);
    _freq=new FreqTriple(MGlob::MWPitch[i],this);
    _value=false;
    _pressed=0;
    _bscaleId=i;
    _freq->setOct(4);
    _vid=0;
    _oct=6;
    _higherOct=0;
}

MWBScaleSwitch::~MWBScaleSwitch()
{
    _freq->deleteLater();
}

void MWBScaleSwitch::calcText()
{
    _text1=_freq->getRootNoteString(MGlob::noteSymbols);

    if(MGlob::showFreqs) {
        _text2.sprintf("%5.1f",_freq->getFreq());
    } else {
        _text2="";
    }

    emit textChanged();
}

int MWBScaleSwitch::pitchId()
{
    //qDebug() << "MWBScaleSwitch::pitchId " << _freq->getPitch() << " bscaleId" << bscaleId;
    return _freq->getRootNote();
}

bool MWBScaleSwitch::selected()
{
    return (_pressed>0 || _value);
}

void MWBScaleSwitch::onPressed()
{
    if(_out && _pressed == 0) _vid=_out->noteOn(_freq->getFreq(),_freq->getMidinote(),_freq->getPitch(),127);
    _pressed++;
    emit selectedChanged();
}

void MWBScaleSwitch::onReleased()
{
    if(_out && _pressed == 1)  {
        _out->noteOff(_vid);
        _value=!_value;
        emit setBscale(_bscaleId,_value);
    }
    _pressed--;
    emit selectedChanged();
}

void MWBScaleSwitch::onSetRootNote(int rootNote)
{
    int newrootNote=rootNote+_bscaleId;
    if(newrootNote>11) {
        newrootNote-=12;
        _higherOct=1;
    } else {
        _higherOct=0;
    }

    //if(newrootNote != _freq->getPitch()) {
        _freq->setOct(_oct+_higherOct);
        _freq->setRootNote(MGlob::MWPitch[newrootNote]);
        emit pitchIdChanged();
        calcText();
    //}

    //qDebug() << "MWBScaleSwitch::setRootNote new: " << newrootNote << " bscaleId " << _bscaleId << " rootNote " << rootNote;
}

void MWBScaleSwitch::setOctMid(int o)
{
    _oct = o;
    _freq->setOct(_oct + _higherOct);
}

void MWBScaleSwitch::onSetScale(int rootNote, QList<bool> scale)
{
    //qDebug() << "MWBScaleSwitch::onSetScale" << bscaleId;
    if(scale[_bscaleId-1]) {
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
    onSetRootNote(rootNote);
}

void MWBScaleSwitch::onSymbolsChanged()
{
    calcText();
}

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

#include "scaleswitch.h"
#include "lib/misulib/comm/senderdebug.h"
#include "lib/misulib/models/types.h"
#include <QDebug>

ScaleSwitch::ScaleSwitch(int scaleId, MasterSender *ms, QObject * parent) : QObject(parent),
    _out(ms),
    _rootNote(0),
    _value(false),
    _noteSymbols(0),
    _showFreqs(false)
{
    //qDebug() << "MWScaleSwitch::MWScaleSwitch " << sizeof(Pitch);
    _freq=new FreqTriple(scaleId,this);
    _value=false;
    _pressed=0;
    _scaleId=scaleId;
    _freq->setOct(4);
    _vid=0;
    _oct=6;
    _higherOct=0;
}

ScaleSwitch::~ScaleSwitch()
{
    _freq->deleteLater();
}

void ScaleSwitch::calcText()
{
    _text1=_freq->getRootNoteString(_noteSymbols);

    if(_showFreqs) {
        _text2.sprintf("%5.1f",_freq->getFreq());
    } else {
        _text2="";
    }

    emit textChanged();
}

int ScaleSwitch::pitchId()
{
    //qDebug() << "MWScaleSwitch::pitchId " << _freq->getPitch() << " scaleId" << scaleId;
    return _freq->getRootNote();
}

bool ScaleSwitch::selected()
{
    return (_pressed>0 || _value);
}

void ScaleSwitch::onPressed()
{
    if(_out && _pressed == 0) _vid=_out->noteOn(_freq->getFreq(),_freq->getMidinote(),_freq->getPitch(),127);
    _pressed++;
    emit selectedChanged();
}

void ScaleSwitch::onReleased()
{
    if(_out && _pressed == 1)  {
        _out->noteOff(_vid);
        _value=!_value;
        emit setScale(_scaleId,_value);
    }
    _pressed--;
    emit selectedChanged();
}

void ScaleSwitch::onSetRootNote(int rootNote)
{
    _rootNote=rootNote+_scaleId;
    if(_rootNote>SCALE_SIZE) {
        _rootNote-=12;
        _higherOct=1;
    } else {
        _higherOct=0;
    }

    _freq->setOct(_oct+_higherOct);
    _freq->setRootNote(_rootNote);
    emit rootNoteChanged();
    calcText();
    //qDebug() << "MWScaleSwitch::setRootNote new: " << newrootNote << " scaleId " << _scaleId << " rootNote " << rootNote;
}

void ScaleSwitch::onPitchChange(int rootNote, int pitch)
{
    _freq->onPitchChange(rootNote,pitch);
}

void ScaleSwitch::setOctMid(int o)
{
    _oct = o;
    _freq->setOct(_oct + _higherOct);
}

void ScaleSwitch::onSetScale(int rootNote, QList<bool> scale)
{
    //qDebug() << "MWScaleSwitch::onSetScale" << scaleId;
    if(scale[_scaleId-1]) {
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

void ScaleSwitch::onSymbolsChange(int s)
{
    _noteSymbols = s;
    calcText();
}

void ScaleSwitch::onShowFreqsChange(bool state)
{
    _showFreqs = state;
    calcText();
}

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

#include "mwfaderparamctl.h"
#include <QDebug>

MWFaderParamCtl::MWFaderParamCtl(int cc, MasterSender *ms, QObject *parent) : MWFader(parent),
    _out(ms),
    _cc(cc)
{
    switch(cc) {
    case 1:
        _text2="top";
        break;
    case 2:
        _text2="bot";
        break;
    case 3:
        _text2="channel";
        break;
    case 4:
        _text2="symbols";
        break;
    case 102:
        _text2="WAV";
        break;
    case 103:
        _text2="ATT";
        break;
    case 104:
        _text2="DEC";
        break;
    case 105:
        _text2="SUST";
        break;
    case 106:
        _text2="REL";
        break;
    case 107:
        _text2="FLTF";
        break;
    case 108:
        _text2="FLTR";
        break;
    case 109:
        _text2="MODF";
        break;
    case 110:
        _text2="MODR";
        break;
    case 111:
        _text2="VOL";
        break;
    }
}

MWFaderParamCtl::~MWFaderParamCtl()
{
}

void MWFaderParamCtl::setValue(int v)
{
    MWFader::setValue(v);
    propagateValueChange();
    emit geoChanged();
}

void MWFaderParamCtl::propagateValueChange() {

    switch(_cc) {
    case 3:
        MGlob::channel=getValue();
        break;
    case 102:
        MGlob::Sound.wave_type=getValue();
        break;
    case 103:
        MGlob::Sound.attack=getValue();
        break;
    case 104:
        MGlob::Sound.decay=getValue();
        break;
    case 105:
        MGlob::Sound.sustain=getValue();
        break;
    case 106:
        MGlob::Sound.release=getValue();
        break;
    case 107:
        MGlob::Sound.filter_cutoff=getValue();
        break;
    case 108:
        MGlob::Sound.filter_resonance=getValue();
        break;
    case 109:
        MGlob::Sound.mod_filter_cutoff=getValue();
        break;
    case 110:
        MGlob::Sound.mod_filter_resonance=getValue();
        break;
    case 111:
        MGlob::Sound.volume=getValue();
        break;
    }

    _out->cc(0,_cc,getValue(),getValue());
}

void MWFaderParamCtl::onPressedPitch(int id)
{
    //qDebug() << "MWFaderPitch::onPressedPitch " << id << " pressed " << pressed << " eventId " << eventId;
    if(_pressed < 2) {
        _eventId=id;
        propagateValueChange();
    }
}

void MWFaderParamCtl::onUpdatedPitch(int id)
{
    //qDebug() << "MWFaderPitch::onUpdatedPitch " << id << " pressed " << pressed << " eventId " << eventId;
    if(id == _eventId) {
        _out->cc(0,_cc,getValue(),getValue());
        propagateValueChange();
    }
}

void MWFaderParamCtl::onReleasedPitch(int)
{
}

void MWFaderParamCtl::valueChange()
{
    emit controlValueChange(_valueDisplay);
}

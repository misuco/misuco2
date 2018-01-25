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

    //for(int i=0;i<12;i++) {
        _freq=new FreqTriple(MWPitch[i],this);
        _freq->setBasenote(MWPitch[i]);
    //}
    _value=false;
    _pressed=0;
    bscaleId=i;
    _freq->setOct(4);
    out=nullptr;
    vId=0;
}

MWBScaleSwitch::~MWBScaleSwitch()
{
    delete(_freq);
}

void MWBScaleSwitch::calcColor()
{
    int l=lOff;
    int s=sOff;
    if(_pressed>0 || _value) {
        l=lOn;
        s=sOn;
    }

    int pitch = _freq->getBasenote();
    if(bwmode) {
        if(_value) {
            if(MWPitch[pitch]->getBW()) {
                _color = hlwkeycolor;
            } else {
                _color = hlbkeycolor;
            }
        } else if(MWPitch[pitch]->getBW()) {
            _color = wkeycolor;
        } else {
            _color = bkeycolor;
        }
    } else {
        _color.fromHsl(MWPitch[pitch]->getHue(),s,l);
    }


    if(_pressed>0 || _value) {
        _fontColor=highlightcolor;
    } else {
        _fontColor=fgcolor;
    }

    _text1=_freq->getBasenoteString(noteSymbols);

    if(showFreqs) {
        _text2.sprintf("%5.1f",_freq->getFreq());
    } else {
        _text2="";
    }

    emit colorChanged();
}

void MWBScaleSwitch::setOut(ISender *value)
{
    out = value;
}

void MWBScaleSwitch::onPressed()
{
    if(out) vId=out->noteOn(channel,_freq->getFreq(),_freq->getMidinote(),_freq->getPitch(),127);
    _value=!_value;
    emit setBscale(bscaleId,_value);
    emit scaleupdate();
    _pressed++;
    calcColor();
}

void MWBScaleSwitch::onReleased()
{
    if(out) out->noteOff(vId);
    _pressed--;
    calcColor();
}

void MWBScaleSwitch::setBaseNote(Pitch *p)
{
    basenote=p->getBasenote();
    int newBaseNote=(basenote+bscaleId)%12;
    //qDebug() << "MWBScaleSwitch::setBaseNote " << newBaseNote << " bscaleId " << bscaleId << " basenote " << basenote;
    _freq->setBasenote(MWPitch[newBaseNote]);
    calcColor();
}

void MWBScaleSwitch::setOctMid(int o)
{
    _freq->setOct(o);
}

void MWBScaleSwitch::onScaleSet(MWScale * scale)
{
    if(scale->bscale[bscaleId-1]) {
        _value=true;
    } else {
        _value=false;
    }

    basenote = (scale->basenote+bscaleId)%12;
    _freq->setBasenote(MWPitch[basenote]);

    calcColor();
}

void MWBScaleSwitch::onscaleupdate()
{
    calcColor();
}

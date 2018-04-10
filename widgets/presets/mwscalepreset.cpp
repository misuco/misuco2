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

#include "mwscalepreset.h"
#include "presetcollection.h"
#include <QDebug>

MWScalePreset::MWScalePreset(QObject *parent): QObject(parent)
{
    PresetScale.rootNote=qrand() % (BSCALE_SIZE+1);
    PresetScale.baseoct=4;
    PresetScale.topoct=5;
    scaleSize=2;
    for(int i=0;i<BSCALE_SIZE;i++) {
        if(qrand() % 2) {
            PresetScale.bscale[i]=true;
            scaleSize+=1;
        } else {
            PresetScale.bscale[i]=false;
        }
    }
    pressed=0;
}

MWScalePreset::MWScalePreset(int rootNote, bool bscale[], QObject *parent): QObject(parent)
{
    PresetScale.rootNote=rootNote;
    scaleSize=2;
    for(int i=0;i<BSCALE_SIZE;i++) {
        if(bscale[i]) {
            PresetScale.bscale[i]=true;
            scaleSize+=1;
        } else {
            PresetScale.bscale[i]=false;
        }
    }
    pressed=0;
}

QStringList MWScalePreset::bscale()
{
    QStringList l;
    l.append("1");
    for(int i=0;i<BSCALE_SIZE;i++) {
        if(PresetScale.bscale[i]) l.append("1");
        else l.append("0");
    }
    return l;
}

int MWScalePreset::rootnote()
{
    return PresetScale.rootNote;
}

void MWScalePreset::overwrite()
{
    PresetScale.rootNote = MGlob::Scale.rootNote;
    scaleSize = 2;

    for(int i=0;i<BSCALE_SIZE;i++) {
        PresetScale.bscale[i]=MGlob::Scale.bscale[i];
        if(PresetScale.bscale[i]) {
            scaleSize+=1;
        }
    }

    emit setScale(&PresetScale);
}

void MWScalePreset::onPressed()
{
    pressed++;
    canceled = false;
}

void MWScalePreset::onPressAndHold()
{
    canceled = true;
    PresetCollection::dialogContext = this;
    emit editPreset();
}

void MWScalePreset::onCanceled()
{
    canceled = true;
}

void MWScalePreset::onReleased()
{
    if(!canceled) {
        emit setScale(&PresetScale);
    }
    pressed--;
}

void MWScalePreset::initialSet()
{
    emit setScale(&PresetScale);
}

void MWScalePreset::playAreaChanged()
{
    emit presetChanged();
}

bool MWScalePreset::isSelected()
{
    if(PresetScale.rootNote!=MGlob::Scale.rootNote ) return false;

    for(int i=0;i<BSCALE_SIZE;i++) {
        if( PresetScale.bscale[i]!=MGlob::Scale.bscale[i] ) return false;
    }
    return true;
}

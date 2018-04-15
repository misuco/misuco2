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
    _presetRootNote=0;
    _scaleSize=2;
    for(int i=0;i<SCALE_SIZE;i++) {
        _presetScale.append(false);
        _currentScale.append(false);
    }
    _pressed=0;
}

MWScalePreset::MWScalePreset(int rootNote, QList<bool> scale, QObject *parent): QObject(parent)
{
    _presetRootNote=rootNote;
    _presetScale = scale;
    _scaleSize = 2 + _presetScale.count(true);
    for(int i=0;i<SCALE_SIZE;i++) {
        _currentScale.append(false);
    }
    _pressed=0;
}

QStringList MWScalePreset::scale()
{
    QStringList l;
    l.append("1");
    for(int i=0;i<SCALE_SIZE;i++) {
        if(_presetScale[i]) l.append("1");
        else l.append("0");
    }
    return l;
}

int MWScalePreset::getRootNote()
{
    return _presetRootNote;
}

bool MWScalePreset::getScale(int i)
{
    return _presetScale[i];
}

void MWScalePreset::overwrite()
{
    _presetRootNote = _currentRootNote;
    _presetScale = _currentScale;
    _scaleSize = 2 + _presetScale.count(true);
    emit presetChanged();
}

void MWScalePreset::onPressed()
{
    _pressed++;
    _canceled = false;
}

void MWScalePreset::onPressAndHold()
{
    _canceled = true;
    PresetCollection::dialogContext = this;
    emit editPreset();
}

void MWScalePreset::onCanceled()
{
    _canceled = true;
}

void MWScalePreset::onReleased()
{
    if(!_canceled) {
        emit setScale(_presetRootNote,_presetScale);
    }
    _pressed--;
}

void MWScalePreset::onSetRootNote(int p)
{
    _currentRootNote = p;
    emit selectedChanged();
}

void MWScalePreset::onSetScale(int n, bool v)
{
    _currentScale[n-1] = v;
    emit selectedChanged();
}

void MWScalePreset::onSetScale(int rootNote, QList<bool> scale)
{
    _currentScale=scale;
    _currentRootNote=rootNote;
    emit selectedChanged();
}

void MWScalePreset::initialSet()
{
    emit setScale(_presetRootNote,_presetScale);
}

bool MWScalePreset::isSelected()
{
    if(_presetRootNote!=_currentRootNote ) return false;

    if(_presetScale.size()<SCALE_SIZE) return false;
    if(_currentScale.size()<SCALE_SIZE) return false;

    for(int i=0;i<SCALE_SIZE;i++) {
        if( _presetScale[i] != _currentScale[i] ) return false;
    }
    return true;
}

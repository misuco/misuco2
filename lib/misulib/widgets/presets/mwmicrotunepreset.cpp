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

#include "mwmicrotunepreset.h"
#include <QDebug>

MWMicrotunePreset::MWMicrotunePreset(QList<QObject *> tuningFaders, QObject *parent): QObject(parent)
{
    for(auto tuningFader:tuningFaders) _tuningFaders.append(qobject_cast<MWFaderPitch*>(tuningFader));
    for(int i=0;i<tuningFaders.size();i++) {
        PresetMicrotune.tuning[i]=rand()%200-100;
    }
}

MWMicrotunePreset::MWMicrotunePreset(int tuning[], QList<QObject *> tuningFaders, QObject *parent): QObject(parent)
{
    for(auto tuningFader:tuningFaders) _tuningFaders.append(qobject_cast<MWFaderPitch*>(tuningFader));
    for(int i=0;i<tuningFaders.size();i++) {
        PresetMicrotune.tuning[i]=tuning[i];
    }
}

void MWMicrotunePreset::processTouchEvent(misuTouchEvent e)
{
    switch(e.state) {
    case Qt::TouchPointPressed:
        if(1/*overwrite*/) {
            int i=0;
            for(auto tuningFader:_tuningFaders) {
                PresetMicrotune.tuning[i]=tuningFader->getValue();
                i++;
            }
        }
        else {
            emit setMicrotune(&PresetMicrotune);
        }
        pressed++;
        break;
    case Qt::TouchPointReleased:
        pressed--;
        break;
    }
}

void MWMicrotunePreset::initialSet()
{
    emit setMicrotune(&PresetMicrotune);
}

bool MWMicrotunePreset::isSelected() {
    int i=0;
    for(auto tuningFader:_tuningFaders) {
        if(PresetMicrotune.tuning[i]!=tuningFader->getValue()) return false;
        i++;
    }
    return true;
}


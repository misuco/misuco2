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

MWMicrotunePreset::MWMicrotunePreset(QObject *parent): MisuWidget(parent)
{
    for(int i=0;i<12;i++) {
        PresetMicrotune.tuning[i]=rand()%200-100;
    }
}

MWMicrotunePreset::MWMicrotunePreset(int tuning[], QObject *parent): MisuWidget(parent)
{
    for(int i=0;i<12;i++) {
        PresetMicrotune.tuning[i]=tuning[i];
    }
}

void MWMicrotunePreset::processTouchEvent(misuTouchEvent e)
{
    switch(e.state) {
    case Qt::TouchPointPressed:
        if(1/*overwrite*/) {
            for(int i=0;i<12;i++) {
                PresetMicrotune.tuning[i]=Microtune.tuning[i];
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
    //update();
}

/*
void MWMicrotunePreset::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    int xr=width()/14;
    int border=xr;
    int h=height()-2*border;

    if(isSelected()) {
        painter.setBrush(highlightcolor);
        painter.setPen(highlightcolor);
    } else {
        painter.setBrush(bgcolor);
        painter.setPen(fgcolor);
    }

    for(int i=0;i<12;i++) {
        int y=h/2 + h*PresetMicrotune.tuning[i]/100;
        painter.drawRect(xr*(i+1),y,xr,xr);
    }

}

void MWMicrotunePreset::resizeEvent(QResizeEvent *)
{

}
*/

void MWMicrotunePreset::initialSet()
{
    emit setMicrotune(&PresetMicrotune);
}

bool MWMicrotunePreset::isSelected() {
    bool selected = true;
    for(int i=0;i<12;i++) {
        if(PresetMicrotune.tuning[i]!=Microtune.tuning[i]) selected = false;
    }
    return selected;
}

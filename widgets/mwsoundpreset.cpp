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

#include "mwsoundpreset.h"
#include <QPainter>
#include <QDebug>

MWSoundPreset::MWSoundPreset(QObject *parent): MisuWidget(parent)
{
    PresetSound.volume=800;
    PresetSound.wave_type=qrand() % 4;
    PresetSound.attack=qrand() % 100;
    PresetSound.decay=qrand() % 700;
    PresetSound.sustain=qrand() % 1000;
    PresetSound.release=qrand() % 1000;
    PresetSound.filter_cutoff=qrand() % 1000;
    PresetSound.filter_resonance=qrand() % 100;
    PresetSound.mod_filter_cutoff=50;
    PresetSound.mod_filter_resonance=0;

}

MWSoundPreset::MWSoundPreset(float vol, int wav, int att, int dec, float sus, int rel, float cut, float res, float modcut, float modres, QObject *parent): MisuWidget(parent)
{
    PresetSound.volume=vol;
    PresetSound.wave_type=wav;
    PresetSound.attack=att;
    PresetSound.decay=dec;
    PresetSound.sustain=sus;
    PresetSound.release=rel;
    PresetSound.filter_cutoff=cut;
    PresetSound.filter_resonance=res;
    PresetSound.mod_filter_cutoff=modcut;
    PresetSound.mod_filter_resonance=modres;
}

void MWSoundPreset::processTouchEvent(MisuWidget::misuTouchEvent e)
{
    switch(e.state) {
    case Qt::TouchPointPressed:
        if(overwrite) {
            PresetSound.volume=Sound.volume;
            PresetSound.wave_type=Sound.wave_type;
            PresetSound.attack=Sound.attack;
            PresetSound.decay=Sound.decay;
            PresetSound.sustain=Sound.sustain;
            PresetSound.release=Sound.release;
            PresetSound.filter_cutoff=Sound.filter_cutoff;
            PresetSound.filter_resonance=Sound.filter_resonance;
            PresetSound.mod_filter_cutoff=Sound.mod_filter_cutoff;
            PresetSound.mod_filter_resonance=Sound.mod_filter_resonance;
        }
        else {
            emit setSound(&PresetSound);
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
void MWSoundPreset::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    int xr=width()/8;
    int h=height()-2*xr;

    QFont font(font1);
    font.setPixelSize(fontheight);
    painter.setFont(font);

    painter.setPen(Qt::NoPen);
    painter.setBrush(bgcolor);

    QPen pen1 = QPen(fgcolor);
    QPen pen2 = QPen(highlightcolor);
    pen1.setWidth(2);
    pen2.setWidth(2);

    painter.drawRect(0,0,width(),height());
    if(isSelected()) {
        painter.setPen(pen2);
    } else {
        painter.setPen(pen1);
    }

    switch(PresetSound.wave_type) {
    case 0:
        painter.drawText(xr,xr,xr*3,height(),Qt::AlignLeft|Qt::AlignVCenter,"SQR");
        break;
    case 1:
        painter.drawText(xr,xr,xr*3,height(),Qt::AlignLeft|Qt::AlignVCenter,"SAW");
        break;
    case 2:
        painter.drawText(xr,xr,xr*3,height(),Qt::AlignLeft|Qt::AlignVCenter,"SIN");
        break;
    case 3:
        painter.drawText(xr,xr,xr*3,height(),Qt::AlignLeft|Qt::AlignVCenter,"TRI");
        break;
    default:
        painter.drawText(xr,xr,xr*3,height(),Qt::AlignLeft|Qt::AlignVCenter,"RND");
        break;
    }

    int xenv = xr*4;
    int xdiff=xr*PresetSound.attack/1000;
    painter.drawLine(xenv,xr+h,xenv+xdiff,xr);

    xenv+=xdiff;
    xdiff=xr*PresetSound.decay/1000;
    int ydiff=h-PresetSound.sustain*h/1000;
    painter.drawLine(xenv,xr,xenv+xdiff,xr+ydiff);

    xenv+=xdiff;
    painter.drawLine(xenv,xr+ydiff,xenv+xr,xr+ydiff);

    xenv+=xr;
    xdiff=xr*PresetSound.release/1000;
    painter.drawLine(xenv,xr+ydiff,xenv+xdiff,xr+h);

}

void MWSoundPreset::resizeEvent(QResizeEvent *)
{
    fontheight=height()/8;
}
*/

void MWSoundPreset::initialSet()
{
    emit setSound(&PresetSound);
}

bool MWSoundPreset::isSelected() {
    if( PresetSound.volume==Sound.volume &&
        PresetSound.wave_type==Sound.wave_type &&
        PresetSound.attack==Sound.attack &&
        PresetSound.decay==Sound.decay &&
        PresetSound.sustain==Sound.sustain &&
        PresetSound.release==Sound.release &&
        PresetSound.filter_cutoff==Sound.filter_cutoff &&
        PresetSound.filter_resonance==Sound.filter_resonance &&
        PresetSound.mod_filter_cutoff==Sound.mod_filter_cutoff &&
        PresetSound.mod_filter_resonance==Sound.mod_filter_resonance
    ) return true;
    else return false;
}

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

#include <QDebug>
#include "pitch.h"

Pitch::Pitch(QObject *parent) : QObject(parent)
{
    basenote=0;
    color=0;
}

void Pitch::setBasenote(int value)
{
    basenote = value;
    calcColor();
    emit change();
}

int Pitch::getHue() const
{
    return color;
}

int Pitch::getBW() const
{
    bool white = true;
    int bn = basenote;
    if(bn == 1 || bn == 3 || bn == 6 || bn == 8 || bn == 10 ) white = false;
    return white;
}

void Pitch::calcColor()
{
    color = basenote * 30 + pitch * 15 / 100;
    if(color>360) color -=360;
    if(color<0) color +=360;

    //qDebug() << "Pitch::calcColor " << color << " from basenote " << basenote << " and pitch " << pitch;
}

void Pitch::setPitch(int value)
{
    pitch = value;
    calcColor();
    emit change();
}

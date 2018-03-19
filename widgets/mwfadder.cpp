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

#include "mwfadder.h"
#include <QDebug>

MWFadder::MWFadder(QObject *parent) : QObject(parent)
{
    //qDebug() << "MWFadder::MWFadder";
    orient=vertical;
    value=0;
    inverted=false;
    valueDisplay=value;
    pressed=0;
    setMinValue(-100);
    setMaxValue(100);
    fineness=5;
    height=500;
    knobSize=height/8;
    calcGeo();
}

void MWFadder::onPressed(int id, int x, int y, int h, int w)
{
    height = h;
    knobSize = w;
    if(pressed<2) {
        pressedTouchId = id;
        xTouchBegin=x;
        yTouchBegin=y;
        valTouchBegin=value;

        float yRelative1 = (float)(y-knobSize) / (float)(height-knobSize);
        float yRelative2 = (float)(y) / (float)(height-knobSize);
        bool fineModeAvailable = (float)valRange/(float)(height-knobSize) > 1.0/(float)fineness;
        if((_faderY >= yRelative1 && _faderY <= yRelative2) || !fineModeAvailable) {
            fadeMode=coarse;
            //qDebug() << "coarse fade";
        } else {
            fadeMode=fine;
            //qDebug() << "fine fade";
        }
        emit valueChange(valueDisplay);
    }
    pressed++;
}

void MWFadder::onUpdated(int id, int y, int h, int w)
{
    height = h;
    knobSize = w;
    if(id==pressedTouchId) {
        if(vertical==orient) {
            if(coarse==fadeMode) {
                value=valTouchBegin+(y-yTouchBegin)*valRange/(height-knobSize);
            } else {
                value=valTouchBegin+(y-yTouchBegin)/fineness;
            }
            if(value>maxValue) value=maxValue;
            if(value<minValue) value=minValue;
            if(inverted) {
                valueDisplay = maxValue - value;
            } else {
                valueDisplay = value;
            }
            emit valueChange(valueDisplay);
            calcGeo();
        }
    }
}

void MWFadder::onReleased()
{
    pressed--;
}

void MWFadder::onResize(int h)
{
    height = h;
    calcGeo();
}

int MWFadder::getValue()
{
    return valueDisplay;
}

void MWFadder::calcGeo()
{
    _faderY=((float)value-(float)minValue)/(float)valRange;
    _text1.sprintf("%d",valueDisplay);
    emit geoChanged();
}

void MWFadder::setMaxValue(int value)
{
    maxValue = value;
    valRange=maxValue-minValue;
    calcGeo();
}

void MWFadder::setInverted(bool v)
{
    inverted = v;
    if(inverted) {
        value=maxValue;
    }
    calcGeo();
}

void MWFadder::setValue(int v)
{
    if(inverted) {
        value = maxValue - v;
        valueDisplay = v;
    } else {
        valueDisplay = v;
        value = v;
    }
    calcGeo();
    emit valueChange(valueDisplay);
}

void MWFadder::setMinValue(int value)
{
    minValue = value;
    valRange=maxValue-minValue;
    calcGeo();
}

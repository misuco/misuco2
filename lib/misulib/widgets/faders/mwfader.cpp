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

#include "mwfader.h"
#include <QDebug>



void MWFader::onPressed(int id, int x, int y, int h, int w)
{
    height = h;
    _knobSize = w;
    if(pressed<2) {
        pressedTouchId = id;
        xTouchBegin=x;
        yTouchBegin=y;
        valTouchBegin=value;

        float yRelative1 = (float)(y-_knobSize) / (float)(height-_knobSize);
        float yRelative2 = (float)(y) / (float)(height-_knobSize);
        bool fineModeAvailable = (float)valRange/(float)(height-_knobSize) > 1.0/(float)fineness;
        if((_faderY >= yRelative1 && _faderY <= yRelative2) || !fineModeAvailable) {
            fadeMode=coarse;
            //qDebug() << "coarse fade";
        } else {
            fadeMode=fine;
            //qDebug() << "fine fade";
        }
        valueChange();
    }
    pressed++;
}

void MWFader::onUpdated(int id, int y, int h, int w)
{
    height = h;
    _knobSize = w;
    if(id==pressedTouchId) {
        if(vertical==orient) {
            if(coarse==fadeMode) {
                value=valTouchBegin+(y-yTouchBegin)*valRange/(height-_knobSize);
            } else {
                value=valTouchBegin+(y-yTouchBegin)/fineness;
            }
            if(value>maxValue) value=maxValue;
            if(value<minValue) value=minValue;
            if(inverted) {
                _valueDisplay = maxValue - value;
            } else {
                _valueDisplay = value;
            }
            valueChange();
            calcGeo();
        }
    }
}

void MWFader::onReleased(int)
{
    pressed--;
}

void MWFader::onResize(int h)
{
    height = h;
    calcGeo();
}

int MWFader::getValue()
{
    return _valueDisplay;
}

void MWFader::valueChange()
{

}

void MWFader::calcGeo()
{
    _faderY=((float)value-(float)minValue)/(float)valRange;
    _text1.sprintf("%d",_valueDisplay);
    emit geoChanged();
}

void MWFader::setMaxValue(int value)
{
    maxValue = value;
    valRange=maxValue-minValue;
    calcGeo();
}

void MWFader::setInverted(bool v)
{
    inverted = v;
    if(inverted) {
        value=maxValue;
    }
    calcGeo();
}

void MWFader::setValue(int v)
{
    if(inverted) {
        value = maxValue - v;
        _valueDisplay = v;
    } else {
        _valueDisplay = v;
        value = v;
    }
    calcGeo();
    valueChange();
}

void MWFader::setMinValue(int value)
{
    minValue = value;
    valRange=maxValue-minValue;
    calcGeo();
}

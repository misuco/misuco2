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

#ifndef MWFADDER_H
#define MWFADDER_H

#include "misuwidget.h"
//#include "conf/color.h"

class MWFadder : public MisuWidget
{
    Q_OBJECT

public:
    MWFadder(QObject *parent);
    enum orientation {
        horizontal,
        vertical
    };
    enum FadeMode {
        coarse,
        fine
    };

    void setMinValue(int value);
    void setMaxValue(int value);    
    void setInverted(bool v);
    void setValue(int v);
    int getValue();

protected:
    void processTouchEvent(misuTouchEvent e);
    int valueDisplay;

signals:
    void valueChange(int v);

private:
    // Fadder state
    int value;
    bool inverted;
    int minValue;
    int maxValue;
    int valRange;
    int step1;
    int step2;
    int pressed;
    int orient;
    int fadderY;
    int knobSize;
    int fineness;

    // Touch event state
    int xTouchBegin;
    int yTouchBegin;
    int valTouchBegin;
    int fadeMode;

    void calcGeo();
};

#endif // MWFADDER_H

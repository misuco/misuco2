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

#ifndef MWFADERPITCH_H
#define MWFADERPITCH_H

#include <QObject>
#include <QWidget>
#include <widgets/mwfadder.h>
#include <conf/freqtriple.h>
#include "comm/isender.h"

class MWFaderPitch : public MWFadder
{
    Q_OBJECT

public:
    MWFaderPitch(QObject *parent, Pitch * p);
    ~MWFaderPitch();
    virtual void processTouchEvent(misuTouchEvent e);
    void setOut(ISender *value);

public slots:
    void setOctMid(int o);
    void pitchChange();

private:
    ISender * out;

    Pitch * p;
    FreqTriple * f;

    int vId;
    int pressed;
};

#endif // MWFADERPITCH_H

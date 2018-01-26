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

#ifndef MWBSCALESWITCH_H
#define MWBSCALESWITCH_H

#include "misuwidget.h"
#include "freqtriple.h"
#include "comm/isender.h"

class MWBScaleSwitch : public MisuWidget
{
    Q_OBJECT

    Q_PROPERTY(QColor color MEMBER _color NOTIFY colorChanged)
    Q_PROPERTY(QColor fontColor MEMBER _fontColor NOTIFY colorChanged)
    Q_PROPERTY(QString text1 MEMBER _text1 NOTIFY colorChanged)
    Q_PROPERTY(QString text2 MEMBER _text2 NOTIFY colorChanged)

public:
    MWBScaleSwitch(int i);
    ~MWBScaleSwitch();
    void setOut(ISender *_value);

    Q_INVOKABLE void onPressed();
    Q_INVOKABLE void onReleased();

signals:
    void setBscale(int,bool);
    void scaleupdate();
    void colorChanged();

public slots:
    void setrootNote(Pitch * p);
    void setOctMid(int o);
    void onScaleSet(MWScale*);
    void onscaleupdate();

//protected:
    //void processTouchEvent(misuTouchEvent e);

private:
    ISender * out;
    FreqTriple * _freq;
    int bscaleId;
    int rootNote;
    int vId;
    int _pressed;
    bool _value;

    QColor          _color;
    QColor          _fontColor;
    QString         _text1;
    QString         _text2;

    void calcColor();
};

#endif // MWBSCALESWITCH_H

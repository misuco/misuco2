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

#ifndef MWBASENOTESETTER_H
#define MWBASENOTESETTER_H

#include "misuwidget.h"
#include "comm/isender.h"
#include "conf/freqtriple.h"

class MWBaseNoteSetter : public MisuWidget
{
    Q_OBJECT

    Q_PROPERTY(QColor color MEMBER _color NOTIFY colorChanged)
    Q_PROPERTY(QColor fontColor MEMBER _fontColor NOTIFY colorChanged)
    Q_PROPERTY(QString text1 MEMBER _text1 NOTIFY colorChanged)
    Q_PROPERTY(QString text2 MEMBER _text2 NOTIFY colorChanged)

public:
    MWBaseNoteSetter(Pitch *pitch, QObject *parent);
    ~MWBaseNoteSetter();
    void setOut(ISender *value);

    Q_INVOKABLE void onPressed();
    Q_INVOKABLE void onReleased();

    QObject * pitch();

public slots:
    void setOctMid(int o);
    void pitchChange();
    void onSetBaseNote(Pitch *pitch);
    void onScaleSet(MWScale*scale);
    void onscaleupdate();

signals:
    void setBaseNote(Pitch *);
    void scaleupdate();
    void colorChanged();

    void fChanged();

private:
    ISender *       _out;
    Pitch *         _pitch;
    FreqTriple *    _freq;
    int             _vId;
    int             _pressed;
    bool            _selected;

    QColor          _color;
    QColor          _fontColor;
    QString         _text1;
    QString         _text2;

    void calcColor();
};

#endif // MWBASENOTESETTER_H

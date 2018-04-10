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

#ifndef MWrootNoteSETTER_H
#define MWrootNoteSETTER_H

#include "conf/mglob.h"
#include "comm/mastersender.h"
#include "conf/freqtriple.h"

class MWRootNoteSetter : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int pitchId READ pitchId CONSTANT)
    Q_PROPERTY(bool selected MEMBER _selected NOTIFY selectedChanged)
    Q_PROPERTY(QString text1 MEMBER _text1 NOTIFY textChanged)
    Q_PROPERTY(QString text2 MEMBER _text2 NOTIFY textChanged)

public:
    MWRootNoteSetter(Pitch *pitch, MasterSender *ms, QObject *parent);
    ~MWRootNoteSetter();

    int pitchId();

    Q_INVOKABLE void onPressed();
    Q_INVOKABLE void onReleased();

    QObject * pitch();

public slots:
    void setOctMid(int o);
    void pitchChange();
    void onSetRootNote(Pitch *pitch);
    void onScaleSet(MWScale*scale);
    void onSymbolsChanged();

signals:
    void setRootNote(int);

    // QML
    void selectedChanged();
    void textChanged();

private:
    MasterSender *  _out;
    Pitch *         _pitch;
    FreqTriple *    _freq;
    int             _vId;
    int             _pressed;
    bool            _selected;

    QString         _text1;
    QString         _text2;

    void calcText();
};

#endif // MWrootNoteSETTER_H

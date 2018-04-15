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

#ifndef MWSCALESWITCH_H
#define MWSCALESWITCH_H

#include "lib/misulib/models/freqtriple.h"
#include "lib/misulib/comm/mastersender.h"

class ScaleSwitch : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int rootNote MEMBER _rootNote NOTIFY rootNoteChanged)
    Q_PROPERTY(bool selected READ selected NOTIFY selectedChanged)
    Q_PROPERTY(QString text1 MEMBER _text1 NOTIFY textChanged)
    Q_PROPERTY(QString text2 MEMBER _text2 NOTIFY textChanged)

public:
    ScaleSwitch(int rootNote, MasterSender * ms, QObject *parent);
    ~ScaleSwitch();

    int pitchId();
    bool selected();

    Q_INVOKABLE void onPressed();
    Q_INVOKABLE void onReleased();

signals:
    void setScale(int,bool);

    // QML
    void rootNoteChanged();
    void selectedChanged();
    void textChanged();

public slots:
    void onSetRootNote(int p);
    void onPitchChange(int rootNote, int pitch);
    void setOctMid(int o);
    void onSetScale(int rootNote, QList<bool> scale);
    void onSymbolsChange(int s);
    void onShowFreqsChange(bool state);

private:
    MasterSender * _out;
    FreqTriple * _freq;
    int _rootNote;
    int _scaleId;
    int _vid;
    int _pressed;
    bool _value;

    int _noteSymbols;
    bool _showFreqs;

    int _oct;
    int _higherOct;

    QString         _text1;
    QString         _text2;

    void calcText();
};

#endif // MWSCALESWITCH_H

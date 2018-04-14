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

#ifndef FREQTRIPLE_H
#define FREQTRIPLE_H

#include <QObject>
#include <QColor>

class FreqTriple: public QObject
{
    Q_OBJECT

    Q_PROPERTY(float hue MEMBER _hue NOTIFY colorChanged)
    Q_PROPERTY(bool pianoWhite MEMBER _pianoWhite NOTIFY colorChanged)

public:
    explicit FreqTriple(int rootNote, QObject *parent = 0);
    ~FreqTriple();

    float getFreq() const;

    int getMidinote() const;
    void setMidinote(int midinote, int pitch);

    int getPitch() const;
    void setPitch(int value);

    float getHue() const;
    bool getBW() const;

    int getRootNote() const;
    void setRootNote(int rootNote);

    int getOct() const;
    void setOct(int value);

    QString getRootNoteString(int lang) const;

signals:
    void colorChanged();

public slots:
    void onPitchChange(int rootNote, int value);

private:
    // calculation base
    float _freq_a=440;

    // frequency determining parameters
    float       _freq;
    int         _midinote;
    int         _rootNote;
    int         _pitch;
    int         _oct;
    QList<int>  _pitchTable;

    // color parameters
    float _hue;
    bool _pianoWhite;

    float calcFreq(int _midinote, int _pitch);
    float calcMidi2Fequal(int x);    

    double Log2(double n);
    int calcOctFromMidinote(int m);
    int calcRootNoteFromMidinote(int m);
    void initFreq();
    void calcColor();
};

#endif // FREQTRIPLE_H

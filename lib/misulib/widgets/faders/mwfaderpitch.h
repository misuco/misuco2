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
#include "mwfader.h"
#include "lib/misulib/models/freqtriple.h"
#include "lib/misulib/comm/mastersender.h"

class MWFaderPitch : public MWFader
{
    Q_OBJECT

    Q_PROPERTY(int rootNote MEMBER _rootNote CONSTANT)
    Q_PROPERTY(bool selected READ selected NOTIFY selectedChanged)

public:
    MWFaderPitch(int rootNote, MasterSender * ms, QObject *parent);
    ~MWFaderPitch();

    bool selected();

    Q_INVOKABLE void onPressed(int id, int x, int y, int h, int w) override;
    Q_INVOKABLE void onUpdated(int id, int y, int h, int w) override;
    Q_INVOKABLE void onReleased(int id) override;

signals:
    void pitchChange(int rootNote, int pitch);
    // QML
    void selectedChanged();

public slots:
    void setOctMid(int o);
    void onPitchChange(int rootNote, int value);

protected:
    void valueChange() override;

private:
    MasterSender * _out;
    FreqTriple * _freq;
    int _rootNote;

    int vId;
    int eventId;
};

#endif // MWFADERPITCH_H

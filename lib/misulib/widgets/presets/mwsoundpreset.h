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

#ifndef MWSound_H
#define MWSound_H

#include <QObject>
#include "lib/misulib/models/types.h"
#include "lib/misulib/widgets/faders/mwfaderparamctl.h"

class MWSoundPreset : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int wave READ wave NOTIFY presetChanged)
    Q_PROPERTY(int attack READ attack NOTIFY presetChanged)
    Q_PROPERTY(int decay READ decay NOTIFY presetChanged)
    Q_PROPERTY(float sustain READ sustain NOTIFY presetChanged)
    Q_PROPERTY(int release READ release NOTIFY presetChanged)
    Q_PROPERTY(float cutoff READ cutoff NOTIFY presetChanged)
    Q_PROPERTY(float resonance READ resonance NOTIFY presetChanged)
    Q_PROPERTY(bool selected READ isSelected NOTIFY selectedChanged)

public:
    MWSoundPreset(QList<QObject *> soundFaders, QObject *parent);
    MWSoundPreset(float vol, int wav, int att, int dec, float sus, int rel, float cut, float res, float modcut, float modres, QList<QObject *> soundFaders, QObject *parent);

    int wave();
    int attack();
    int decay();
    float sustain();
    int release();
    float cutoff();
    float resonance();
    float mod_cutoff();
    float mod_resonance();
    float volume();

    void overwrite();

    Q_INVOKABLE void onPressed();
    Q_INVOKABLE void onPressAndHold();
    Q_INVOKABLE void onCanceled();
    Q_INVOKABLE void onReleased();

public slots:
    void initialSet();
    void onSoundChanged();

signals:
    void setSound(MWSound * s);
    void editPreset();

    // QML
    void presetChanged();
    void selectedChanged();

private:
    MWSound PresetSound;
    QList<MWFaderParamCtl*> _soundFaders;

    int pressed=0;
    int fontheight=8;
    bool isSelected();

    bool canceled;
};

#endif // MWSound_H

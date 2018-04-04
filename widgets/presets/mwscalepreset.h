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

#ifndef MWPRESET_H
#define MWPRESET_H

#include "conf/mglob.h"
#include "conf/types.h"

class MWScalePreset : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int rootNote READ rootnote NOTIFY presetChanged)
    Q_PROPERTY(QStringList bScale READ bscale NOTIFY presetChanged)
    Q_PROPERTY(int bScaleSize READ bscalesize NOTIFY presetChanged)
    Q_PROPERTY(bool selected READ isSelected NOTIFY presetChanged)

public:
    MWScalePreset(QObject *parent);
    MWScalePreset(int rootNote, bool bscale[BSCALE_SIZE], QObject *parent);

    MWScale PresetScale;

    QStringList bscale();
    int bscalesize();
    int rootnote();

    void overwrite();

    Q_INVOKABLE void onPressed();
    Q_INVOKABLE void onPressAndHold();
    Q_INVOKABLE void onCanceled();
    Q_INVOKABLE void onReleased();

public slots:
    void initialSet();
    void playAreaChanged();

signals:
    void setScale(MWScale *);
    void editPreset();

    // QML
    void presetChanged();

private:
    int pressed;
    bool isSelected();
    bool canceled;
};

#endif // MWPRESET_H

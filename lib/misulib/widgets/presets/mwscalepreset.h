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

#include <QObject>
#include "lib/misulib/models/types.h"

class MWScalePreset : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int rootNote MEMBER _presetRootNote NOTIFY presetChanged)
    Q_PROPERTY(QStringList Scale READ scale NOTIFY presetChanged)
    Q_PROPERTY(int ScaleSize MEMBER _scaleSize NOTIFY presetChanged)
    Q_PROPERTY(bool selected READ isSelected NOTIFY selectedChanged)

public:
    MWScalePreset(QObject *parent);
    MWScalePreset(int rootNote, QList<bool> scale, QObject *parent);

    QStringList scale();
    int getRootNote();
    bool getScale(int i);

    void overwrite();

    Q_INVOKABLE void onPressed();
    Q_INVOKABLE void onPressAndHold();
    Q_INVOKABLE void onCanceled();
    Q_INVOKABLE void onReleased();

public slots:
    void onSetRootNote(int p);
    void onSetScale(int n, bool v);
    void onSetScale(int rootNote, QList<bool> scale);
    void initialSet();

signals:
    void setScale(int rootNote, QList<bool> scale);
    void editPreset();

    // QML
    void presetChanged();
    void selectedChanged();

private:
    int _currentRootNote;
    QList<bool> _currentScale;

    int _presetRootNote;
    QList<bool> _presetScale;

    int _pressed;
    int _scaleSize;
    bool _canceled;

    bool isSelected();

};

#endif // MWPRESET_H

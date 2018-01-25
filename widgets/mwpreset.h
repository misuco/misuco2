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

#include "misuwidget.h"
#include "conf/types.h"

class MWPreset : public MisuWidget
{
    Q_OBJECT

public:
    MWPreset(Pitch * p[], QWidget *parent);
    MWPreset(Pitch *p[], int basenote, int baseoct, int topoct, bool bscale[BSCALE_SIZE], QObject *parent);

    virtual void processTouchEvent(misuTouchEvent e);
    MWScale PresetScale;

public slots:
    void initialSet();

signals:
    void setScale(MWScale *);
    void scaleupdate();

private:
    Pitch ** MWPitch;
    int pressed=0;
    bool isSelected();
};

#endif // MWPRESET_H

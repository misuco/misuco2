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
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef MWSound_H
#define MWSound_H

#include "misuwidget.h"
#include "conf/types.h"

class MWSoundPreset : public MisuWidget
{
    Q_OBJECT

public:
    MWSoundPreset(QWidget *parent);
    MWSoundPreset(float vol, int wav, int att, int dec, float sus, int rel, float cut, float res, float modcut, float modres, QWidget *parent);
    virtual void processTouchEvent(misuTouchEvent e);
    MWSound PresetSound;

protected:
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *);

public slots:
    void initialSet();

signals:
    void setSound(MWSound * s);

private:
    int pressed=0;
    bool isSelected();
};

#endif // MWSound_H

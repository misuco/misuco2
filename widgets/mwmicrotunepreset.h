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

#ifndef MWMicrotunePreset_H
#define MWMicrotunePreset_H

#include "misuwidget.h"
#include "conf/types.h"

class MWMicrotunePreset : public MisuWidget
{
    Q_OBJECT

public:
    MWMicrotunePreset(QWidget *parent);
    MWMicrotunePreset(int tuning[], QWidget *parent);
    virtual void processTouchEvent(misuTouchEvent e);
    MWMicrotune PresetMicrotune;

protected:
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *);

public slots:
    void initialSet();

signals:
    void setMicrotune(MWMicrotune * m);

private:
    int pressed=0;
    bool isSelected();
};

#endif // MWMicrotunePreset_H

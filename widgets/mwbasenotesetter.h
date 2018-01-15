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

#ifndef MWBASENOTESETTER_H
#define MWBASENOTESETTER_H

#include "misuwidget.h"
#include "comm/isender.h"
#include "conf/freqtriple.h"

class MWBaseNoteSetter : public MisuWidget
{
    Q_OBJECT

public:
    MWBaseNoteSetter(Pitch *pitch, QWidget *parent);
    ~MWBaseNoteSetter();
    virtual void processTouchEvent(misuTouchEvent e);
    void setOut(ISender *value);

protected:
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *);

public slots:
    void setOctMid(int o);
    void pitchChange();
    void onSetBaseNote(Pitch *pitch);
    void onScaleSet(MWScale*scale);
    void onScaleUpdate();


signals:
    void setBaseNote(Pitch *);
    void scaleUpdate();

private:
    ISender * out;
    Pitch * p;
    FreqTriple * f;
    int vId;
    int pressed;
    bool selected;
};

#endif // MWBASENOTESETTER_H

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

#ifndef MWHEADERSETTER_H
#define MWHEADERSETTER_H

#include "misuwidget.h"

class MWHeaderSetter : public MisuWidget
{
    Q_OBJECT

public:
    MWHeaderSetter(int headerId, QObject *parent);
    MWHeaderSetter(int headerId, int state, QObject *parent);
    virtual void processTouchEvent(misuTouchEvent e);
    int getState();

signals:
    void currentHeader(int i);
    void currentMainView(int i);
    void setBendHori(bool b);
    void setBendVertTop(int b);
    void setBendVertBot(int b);
    void togglePresets();
    void toggleMenu();
    void toggleBW();
    void scaleupdate();
    void toggleSender(int i);
    void toggleShowFreqs();

public slots:
    void setState(int id, int s);

private:
    int headerId;
    int pressed;
    int state;
    QString midi2TextUrl(int midinote);

};

#endif // MWHEADERSETTER_H

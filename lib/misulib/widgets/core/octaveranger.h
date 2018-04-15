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

#ifndef MWOCTAVERANGER_H
#define MWOCTAVERANGER_H

#include <QObject>

class OctaveRanger : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int bottom MEMBER bottom NOTIFY bottomChanged)
    Q_PROPERTY(int top MEMBER top NOTIFY topChanged)
    Q_PROPERTY(int nOctaves MEMBER noct CONSTANT)

public:
    OctaveRanger(QObject *parent);
    ~OctaveRanger();

    Q_INVOKABLE void onUpdated(int id, int x);
    Q_INVOKABLE void onReleased(int id);
    Q_INVOKABLE void onResize(int w);

signals:
    void setOctConf(int bottom,int top);
    void setOctMid(int mid);
    void bottomChanged();
    void topChanged();

public slots:
    void octUp();
    void octDown();

private:
    int top;
    int mid;
    int bottom;
    int segwidth;
    int noct;
    bool grabedTop;
    int grabedTopTouchId;
    bool grabedBot;
    int grabedBotTouchId;
};

#endif // MWOCTAVERANGER_H

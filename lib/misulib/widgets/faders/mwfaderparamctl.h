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

#ifndef MWFaderParamCtl_H
#define MWFaderParamCtl_H

#include <QObject>
#include "mwfader.h"
#include "lib/misulib/comm/mastersender.h"

class MWFaderParamCtl : public MWFader
{
    Q_OBJECT

public:
    MWFaderParamCtl(int cc, MasterSender * ms, QObject *parent);
    ~MWFaderParamCtl();

    int getCc();

signals:
    void controlValueChange(int value);

protected:
    void valueChange() override;

private:
    MasterSender * _out;

    int _cc;
    int _pressed;
    int _eventId;

};

#endif // MWFaderParamCtl_H

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

#include "mwfaderpitch.h"
#include <QDebug>

MWFaderPitch::MWFaderPitch(QObject *parent, Pitch *p) : MWFadder(parent)
{
    f = new FreqTriple(p);
    f->setOct(4);
}

MWFaderPitch::~MWFaderPitch()
{
    f->deleteLater();
}

void MWFaderPitch::setOut(ISender *value)
{
    out = value;
}

/*
void MWFaderPitch::onResize(int w, int h)
{
    qDebug() << "MWFaderPitch::onResize w: " << w << " h: " << h;
    MWFadder::onResize(w,h);
}
*/

void MWFaderPitch::setOctMid(int o)
{
    f->setOct(o);
}

void MWFaderPitch::pitchChange()
{
    f->pitchChange();
    //update();
}

void MWFaderPitch::processTouchEvent(misuTouchEvent e)
{
    switch(e.state) {
    case Qt::TouchPointPressed:
        vId=out->noteOn(channel,f->getFreq(),f->getMidinote(),f->getPitch(),127);
        //qDebug() << "MWFaderPitch::processTouchEvent TouchPointPressed " << out << " vId:" << vId;
        //update();
        break;
    case Qt::TouchPointMoved:
        out->pitch(channel,vId,f->getFreq(),f->getMidinote(),f->getPitch());
        Microtune.tuning[f->getrootNote()] = getValue();
        //update();
        break;
    case Qt::TouchPointReleased:
        //qDebug() << "MWFaderPitch::processTouchEvent TouchPointReleased vId:" << vId;
        out->noteOff(vId);
        //update();
        break;
    }
    //MWFadder::processTouchEvent(e);
}

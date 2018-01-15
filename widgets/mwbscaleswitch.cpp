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

#include "mwbscaleswitch.h"
#include <QPainter>
#include "comm/senderdebug.h"
#include <QDebug>

MWBScaleSwitch::MWBScaleSwitch(int i, Pitch **MWP)
{
    //qDebug() << "MWBScaleSwitch::MWBScaleSwitch " << sizeof(Pitch);

    for(int i=0;i<12;i++) {
        MWPitch[i]=*(MWP+i);
        f=new FreqTriple(MWPitch[i],this);
        f->setBasenote(MWPitch[i]);
    }
    // TODO: how to type cast this?
    //MWPitch=MWP;
    value=false;
    pressed=0;
    bscaleId=i;
    f->setOct(4);
    out=new SenderDebug();
}

MWBScaleSwitch::~MWBScaleSwitch()
{
    delete(f);
}

void MWBScaleSwitch::processTouchEvent(misuTouchEvent e)
{
    switch(e.state) {
    case Qt::TouchPointPressed:
        vId=out->noteOn(channel,f->getFreq(),f->getMidinote(),f->getPitch(),127);
        value=!value;
        emit setBscale(bscaleId,value);
        emit scaleUpdate();
        pressed++;
        update();
        break;
    case Qt::TouchPointReleased:
        out->noteOff(vId);
        pressed--;
        update();
        break;
    }
}

void MWBScaleSwitch::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QString cap;
    QFont font(font1);
    font.setPixelSize(font1size);
    painter.setFont(font);

    if(bwmode) {
        if(value) {
            if(f->getBW()) {
                painter.setBrush(hlwkeycolor);
            } else {
                painter.setBrush(hlbkeycolor);
            }
        } else if(f->getBW()) {
            painter.setBrush(wkeycolor);
        } else {
            painter.setBrush(bkeycolor);
        }
    } else {
        int l=lOff;
        int s=sOff;
        if(value) {
            l=lOn;
            s=sOn;
        }
        painter.setBrush(QColor::fromHsl(f->getHue(),s,l));
    }

    painter.setPen(Qt::NoPen);
    painter.drawRect(0,0,width(),height());

    if(pressed>0 || value) {
        painter.setPen(highlightcolor);
    } else {
        painter.setPen(fgcolor);
    }
    cap.sprintf("%d",bscaleId);
    painter.drawText(0,0,width(),height(),Qt::AlignCenter|Qt::AlignHCenter,cap);
}

void MWBScaleSwitch::setOut(ISender *value)
{
    out = value;
}

void MWBScaleSwitch::setBaseNote(Pitch *p)
{
    basenote=p->basenote;
    int newBaseNote=(basenote+bscaleId)%12;
    //qDebug() << "MWBScaleSwitch::setBaseNote " << newBaseNote << " bscaleId " << bscaleId << " basenote " << basenote;
    f->setBasenote(MWPitch[newBaseNote]);
    update();
}

void MWBScaleSwitch::setOctMid(int o)
{
    f->setOct(o);
}

void MWBScaleSwitch::onScaleSet(MWScale * scale)
{
    if(scale->bscale[bscaleId-1]) {
        value=true;
    } else {
        value=false;
    }

    basenote = (scale->basenote+bscaleId)%12;
    f->setBasenote(MWPitch[basenote]);

    update();
}

void MWBScaleSwitch::onScaleUpdate()
{
    update();
}

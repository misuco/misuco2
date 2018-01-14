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

#include "mwbasenotesetter.h"
#include "comm/senderdebug.h"
#include <QPainter>
#include <QDebug>

MWBaseNoteSetter::MWBaseNoteSetter(Pitch * pitch, QWidget *parent) : MisuWidget(parent)
{
    //qDebug() << "MWBaseNoteSetter::MWBaseNoteSetter " << pitch->pitch << " pitch basenote " << pitch->basenote;
    out=new SenderDebug();
    p=pitch;
    f=new FreqTriple(p);
    f->setOct(4);
    f->setBasenote(p);
    //qDebug() << "f: " << f->getFreq() << " " << f->getPitch() << " " << f->getHue();
    vId=0;
    pressed=0;
    selected=false;
}

MWBaseNoteSetter::~MWBaseNoteSetter()
{
    f->deleteLater();
}

void MWBaseNoteSetter::processTouchEvent(misuTouchEvent e)
{
    switch(e.state) {
    case Qt::TouchPointPressed:
        vId=out->noteOn(channel,f->getFreq(),f->getMidinote(),f->getPitch(),127);
        //qDebug() << "MWBaseNoteSetter::processTouchEvent TouchPointPressed " << out << " vId:" << vId;
        emit setBaseNote(p);
        emit scaleUpdate();
        //qDebug() << "MWBaseNoteSetter::processTouchEvent emit setBaseNote " << f->getBasenote();
        pressed++;
        update();
        break;
    case Qt::TouchPointReleased:
        //qDebug() << "MWBaseNoteSetter::processTouchEvent TouchPointReleased vId:" << vId;
        out->noteOff(vId);
        pressed--;
        update();
        break;
    }
}

void MWBaseNoteSetter::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QString cap;
    int l=lOff;
    int s=sOff;
    if(pressed>0 || selected) {
        l=lOn;
        s=sOn;
    }
    if(bwmode) {
        if(selected) {
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
        painter.setBrush(QColor::fromHsl(f->getHue(),s,l));
    }
    painter.setPen(Qt::NoPen);
    painter.drawRect(0,0,width(),height());
    //cap.sprintf("%d %5.2f",f->getBasenote(), f->getFreq());

    if(pressed>0 || selected) {
        painter.setPen(highlightcolor);
    } else {
        painter.setPen(fgcolor);
    }

    QFont font(font1);
    QString basenote = f->getBasenoteString(noteSymbols);
    if(basenote.startsWith("_")) {
        font.setUnderline(true);
        basenote.remove(0,1);
    }
    font.setPixelSize(font1size);
    painter.setFont(font);
    cap.sprintf("%s",basenote.toStdString().c_str());
    painter.drawText(0,0,width(),height(),Qt::AlignTop|Qt::AlignCenter,cap);
    font.setUnderline(false);

    if(showFreqs) {
        font.setPixelSize(font1size/2);
        painter.setFont(font);
        cap.sprintf("%5.2f",f->getFreq());
        painter.drawText(0,0,width(),height(),Qt::AlignBottom|Qt::AlignCenter,cap);
    }
}

void MWBaseNoteSetter::resizeEvent(QResizeEvent *)
{
    //qDebug() << "MWBaseNoteSetter::resizeEvent" << width();
}

void MWBaseNoteSetter::setOctMid(int o)
{
    f->setOct(o);
    update();
}

void MWBaseNoteSetter::pitchChange()
{
    //qDebug() << "MWBaseNoteSetter::pitchChange "  << f->getPitch();
    f->pitchChange();
    update();
}

void MWBaseNoteSetter::onSetBaseNote(Pitch * pitch)
{
    if(pitch == p) {
        selected = true;
    } else {
        selected = false;
    }
    update();
}

void MWBaseNoteSetter::onScaleSet(MWScale * scale)
{
    //qDebug() << "MWBaseNoteSetter::onScaleSet " << scale->basenote << " " << p->basenote;
    if(scale->basenote==p->basenote) {
        selected = true;
    } else {
        selected = false;
    }
    update();
}

void MWBaseNoteSetter::onScaleUpdate()
{
    update();
}

void MWBaseNoteSetter::setOut(ISender *value)
{
    out = value;
    //qDebug() << "MWBaseNoteSetter::setOut:" << out;
}


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

#include "mwbasenotesetter.h"
#include "comm/senderdebug.h"
#include <QPainter>
#include <QDebug>

MWBaseNoteSetter::MWBaseNoteSetter(Pitch * pitch, QObject *parent) : MisuWidget(parent)
{
    //qDebug() << "MWBaseNoteSetter::MWBaseNoteSetter " << pitch->pitch << " pitch basenote " << pitch->basenote;
    _out=new SenderDebug();
    _pitch=pitch;
    _freq=new FreqTriple(_pitch);
    _freq->setOct(4);
    _freq->setBasenote(_pitch);
    //qDebug() << "f: " << f->getFreq() << " " << f->getPitch() << " " << f->getHue();
    _vId=0;
    _pressed=0;
    _selected=false;
    calcColor();
}

MWBaseNoteSetter::~MWBaseNoteSetter()
{
    _freq->deleteLater();
}

void MWBaseNoteSetter::processTouchEvent(misuTouchEvent e)
{
    switch(e.state) {
    case Qt::TouchPointPressed:
        _vId=_out->noteOn(channel,_freq->getFreq(),_freq->getMidinote(),_freq->getPitch(),127);
        //qDebug() << "MWBaseNoteSetter::processTouchEvent TouchPointPressed " << out << " vId:" << vId;
        emit setBaseNote(_pitch);
        emit scaleupdate();
        //qDebug() << "MWBaseNoteSetter::processTouchEvent emit setBaseNote " << f->getBasenote();
        _pressed++;
        //update();
        break;
    case Qt::TouchPointReleased:
        //qDebug() << "MWBaseNoteSetter::processTouchEvent TouchPointReleased vId:" << vId;
        _out->noteOff(_vId);
        _pressed--;
        //update();
        break;
    }
}

/*
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
        painter.setFont(font);
        cap.sprintf("%5.1f",f->getFreq());
        painter.drawText(0,0,width(),height(),Qt::AlignBottom|Qt::AlignCenter,cap);
    }
}
*/

void MWBaseNoteSetter::setOctMid(int o)
{
    _freq->setOct(o);
    calcColor();
}

void MWBaseNoteSetter::pitchChange()
{
    //qDebug() << "MWBaseNoteSetter::pitchChange "  << f->getPitch();
    _freq->pitchChange();
    calcColor();
}

void MWBaseNoteSetter::onSetBaseNote(Pitch * pitch)
{
    if(pitch == _pitch) {
        _selected = true;
    } else {
        _selected = false;
    }
    calcColor();
}

void MWBaseNoteSetter::onScaleSet(MWScale * scale)
{
    //qDebug() << "MWBaseNoteSetter::onScaleSet " << scale->basenote << " " << p->basenote;
    if(scale->basenote==_pitch->getBasenote()) {
        _selected = true;
    } else {
        _selected = false;
    }
    calcColor();
}

void MWBaseNoteSetter::onscaleupdate()
{
    calcColor();
}

void MWBaseNoteSetter::calcColor()
{
    int l=lOff;
    int s=sOff;
    if(_pressed>0 || _selected) {
        l=lOn;
        s=sOn;
    }
    if(bwmode) {
        if(_selected) {
            if(_pitch->getBW()) {
                _color = hlwkeycolor;
            } else {
                _color = hlbkeycolor;
            }
        } else if(_pitch->getBW()) {
            _color = wkeycolor;
        } else {
            _color = bkeycolor;
        }
    } else {
        _color.fromHsl(_pitch->getHue(),s,l);
    }


    if(_pressed>0 || _selected) {
        _fontColor=highlightcolor;
    } else {
        _fontColor=fgcolor;
    }

    QString basenote = _freq->getBasenoteString(noteSymbols);
    if(basenote.startsWith("_")) {
        _underline=true;
        basenote.remove(0,1);
        basenote="<u>"+basenote+"</u>";
    } else {
        _underline=false;
    }

    _text1=basenote;

    if(showFreqs) {
        _text2.sprintf("%5.1f",_freq->getFreq());
    } else {
        _text2="";
    }

    emit colorChanged();
}

void MWBaseNoteSetter::setOut(ISender *value)
{
    _out = value;
    //qDebug() << "MWBaseNoteSetter::setOut:" << out;
}

void MWBaseNoteSetter::onPressed()
{
    _vId=_out->noteOn(channel,_freq->getFreq(),_freq->getMidinote(),_freq->getPitch(),127);
    emit setBaseNote(_pitch);
    emit scaleupdate();
    _pressed++;
    calcColor();
}

void MWBaseNoteSetter::onReleased()
{
    _out->noteOff(_vId);
    _pressed--;
    calcColor();
}

QObject *MWBaseNoteSetter::pitch()
{
    return _pitch;
}


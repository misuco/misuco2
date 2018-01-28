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

#include "mwfaderparamctl.h"
#include <QPainter>
#include <QDebug>

MWFaderParamCtl::MWFaderParamCtl(QObject *parent, int cc) : MWFadder(parent), cc(cc)
{
    switch(cc) {
    case 1:
        _text2="top";
        break;
    case 2:
        _text2="bot";
        break;
    case 3:
        _text2="channel";
        break;
    case 4:
        _text2="symbols";
        break;
    case 102:
        _text2="WAV";
        break;
    case 103:
        _text2="ATT";
        break;
    case 104:
        _text2="DEC";
        break;
    case 105:
        _text2="SUST";
        break;
    case 106:
        _text2="REL";
        break;
    case 107:
        _text2="FLTF";
        break;
    case 108:
        _text2="FLTR";
        break;
    case 109:
        _text2="MODF";
        break;
    case 110:
        _text2="MODR";
        break;
    case 111:
        _text2="VOL";
        break;
    }
}

MWFaderParamCtl::~MWFaderParamCtl()
{
}

void MWFaderParamCtl::setOut(ISender *value)
{
    out = value;
}

void MWFaderParamCtl::setValue(int v)
{
    MWFadder::setValue(v);
    propagateValueChange();
    emit geoChanged();
}

/*
void MWFaderParamCtl::paintEvent(QPaintEvent *E)
{
    MWFadder::paintEvent(E);

    QPainter painter(this);
    QString cap;

    if(pressed>0) painter.setBrush(highlightcolor);
    else painter.setBrush(bgcolor);

    QPen pen1 = QPen(fgcolor);
    QPen pen2 = QPen(highlightcolor);
    pen2.setWidth(3);
    painter.setPen(pen2);
    QFont font(font1);
    font.setPixelSize(font1size);
    painter.setFont(font);

    int sustainLineY = font1size + ((1000.0f - Sound.sustain) * (float)(height()-2*font1size) / 1000.0f);
    int filterSpikeH = (Sound.filter_resonance) * (float)(height()-2*font1size) / 1000.0f;
    int filterY1 =height()/2 - filterSpikeH / 2;
    int filterY2 =filterY1 + filterSpikeH;

    switch(cc) {
    case 1:
        _text2="bend\ntop");
        break;
    case 2:
        _text2="bend\nbot");
        break;
    case 3:
        _text2="channel");
        break;
    case 4:
        _text2="symbols");
        break;
    case 102:
        _text2="WAV");
        for(int i=0;i<5;i++) {
            switch(i) {
            case 0:
                _text2="SQR");
                break;
            case 1:
                _text2="SAW");
                break;
            case 2:
                _text2="SIN");
                break;
            case 3:
                _text2="TRI");
                break;
            case 4:
                _text2="RND");
                break;
            }
            painter.drawText(0,height()*(4-i)/5,width(),height()/5,Qt::AlignCenter|Qt::AlignVCenter,cap);
        }
        break;
    case 103:
        _text2="ATT");
        painter.drawLine(0,height()-font1size,width()-width()*valueDisplay/1000,height()-font1size);
        painter.drawLine(width()-width()*valueDisplay/1000,height()-font1size,width(),font1size);
        break;
    case 104:
        _text2="DEC");
        painter.drawLine(0,font1size,width()*valueDisplay/1000,sustainLineY);
        painter.drawLine(width()*valueDisplay/1000,sustainLineY,width(),sustainLineY);
        break;
    case 105:
        _text2="SUST");
        painter.drawLine(0,sustainLineY,width(),sustainLineY);
        break;
    case 106:
        _text2="REL");
        painter.drawLine(0,sustainLineY,width()*valueDisplay/1000,height()-font1size);
        painter.drawLine(width()*valueDisplay/1000,height()-font1size,width(),height()-font1size);
        break;
    case 107:
        _text2="FLTF");
        painter.drawLine(0,filterY2,width()*valueDisplay/1000,filterY1);
        painter.drawLine(width()*valueDisplay/1000,filterY1,width(),filterY2);
        break;
    case 108:
        _text2="FLTR");
        painter.drawLine(0,filterY2,width(),filterY2);
        break;
    case 109:
        _text2="MODF");
        break;
    case 110:
        _text2="MODR");
        break;
    case 111:
        _text2="VOL");
        break;

    }
    painter.setPen(pen1);
    painter.drawText(0,0,width(),height(),Qt::AlignBottom|Qt::AlignLeft,cap);
    //qDebug() << "MWFadder::paintEvent hue " << color->getHue() << " value " << value;
}

*/

void MWFaderParamCtl::propagateValueChange() {

    switch(cc) {
    case 3:
        channel=getValue();
        break;
    case 102:
        Sound.wave_type=getValue();
        break;
    case 103:
        Sound.attack=getValue();
        break;
    case 104:
        Sound.decay=getValue();
        break;
    case 105:
        Sound.sustain=getValue();
        break;
    case 106:
        Sound.release=getValue();
        break;
    case 107:
        Sound.filter_cutoff=getValue();
        break;
    case 108:
        Sound.filter_resonance=getValue();
        break;
    case 109:
        Sound.mod_filter_cutoff=getValue();
        break;
    case 110:
        Sound.mod_filter_resonance=getValue();
        break;
    case 111:
        Sound.volume=getValue();
        break;
    }

    out->cc(channel,0,cc,getValue(),getValue());
}

void MWFaderParamCtl::onPressedPitch(int id)
{
    //qDebug() << "MWFaderPitch::onPressedPitch " << id << " pressed " << pressed << " eventId " << eventId;
    if(pressed < 2) {
        eventId=id;
        propagateValueChange();
    }
}

void MWFaderParamCtl::onUpdatedPitch(int id)
{
    //qDebug() << "MWFaderPitch::onUpdatedPitch " << id << " pressed " << pressed << " eventId " << eventId;
    if(id == eventId) {
        out->cc(channel,0,cc,getValue(),getValue());
        propagateValueChange();
    }
}

void MWFaderParamCtl::onReleasedPitch(int)
{
}

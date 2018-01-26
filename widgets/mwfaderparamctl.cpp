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
    //update();
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
        cap.sprintf("bend\ntop");
        break;
    case 2:
        cap.sprintf("bend\nbot");
        break;
    case 3:
        cap.sprintf("channel");
        break;
    case 4:
        cap.sprintf("symbols");
        break;
    case 102:
        cap.sprintf("WAV");
        for(int i=0;i<5;i++) {
            switch(i) {
            case 0:
                cap.sprintf("SQR");
                break;
            case 1:
                cap.sprintf("SAW");
                break;
            case 2:
                cap.sprintf("SIN");
                break;
            case 3:
                cap.sprintf("TRI");
                break;
            case 4:
                cap.sprintf("RND");
                break;
            }
            painter.drawText(0,height()*(4-i)/5,width(),height()/5,Qt::AlignCenter|Qt::AlignVCenter,cap);
        }
        break;
    case 103:
        cap.sprintf("ATT");
        painter.drawLine(0,height()-font1size,width()-width()*valueDisplay/1000,height()-font1size);
        painter.drawLine(width()-width()*valueDisplay/1000,height()-font1size,width(),font1size);
        break;
    case 104:
        cap.sprintf("DEC");
        painter.drawLine(0,font1size,width()*valueDisplay/1000,sustainLineY);
        painter.drawLine(width()*valueDisplay/1000,sustainLineY,width(),sustainLineY);
        break;
    case 105:
        cap.sprintf("SUST");
        painter.drawLine(0,sustainLineY,width(),sustainLineY);
        break;
    case 106:
        cap.sprintf("REL");
        painter.drawLine(0,sustainLineY,width()*valueDisplay/1000,height()-font1size);
        painter.drawLine(width()*valueDisplay/1000,height()-font1size,width(),height()-font1size);
        break;
    case 107:
        cap.sprintf("FLTF");
        painter.drawLine(0,filterY2,width()*valueDisplay/1000,filterY1);
        painter.drawLine(width()*valueDisplay/1000,filterY1,width(),filterY2);
        break;
    case 108:
        cap.sprintf("FLTR");
        painter.drawLine(0,filterY2,width(),filterY2);
        break;
    case 109:
        cap.sprintf("MODF");
        break;
    case 110:
        cap.sprintf("MODR");
        break;
    case 111:
        cap.sprintf("VOL");
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

void MWFaderParamCtl::processTouchEvent(misuTouchEvent e)
{
    MWFadder::processTouchEvent(e);
    switch(e.state) {
    case Qt::TouchPointPressed:
        //qDebug() << "MWFaderParamCtl::processTouchEvent TouchPointPressed " << out << " cc:" << cc << " value: " << getValue();
        propagateValueChange();
        pressed++;
        //update();
        break;
    case Qt::TouchPointMoved:
        out->cc(channel,0,cc,getValue(),getValue());
        propagateValueChange();
        //update();
        break;
    case Qt::TouchPointReleased:
        //qDebug() << "MWFaderParamCtl::processTouchEvent TouchPointReleased cc:" << cc;
        pressed--;
        //update();
        break;
    }
}

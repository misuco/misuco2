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

#include "mwoctaveranger.h"
#include <QPainter>
#include <QDebug>

MWOctaveRanger::MWOctaveRanger(QWidget *parent) : MisuWidget(parent)
{
    //qDebug() << "MWOctaveRanger::MWOctaveRanger()";
    //setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    top=4;
    mid=3;
    bottom=3;
    noct=10;
    grabed=none;
}

MWOctaveRanger::~MWOctaveRanger()
{

}

void MWOctaveRanger::processTouchEvent(MisuWidget::misuTouchEvent e)
{
    int touchOct;
    switch(e.state) {
    case Qt::TouchPointPressed:
    case Qt::TouchPointMoved:
        touchOct=e.x/segwidth;
        if(touchOct==bottom) {
            grabed=bottomPad;
        } else if( touchOct==top) {
            grabed=topPad;
        } else {
            if(touchOct<bottom) {
                bottom=touchOct;
            } else if(touchOct>top) {
                top=touchOct;
            } else {
                if(grabed==topPad) {
                    top=touchOct;
                } else if(grabed==bottomPad) {
                    bottom=touchOct;
                } else {
                    int distTop=top-touchOct;
                    int distBottom=touchOct-bottom;
                    if(distTop>distBottom) {
                        bottom=touchOct;
                    }
                    if(distTop<distBottom) {
                        top=touchOct;
                    }
                }
            }
            emit setOctConf(bottom,top);
            int midnew=bottom+(top-bottom)/2;
            if(midnew!=mid) {
                mid=midnew;
                emit setOctMid(mid);
            }
            update();
        }
        break;
    case Qt::TouchPointReleased:
        grabed=none;
        break;
    }

}

void MWOctaveRanger::paintEvent(QPaintEvent *)
{
    //qDebug() << "MWOctaveRanger::paintEvent";
    QPainter painter(this);
    QString cap;
    QFont font(font1);
    font.setPixelSize(font1size);
    painter.setFont(font);
    painter.setPen(Qt::NoPen);
    painter.setBrush(bgcolor);
    painter.drawRect(0,0,width(),height());
    painter.setBrush(highlightcolor);
    painter.drawRect(bottom*segwidth,0,(1+top-bottom)*segwidth,height());
    painter.setPen(fgcolor);
    for(int i=0;i<noct;i++) {
        cap.sprintf("%d",i);
        painter.drawText(i*segwidth,0,segwidth,height(),Qt::AlignCenter|Qt::AlignHCenter,cap);
    }
}

void MWOctaveRanger::resizeEvent(QResizeEvent *)
{
    //qDebug() << "MWOctaveRanger::resizeEvent";
    segwidth=width()/noct;
}


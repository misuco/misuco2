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

#include "mwoctaveranger.h"

MWOctaveRanger::MWOctaveRanger(QObject *parent) : QObject(parent)
{
    top=7;
    mid=6;
    bottom=6;
    noct=10;
    grabedTop=false;
    grabedBot=false;
}

MWOctaveRanger::~MWOctaveRanger()
{

}

void MWOctaveRanger::onUpdated(int id, int x)
{
    int touchOct=x/segwidth;
    if(touchOct==bottom) {
        grabedBot=true;
        grabedBotTouchId=id;
    } else if( touchOct==top) {
        grabedTop=true;
        grabedTopTouchId=id;
    } else {
        if(touchOct<bottom) {
            bottom=touchOct;
            emit bottomChanged();
        } else if(touchOct>top) {
            top=touchOct;
            emit topChanged();
        } else {
            if(grabedTop && grabedTopTouchId == id) {
                top=touchOct;
                emit topChanged();
            } else if(grabedBot && grabedBotTouchId == id) {
                bottom=touchOct;
                emit bottomChanged();
            } else {
                int distTop=top-touchOct;
                int distBottom=touchOct-bottom;
                if(distTop>distBottom) {
                    bottom=touchOct;
                    emit bottomChanged();
                }
                if(distTop<distBottom) {
                    top=touchOct;
                    emit topChanged();
                }
            }
        }
        emit setOctConf(bottom,top);
        int midnew=bottom+(top-bottom)/2;
        if(midnew!=mid) {
            mid=midnew;
            emit setOctMid(mid);
        }
    }
}

void MWOctaveRanger::onReleased(int id)
{
    if(grabedTopTouchId == id) {
        grabedTop = false;
    } else if(grabedBot && grabedBotTouchId == id) {
        grabedBot = false;
    }
}

void MWOctaveRanger::onResize(int w)
{
    segwidth = w / noct;
}

void MWOctaveRanger::octUp()
{
    if(top<9) {
        top++;
        mid++;
        bottom++;
        emit setOctConf(bottom,top);
        emit setOctMid(mid);
        emit topChanged();
        emit bottomChanged();
    }
}

void MWOctaveRanger::octDown()
{
    if(bottom>0) {
        top--;
        mid--;
        bottom--;
        emit setOctConf(bottom,top);
        emit setOctMid(mid);
        emit topChanged();
        emit bottomChanged();
    }
}

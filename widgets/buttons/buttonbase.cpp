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

#include "buttonbase.h"

ButtonBase::ButtonBase(QString text, QObject *parent) : QObject(parent),
    _state(0),
    _pressed(0),
    _text(text)
{
}

ButtonBase::ButtonBase(QString text, int state, QObject *parent): QObject(parent),
    _state(state),
    _pressed(0),
    _text(text)
{
}

int ButtonBase::getState()
{
    return _state;
}

bool ButtonBase::selected()
{
    if(_state>0 || _pressed>0) {
        return true;
    } else {
        return false;
    }
}

void ButtonBase::onPressed()
{
    _pressed=1;
    pressAction();
    emit selectedChanged();
}

void ButtonBase::onReleased()
{
    _pressed=0;
    releaseAction();
    emit selectedChanged();
}

void ButtonBase::pressAction()
{

}

void ButtonBase::releaseAction()
{

}

void ButtonBase::setAction()
{

}

void ButtonBase::setState(int s)
{
    _state = s;
    setAction();
    emit selectedChanged();
}

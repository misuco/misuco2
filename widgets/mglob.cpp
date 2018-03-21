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

#include "mglob.h"
#include <QEvent>
#include <QTouchEvent>
#include <QDateTime>
#include <QDebug>

QColor MGlob::fgcolor(QColor::fromRgb(200,200,200));
QColor MGlob::bgcolor(QColor::fromRgb(25,25,25));
QColor MGlob::highlightcolor("Red");
QString MGlob::font1("Sans");
int MGlob::font1size(10);

bool MGlob::bwmode = false;
QColor MGlob::bkeycolor("Black");
QColor MGlob::wkeycolor("White");
QColor MGlob::hlbkeycolor(QColor::fromHsl(1,200,20));
QColor MGlob::hlwkeycolor(QColor::fromHsl(1,200,200));

float MGlob::lOff = 0.4;
float MGlob::lOn = 0.8;
float MGlob::sOff = 0.8;
float MGlob::sOn = 0.8;

Pitch * MGlob::MWPitch[BSCALE_SIZE+1];
MWScale MGlob::Scale;

MWSound MGlob::Sound;
MWMicrotune MGlob::Microtune;

int MGlob::noteSymbols = 0;
bool MGlob::showFreqs = false;

int MGlob::channel = 0;

bool MGlob::sendCC1 = true;
QObject * MGlob::overwritePreset = 0;

int MGlob::playAreaWidth = 0;
int MGlob::playAreaHeight = 0;
int MGlob::playFieldWidth = 0;
int MGlob::playFieldHeight = 0;

MGlob::MGlob(QObject *parent) : QObject(parent)
{
}

MGlob::~MGlob()
{
}

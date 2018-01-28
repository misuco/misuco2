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

#include "misuwidget.h"
#include <QEvent>
#include <QTouchEvent>
#include <QDateTime>
#include <QDebug>

QColor MisuWidget::fgcolor(QColor::fromRgb(200,200,200));
QColor MisuWidget::bgcolor(QColor::fromRgb(25,25,25));
QColor MisuWidget::highlightcolor("Red");
QString MisuWidget::font1("Sans");
int MisuWidget::font1size(10);

bool MisuWidget::bwmode = false;
QColor MisuWidget::bkeycolor("Black");
QColor MisuWidget::wkeycolor("White");
QColor MisuWidget::hlbkeycolor(QColor::fromHsl(1,200,20));
QColor MisuWidget::hlwkeycolor(QColor::fromHsl(1,200,200));

float MisuWidget::lOff = 0.4;
float MisuWidget::lOn = 0.8;
float MisuWidget::sOff = 0.8;
float MisuWidget::sOn = 0.8;

Pitch * MisuWidget::MWPitch[BSCALE_SIZE+1];
MWScale MisuWidget::Scale;
MWSound MisuWidget::Sound;
MWMicrotune MisuWidget::Microtune;

int MisuWidget::noteSymbols = 0;
bool MisuWidget::showFreqs = false;

int MisuWidget::channel = 0;

bool MisuWidget::sendCC1 = true;
bool MisuWidget::holdMode = false;
QObject * MisuWidget::overwritePreset = 0;

MisuWidget::MisuWidget(QObject *parent) : QObject(parent)
{
}

MisuWidget::~MisuWidget()
{
}

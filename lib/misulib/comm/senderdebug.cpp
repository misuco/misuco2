/*

Copyright (C) 2013 by Claudio Zopfi, Zurich, Suisse, z@x21.ch

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/
#include "senderdebug.h"

#include <QDebug>

SenderDebug::SenderDebug()
{
}

void SenderDebug::cc(int voiceId, int cc, float v1, float v1avg)
{
    qDebug() << "SenderDebug::cc(" << voiceId << "," << cc << "," << v1 << "," << v1avg << ")";
}

void SenderDebug::pc(int v1)
{
    qDebug() << "SenderDebug::pc(" << v1 << ")";
}

void SenderDebug::noteOn(int voiceId, float f, int midinote, int pitch, int v)
{
    qDebug() << "SenderDebug::noteOn(" << voiceId << "," << f << "," << v << "," << midinote << "," << pitch << ")";
}

void SenderDebug::noteOff(int voiceId)
{
    qDebug() << "SenderDebug::noteOff(" << voiceId << ")";
}

void SenderDebug::pitch(int voiceId, float v1, int midinote, int pitch)
{
    qDebug() << "SenderDebug::pitch(" << voiceId << "," << v1  << "," << midinote << "," << pitch << ")";
}

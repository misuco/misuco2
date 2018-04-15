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
#ifndef SENDERDEBUG_H
#define SENDERDEBUG_H

#include "isender.h"

class SenderDebug : public ISender
{
public:
    SenderDebug();
    virtual void cc(int voiceId, int cc, float v1, float v1avg);
    virtual void pc(int v1);
    virtual void noteOn(int voiceId, float f, int midinote, int pitch, int v);
    virtual void noteOff(int voiceId);
    virtual void pitch(int voiceId, float f, int midinote, int pitch);
    virtual void setDestination(char *,int) {}
    virtual void reconnect() {}
};

#endif // SENDERDEBUG_H

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
#ifndef ISENDER_H
#define ISENDER_H

class ISender
{
public:
    virtual ~ISender() {}
    virtual void cc(int voiceId, int cc, float v1, float v1avg)=0;
    virtual void pc(int v1)=0;
    virtual void noteOn(int voiceId, float f, int midinote, int pitch, int v)=0;
    virtual void noteOff(int voiceId)=0;
    virtual void pitch(int voiceId, float f, int midinote, int pitch)=0;
    virtual void setDestination(char * adr, int port)=0;
    virtual void reconnect()=0;
};

#endif // ISENDER_H

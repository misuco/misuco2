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
#ifndef SENDEROSCPUREDATA_H
#define SENDEROSCPUREDATA_H

#include "isender.h"
#include "comm/libofqf/qoscclientinterface.h"

class SenderOscPuredata : public ISender
{
public:
    SenderOscPuredata();
    ~SenderOscPuredata();
    virtual void cc(int voiceId, int cc, float, float v1avg);
    virtual void pc(int v1);
    virtual void noteOn(int voiceId, float f, int midinote, int pitch, int scalenote, int vel);
    virtual void noteOff(int voiceId, int midinote);
    virtual void pitch(int, float fr, int, int pitch, int scalenote);
    virtual void setDestination(char * a,int p);
    virtual void reconnect();
    virtual int getPort() {return port;}
    virtual char* getAddress() {return adr;}

private:
    QOscClientInterface* oscout;
    char * adr;
    int port;
    int prog;              // current program
    //quint8 * notestate;   // currently played notes
    int * ccstate;       // current ccval;
    void sendOsc(QString path, QVariant list);
};

#endif // SENDEROSCPUREDATA_H

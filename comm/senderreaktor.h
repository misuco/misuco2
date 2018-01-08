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

#ifndef SENDERREAKTOR_H
#define SENDERREAKTOR_H

#include "isender.h"
#include "../comm/libofqf/qoscclientinterface.h"

class SenderReaktor : public ISender
{
public:
    SenderReaktor();
    ~SenderReaktor();
    virtual void cc(int chan, int voiceId, int cc, float, float v1avg);
    virtual void pc(int chan, int v1);
    virtual int noteOn(int chan, float f, int midinote, int pitch, int v);
    virtual void noteOn(int chan, int voiceId, float f, int midinote, int pitch, int vel);
    virtual void noteOff(int voiceId);
    virtual void pitch(int chan, int voiceId, float f, int midinote, int pitch);
    virtual void setDestination(char * a,int p);
    virtual void reconnect();
    virtual int getPort() {return port;}
    virtual char* getAddress() {return adr;}
    virtual bool voiceBased() {return false;}

private:
    QOscClientInterface* oscout;
    char * adr;
    int port;
    int prog;              // current program
    quint8 * notestate;   // currently played notes
    quint8 * notechan;    // currently played notes chan
    int * ccstate;       // current ccval;
    void sendOsc(QString path, QVariant list);
};

#endif // SENDERREAKTOR_H

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

#ifndef SENDERMULTI_H
#define SENDERMULTI_H

#include <QList>
#include "isender.h"
#include "../comm/libofqf/qoscclient.h"
#include "sendermobilesynth.h"
//#include "sendersfmidi.h"
//#include "sequencer.h"

class SenderMulti : public ISender
{
public:
    SenderMulti();
    ~SenderMulti();
    virtual void cc(int chan, int nextVoiceId, int cc, float v1, float v1avg);
    virtual void pc(int chan, int v1);
    virtual int noteOn(int chan, float f, int midinote, int pitch, int v);
    virtual void noteOn(int chan, int voiceId, float f, int midinote, int pitch, int v);
    virtual void noteOff(int voiceId);
    virtual void pitch(int chan, int voiceId, float f, int midinote, int pitch);

    virtual void setDestination(char * a,int p);
    virtual void reconnect();
    virtual bool voiceBased() {return true;}

    bool senderEnabled[4];

    //void sync(int mode, int bar, float bpm);

    //void setSeq(Sequencer * s) {this->seq=s;}

    void setDestination(int i, char * a,int p);
    /*

    void sendOff();
    
    int repeatOff;
    
    struct offRepeat {
        int voiceId;
        int midinote;
        int chan;
        int offSent;
    };

    QHostAddress getIP() {
        return syncout->getIP();
    }
    */

private:
    QList<ISender *> senders;
    //QList<offRepeat *> offToRepeat;
    //QOscClient* syncout;

    int nextVoiceId;

    bool midiOn[256];
    quint8 * notestate;   // currently played notes
    int onCnt;
    //void createOTR(int chan, int nextVoiceId, int midinote);
};

#endif // SENDERMULTI_H

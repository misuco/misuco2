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

#include "sendermulti.h"
#include "senderoscmidigeneric.h"
#include "sendersupercollider.h"
#include "senderreaktor.h"
#include "sendermobilesynth.h"
#include "qoscclient.h"

SenderMulti::SenderMulti()
{
    senders.append(new SenderMobileSynth());
    senders.append(new SenderOscMidiGeneric());
    senders.append(new SenderReaktor());
    senders.append(new SenderSuperCollider());
    //syncout=new QOscClient();

    senderEnabled[0] = true;
    senderEnabled[1] = false;
    senderEnabled[2] = true;
    senderEnabled[3] = false;

    nextVoiceId=1;

    //repeatOff=0;
    onCnt=0;
    for(int i=0;i<256;i++) {
        midiOn[i]=false;
    }
    notestate=new quint8[1024];
    for(int i=0;i<1024;i++) {
        notestate[i]=0;
    }
}

SenderMulti::~SenderMulti()
{
    //delAll();
}

void SenderMulti::cc(int chan, int voiceId, int cc, float v1, float v1avg)
{
    //qDebug() << "SenderMulti::cc(" << chan << "," << voiceId << "," << cc << "," << v1 << "," << v1avg << ")";
    for(int i=0;i<senders.count();i++) {
        if(senderEnabled[i]) senders.at(i)->cc(chan,voiceId,cc,v1,v1avg);
    }
}

void SenderMulti::pc(int chan, int v1)
{
    for(int i=0;i<senders.count();i++) {
        if(senderEnabled[i]) senders.at(i)->pc(chan,v1);
    }
}

int SenderMulti::noteOn(int chan, float f, int midinote, int pitch, int v)
{
    int vid=nextVoiceId++;
    if(nextVoiceId>1023)  nextVoiceId=1;
    //notestate[vid]=midinote;
    for(int i=0;i<senders.count();i++) {
        if(senderEnabled[i]) {
            senders.at(i)->noteOn(chan,vid,f,midinote,pitch,v);
            //qDebug() << "SenderMulti::noteOn(sender:" << i << ",chan:" << chan << ",vid:" << vid << ",f:" << f << ",midinote:" << midinote << "," << pitch << "," << v << ")";
        }
    }
    onCnt++;
    //midiOn[midinote]=true;

    return vid;
}

void SenderMulti::noteOn(int chan, int voiceId, float f, int midinote, int pitch, int v)
{

}

void SenderMulti::noteOff(int voiceId)
{
    //qDebug() << "SenderMulti::noteOff(" <<  voiceId  << ")";
    //int vid=voiceId%1024;
    //int midinote=notestate[vid];
    for(int i=0;i<senders.count();i++) {
        if(senderEnabled[i]) senders.at(i)->noteOff(voiceId);
    }
    //createOTR(chan, voiceId, midinote);
    //midiOn[midinote]=false;
    onCnt--;
}

void SenderMulti::pitch(int chan, int voiceId, float f, int midinote, int pitch)
{
    //qDebug() << "SenderMulti::pitch(" << chan << "," << voiceId << "," << f << "," << midinote << "," << pitch << ")";
    for(int i=0;i<senders.count();i++) {
        if(senderEnabled[i]) senders.at(i)->pitch(chan,voiceId,f,midinote,pitch);
        /*
        if(senders.at(i)->voiceBased()==true) {
            senders.at(i)->pitch(chan,voiceId,f,midinote,pitch);
        } else {
            int vid=voiceId%1024;
            if(notestate[vid]!=midinote) {
                senders.at(i)->noteOff(voiceId);
                createOTR(chan, voiceId, notestate[vid]);
                senders.at(i)->noteOn(chan,f,midinote,pitch,127);
                midiOn[notestate[vid]]=false;
                notestate[vid]=midinote;
                midiOn[midinote]=true;
            } else {
                senders.at(i)->pitch(chan,voiceId,f,midinote,pitch);
            }
        }
        */
    }
}

void SenderMulti::setDestination(char * a, int p)
{
    for(int i=0;i<senders.count();i++) {
        senders.at(i)->setDestination(a,p);
    }
    //syncout->setAddress(a,3333);
}


/*

void SenderMulti::del(int i) {
    if(i<senders.count()) {
        delete(senders.at(i));
        senders.removeAt(i);
    }
}

void SenderMulti::delAll(){
    for(int i=0;i<senders.count();i++) {
        delete(senders.at(i));
    }
    senders.clear();
}

void SenderMulti::createOTR(int chan, int voiceId, int midinote)
{
    offRepeat * otr=new offRepeat;
    otr->voiceId=voiceId;
    otr->midinote=midinote;
    otr->chan=chan;
    otr->offSent=1;
    offToRepeat.append(otr);
}

*/

void SenderMulti::setDestination(int i, char * a,int p) {
    if(i<senders.count()) {
        senders.at(i)->setDestination(a,p);
    }
}

void SenderMulti::reconnect() {
    for(int i=0;i<senders.count();i++) {
        senders.at(i)->reconnect();
        //qDebug() << "reconnect " << i;
    }
}
/*
int SenderMulti::getPort()
{
    if(senders.count()>0)
        return senders.at(0)->getPort();
    else
        return 0;
}

char *SenderMulti::getAddress()
{
    if(senders.count()>0)
        return senders.at(0)->getAddress();
    else
        return 0;
}

void SenderMulti::sendOff() {
    for(int j=offToRepeat.count();j>0;j--) {
        int i=j-1;
        if(offToRepeat.at(i)->offSent>=repeatOff) {
            delete(offToRepeat.at(i));
            offToRepeat.removeAt(i);
        } else {
            for(int j=0;j<senders.count();j++) {
                if(senders.at(j)->voiceBased()==true || midiOn[offToRepeat.at(i)->midinote]==false) {
                    senders.at(j)->noteOff( offToRepeat.at(i)->voiceId );
                }
            }
            //qDebug() << "repeating off " << offToRepeat.at(i)->midinote << " for " << offToRepeat.at(i)->offSent;
            offToRepeat.at(i)->offSent++;
        }
    }
}

*/

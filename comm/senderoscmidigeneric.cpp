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
#include "senderoscmidigeneric.h"
#include "../comm/libofqf/qoscclient.h"

SenderOscMidiGeneric::SenderOscMidiGeneric()
{
    adr=new char[16];
    strcpy(adr,"255.255.255.255");
    port=3150;
    oscout=new QOscClient();
    oscout->setAddress(adr,port);
    ccstate=new int[1024];
    for(int i=0;i<1024;i++) {
        ccstate[i]=0;
    }
    prog=0;
}

SenderOscMidiGeneric::~SenderOscMidiGeneric()
{
    delete(ccstate);
    delete(oscout);
}

int SenderOscMidiGeneric::noteOn(int chan, float f, int midinote, int pitch, int val)
{
    //qDebug() << "SenderOscMidiGeneric::noteOn " << chan << " " << voiceId << " " << midinote << " " << pitch;

}

void SenderOscMidiGeneric::noteOn(int chan, int voiceId, float f, int midinote, int pitch, int val)
{
    QVariantList v;
    v.append(midinote);
    v.append(val);
    v.append(chan);
    QString path;
    path.sprintf("/note/%d",chan);
    sendOsc(path,v);

    v.clear();
    v.append(pitch);
    v.append(chan);
    path.sprintf("/pitch/%d",chan);
    sendOsc(path,v);
}

void SenderOscMidiGeneric::noteOff(int)
{
    QVariantList v;
    QString path;
    //v.append(notestate[voiceId%1024]);

    //v.append(midinote);
    //v.append(0);
    //v.append(chan);
    //path.sprintf("/note/%d",chan);
    sendOsc(path,v);
}

void SenderOscMidiGeneric::pitch(int chan, int voiceId, float f, int midinote, int pitch)
{
    //qDebug() << "SenderOscMidiGeneric::pitch " << chan << " " << voiceId << " " << midinote << " " << pitch;
    QVariantList v;
    QString path;
    v.append(pitch);
    v.append(chan);
    path.sprintf("/pitch/%d",chan);
    sendOsc(path,v);
}

void SenderOscMidiGeneric::setDestination(char * a, int p)
{
    delete[] adr;
    adr=new char[strlen(a)];
    strcpy(adr,a);
    port=p;
    oscout->setAddress(adr,p);
}

void SenderOscMidiGeneric::reconnect()
{
    delete(oscout);
    oscout=new QOscClient();
    oscout->setAddress(adr,port);
}

void SenderOscMidiGeneric::pc(int chan, int v1)
{
    QVariantList v;
    QString path;
    v.append(v1);
    v.append(chan);
    path.sprintf("/pc/%d",chan);
    sendOsc(path,v);
}

void SenderOscMidiGeneric::cc(int chan, int, int cc, float, float v1avg)
{
    //qDebug() <<  "SenderOscPuredata::cc " << cc << " v1 " << v1;

    // translate value to midi
    int v1mid=(float)127*v1avg;

    // translate cc numbers
    if(v1mid!=ccstate[cc]) {
        ccstate[cc]=v1mid;
        QVariantList v;
        QString path;
        v.append(v1mid);
        v.append(cc);
        v.append(chan);
        path.sprintf("/cc/%d/%d",chan,cc);
        sendOsc(path,v);
    }
}

void SenderOscMidiGeneric::sendOsc(QString path, QVariant list)
{
    //qDebug() << " sendOsc to " << path << " values " << list;
    oscout->sendData(path,list);
}

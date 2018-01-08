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
#include "senderoscpuredata.h"
#include "../comm/libofqf/qoscclient.h"

SenderOscPuredata::SenderOscPuredata()
{
    adr=new char[16];
    strcpy(adr,"255.255.255.255");
    port=3334;
    oscout=new QOscClient();
    oscout->setAddress(adr,port);
    //notestate=new quint8[1024];
    ccstate=new int[1024];
    for(int i=0;i<1024;i++) {
        //notestate[i]=0;
        ccstate[i]=0;
    }
    prog=0;
}

SenderOscPuredata::~SenderOscPuredata()
{
    delete(ccstate);
    delete(oscout);
}

void SenderOscPuredata::noteOn(int chan, int voiceId, float, int midinote, int pitch, int scalenote, int)
{
    //int f = midinote;
    //int vid=voiceId%1024;
    //notestate[vid]=f;
    QVariantList v;
    v.append(chan);
    v.append(midinote);
    v.append(127);
    sendOsc("/note",v);

    v.clear();
    v.append(chan);
    v.append(pitch);
    sendOsc("/pitch",v);
}

void SenderOscPuredata::noteOff(int chan, int, int midinote)
{
    QVariantList v;
    v.append(chan);
    //v.append(notestate[voiceId%1024]);
    v.append(midinote);
    v.append(0);
    sendOsc("/note",v);
}

void SenderOscPuredata::pitch(int chan, int, float, int, int pitch, int scalenote)
{
    QVariantList v;
    v.append(chan);
    v.append(pitch);
    sendOsc("/pitch",v);
}

void SenderOscPuredata::setDestination(char * a, int p)
{
    delete[] adr;
    adr=new char[strlen(a)];
    strcpy(adr,a);
    port=p;
    oscout->setAddress(adr,p);
}

void SenderOscPuredata::reconnect()
{
    delete(oscout);
    oscout=new QOscClient();
    oscout->setAddress(adr,port);
}

void SenderOscPuredata::pc(int chan, int v1)
{
    QVariantList v;
    v.append(chan);
    v.append(v1);
    sendOsc("/pc",v);
}

void SenderOscPuredata::cc(int chan, int, int cc, float, float v1avg)
{
    //qDebug() <<  "SenderOscPuredata::cc " << cc << " v1 " << v1;

    // translate value to midi
    int v1mid=(float)127*v1avg;

    // translate cc numbers
    if(v1mid!=ccstate[cc]) {
        ccstate[cc]=v1mid;
        QVariantList v;
        v.append(chan);
        v.append(cc);
        v.append(v1mid);
        sendOsc("/cc",v);
    }

    /*
    if(cc==1) x=v1*127.0f;
    if(cc==2) y=v1*127.0f;

    QVariantList v;
    v.append(x);
    v.append(y);
    sendOsc("/xy",v);
    */
}

void SenderOscPuredata::sendOsc(QString path, QVariant list)
{
//    qDebug() << " sendOsc to " << path << " values " << list;
    oscout->sendData(path,list);
    //rc1->getEvstat()->incOsccount();
}

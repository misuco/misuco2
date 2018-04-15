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
#include "senderreaktor.h"
#include "lib/misulib/comm/libofqf/qoscclient.h"

SenderReaktor::SenderReaktor()
{
    adr=new char[16];
    strcpy(adr,"255.255.255.255");
    port=3150;
    oscout=new QOscClient();
    oscout->setAddress(adr,port);
    notestate=new quint8[1024];
    notechan=new quint8[1024];
    ccstate=new int[1024];
    for(int i=0;i<1024;i++) {
        notestate[i]=0;
        notechan[i]=0;
        ccstate[i]=0;
    }
    prog=0;
}

SenderReaktor::~SenderReaktor()
{
    delete(ccstate);
    delete(oscout);
}

int SenderReaktor::noteOn(float, int, int, int)
{
    return 0;
}

void SenderReaktor::noteOn(int voiceId, float, int midinote, int pitch, int vel)
{
    QVariantList v;
    v.append(midinote);
    v.append(vel);
    QString path;
    path.sprintf("/note/%d",_channel);
    sendOsc(path,v);
    v.clear();
    v.append(pitch);
    path.sprintf("/pitch/%d",_channel);
    sendOsc(path,v);
    notechan[voiceId%1024]=_channel;
    notestate[voiceId%1024]=midinote;
}

void SenderReaktor::noteOff(int voiceId)
{
    QVariantList v;
    QString path;
    v.append(notestate[voiceId%1024]);
    v.append(0);
    path.sprintf("/note/%d",notechan[voiceId%1024]);
    sendOsc(path,v);
}

void SenderReaktor::pitch(int, float, int, int pitch)
{
    QVariantList v;
    QString path;

    v.append(pitch*64);
    path.sprintf("/pitch/%d",_channel);
    sendOsc(path,v);
}

void SenderReaktor::setDestination(char * a, int p)
{
    delete[] adr;
    adr=new char[strlen(a)];
    strcpy(adr,a);
    port=p;
    oscout->setAddress(adr,p);
}

void SenderReaktor::reconnect()
{
    delete(oscout);
    oscout=new QOscClient();
    oscout->setAddress(adr,port);
}

void SenderReaktor::setChannel(int c)
{
    _channel = c;
}

void SenderReaktor::pc(int v1)
{
    QVariantList v;
    QString path;
    v.append(v1);
    path.sprintf("/pc/%d",_channel);
    sendOsc(path,v);
}

void SenderReaktor::cc(int, int cc, float, float v1avg)
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
        path.sprintf("/cc/%d/%d",_channel,cc);
        sendOsc(path,v);
    }
}

void SenderReaktor::sendOsc(QString path, QVariant list)
{
    //qDebug() << " sendOsc to " << path << " values " << list;
    oscout->sendData(path,list);
}

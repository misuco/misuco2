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

#include <QVariantList>
#include "sendersupercollider.h"
#include "../comm/libofqf/qoscclient.h"

SenderSuperCollider::SenderSuperCollider()
{
    adr=new char[16];
    sy="m0";
    strcpy(adr,"255.255.255.255");
    port=57110;
    oscout=new QOscClient();
    oscout->setAddress(adr,port);
}

SenderSuperCollider::~SenderSuperCollider()
{
    delete(oscout);
}

void SenderSuperCollider::cc(int voiceId, int cc, float v1, float)
{
    QVariantList v;
    QString p;
    p.sprintf("par%d",cc);
    v.append(voiceId);
    v.append(p);
    v.append(v1);
    sendOsc("/n_set",v);
}

void SenderSuperCollider::pc(int v1)
{
    sy.sprintf("m%d",v1);
}

void SenderSuperCollider::setDestination(char * a, int p)
{
    delete[] adr;
    adr=new char[strlen(a)];
    strcpy(adr,a);
    port=p;
    oscout->setAddress(adr,p);
}

void SenderSuperCollider::reconnect()
{
    delete(oscout);
    oscout=new QOscClient();
    oscout->setAddress(adr,port);
}

int SenderSuperCollider::noteOn(float, int, int, int)
{
    return 0;
}

void SenderSuperCollider::noteOn(int voiceId, float f, int, int, int vel)
{
    //qDebug() << "noteOn vid:" << voiceId << " f " << f << " vel " << vel;
    QVariantList v;
    v.append(sy);
    v.append(voiceId);
    v.append(0);
    v.append(0);
    v.append("freq");
    v.append(f);
    v.append("amp");
    v.append(vel);
    sendOsc("/s_new",v);
}

void SenderSuperCollider::pitch(int voiceId, float f, int, int)
{
    //qDebug() << "pitch vid:" << voiceId << " f " << f;
    QVariantList v;
    v.append(voiceId);
    v.append("freq");
    v.append(f);
    sendOsc("/n_set",v);
}

void SenderSuperCollider::sendOsc(QString path, QVariant list)
{
    //qDebug() << " sendOsc to " << path << " values " << list;
    oscout->sendData(path,list);
}


void SenderSuperCollider::noteOff(int voiceId)
{
    //qDebug() << "note off vid:" << voiceId ;
    QVariantList v;
    v.append(voiceId);
    v.append("amp");
    v.append(0);
    sendOsc("/n_set",v);
}

/*

Copyright (C) 2014 by Claudio Zopfi, Zurich, Suisse, z@x21.ch

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

#include "senderoscxy.h"
#include "../comm/libofqf/qoscclient.h"

SenderOscXY::SenderOscXY()
{
    adr=new char[16];
    strcpy(adr,"255.255.255.255");
    port=3150;
    oscout=new QOscClient();
    oscout->setAddress(adr,port);
    x=0;
    y=0;
}

SenderOscXY::~SenderOscXY()
{
    delete(oscout);
}

void SenderOscXY::noteOn(int, int, float, int, int, int scalenote, int)
{
}

void SenderOscXY::setDestination(char * a, int p)
{
    delete[] adr;
    adr=new char[strlen(a)];
    strcpy(adr,a);
    port=p;
    oscout->setAddress(adr,p);
}

void SenderOscXY::reconnect()
{
    delete(oscout);
    oscout=new QOscClient();
    oscout->setAddress(adr,port);
}

void SenderOscXY::pc(int, int)
{

}

void SenderOscXY::cc(int, int, int cc, float v1, float)
{

    if(cc==1) x=v1;
    if(cc==2) y=v1;

    QVariantList v;
    v.append(x);
    v.append(y);
    sendOsc("/xy",v);
}

void SenderOscXY::sendOsc(QString path, QVariant list)
{
//    qDebug() << " sendOsc to " << path << " values " << list;
    oscout->sendData(path,list);
}

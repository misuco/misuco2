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

#include <QObject>
#include <QList>
#include "lib/misulib/comm/libofqf/qoscclient.h"
#include "sendermobilesynth.h"

class MasterSender : public QObject
{
public:
    explicit MasterSender(QObject * parent = nullptr);
    ~MasterSender();
    void cc(int nextVoiceId, int cc, float v1, float v1avg);
    void pc(int v1);
    int noteOn(float f, int midinote, int pitch, int v);
    void noteOff(int voiceId);
    void pitch(int voiceId, float f, int midinote, int pitch);

    void setDestination(char * a,int p);
    void reconnect();

    void setDestination(int i, char * a,int p);
    void addSender(ISender * s);

public slots:
    void onToggleSender(int i, bool value);

private:
    QList<ISender *> senders;
    QList<bool> senderEnabled;

    int nextVoiceId;

    bool midiOn[256];
    quint8 * notestate;   // currently played notes
    int onCnt;
};

#endif // SENDERMULTI_H

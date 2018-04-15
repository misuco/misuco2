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

#ifndef SENDERMOBILESYNTH_H
#define SENDERMOBILESYNTH_H

#include <QtGlobal>
#include "isender.h"
#ifdef Q_OS_IOS
#include "lib/mobilesynth/mobilesynthviewcontrollerrc1.hpp"
#include "lib/mobilesynth/synth/controller.h"
#else
#include "lib/mobilesynth/mobilesynthqt52.h"
#endif

class SenderMobileSynth : public ISender
{
public:
    SenderMobileSynth();
    ~SenderMobileSynth();
    virtual void cc(int voiceId, int cc, float v1, float);
    virtual void pc(int);
    virtual void noteOn(int voiceId, float f, int midinote, int pitch, int v);
    virtual void noteOff(int voiceId);
    virtual void pitch(int voiceId, float f, int, int);
    virtual void setDestination(char * a,int p);
    virtual void reconnect() {}
    virtual int getPort() {return 0;}
    virtual char* getAddress() {return 0;}
    synth::Controller * getSynthController() {
#ifdef Q_OS_IOS
        return sy;
#else
        return sy->getSyctl();
#endif
    }
    
private:
#ifdef Q_OS_IOS
    mobilesynthview::Widget * syco;
    synth::Controller * sy;
#else
    mobileSynthQT52 * sy;
#endif
};

#endif // SENDERMOBILESYNTH_H

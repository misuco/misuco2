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
#include <QDebug>
#include "conf/platform.h"
#include "sendermobilesynth.h"

#ifdef RC1_IOS

SenderMobileSynth::SenderMobileSynth()
{
    
    syco=new mobilesynthview::Widget();
    sy=new synth::Controller();
    syco->setController(sy);
}

SenderMobileSynth::~SenderMobileSynth()
{
    delete(syco);
    delete(sy);
}

void SenderMobileSynth::cc(int, int voiceId, int cc, float v1, float)
{
    if(cc==1) {
        sy->set_modulation(voiceId,v1);
    } else if(cc==102) {
        sy->set_osc1_wave_type_int(v1*4.0f);
    } else if(cc==103) {
        sy->setAttack(0, v1*90000.0f);
    } else if(cc==104) {
        sy->setDecay(0, v1*90000.0f);
    } else if(cc==105) {
        sy->setSustain(0, v1);
    } else if(cc==106) {
        sy->setRelease(0, v1*90000.0f);
    } else if(cc==107) {
        sy->set_filter_cutoff(v1);
    } else if(cc==108) {
        sy->set_filter_resonance(v1*4.0f);
    } else if(cc==109) {
        sy->set_mod_filter_cutoff(v1);
    } else if(cc==110) {
        sy->set_mod_filter_resonance(v1);
    } else if(cc==111) {
        sy->set_volume(v1);
    }
}

void SenderMobileSynth::pc(int, int) {}

void SenderMobileSynth::noteOn(int, int voiceId, float f, int, int, int, int)
{
    sy->NoteOn(voiceId, (float)f);
}

void SenderMobileSynth::noteOff(int,int voiceId, int)
{
    sy->NoteOff(voiceId);
}

void SenderMobileSynth::pitch(int, int voiceId, float f, int, int, int) {
    sy->NoteOn(voiceId, (float)f);
}

void SenderMobileSynth::setDestination(char *,int)
{

}

#else

SenderMobileSynth::SenderMobileSynth()
{
    sy= new mobileSynthQT52();
    //qDebug() << "mobileSynthQt52 created";
}

SenderMobileSynth::~SenderMobileSynth()
{
    delete(sy);
}

void SenderMobileSynth::cc(int chan, int voiceId, int cc, float v1, float)
{
    if(cc==1) {
        sy->getSyctl()->set_modulation(voiceId,v1);
    } else if(cc==102) {
        sy->getSyctl()->set_osc1_wave_type_int(v1*4.0f);
    } else if(cc==103) {
        sy->getSyctl()->setAttack(0, v1*90000.0f);
    } else if(cc==104) {
        sy->getSyctl()->setDecay(0, v1*90000.0f);
    } else if(cc==105) {
        sy->getSyctl()->setSustain(0, v1);
    } else if(cc==106) {
        sy->getSyctl()->setRelease(0, v1*90000.0f);
    } else if(cc==107) {
        sy->getSyctl()->set_filter_cutoff(v1);
    } else if(cc==108) {
        sy->getSyctl()->set_filter_resonance(v1*4.0f);
    } else if(cc==109) {
        sy->getSyctl()->set_mod_filter_cutoff(v1);
    } else if(cc==110) {
        sy->getSyctl()->set_mod_filter_resonance(v1);
    } else if(cc==111) {
        sy->getSyctl()->set_volume(v1);
    }
}

void SenderMobileSynth::pc(int chan, int v1) {}

int SenderMobileSynth::noteOn(int, float f, int, int, int)
{
    int voiceId=nextVid++;
    qDebug() << "SenderMobileSynth::noteOn " << voiceId << " f: " << f;
    sy->noteOn(voiceId, (float)f);
    return voiceId;
}

void SenderMobileSynth::noteOff(int voiceId)
{
    qDebug() << "SenderMobileSynth::noteOff " << voiceId;
    sy->noteOff(voiceId);
}

void SenderMobileSynth::pitch(int chan, int voiceId, float f, int midinote, int pitch) {
    sy->noteOn(voiceId, (float)f);
}

void SenderMobileSynth::setDestination(char *, int) {}

#endif

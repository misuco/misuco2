// controller.cpp
// Author: Allen Porter <allen@thebends.org>

#include "controller.h"

#include <math.h>
#include <assert.h>
#include "envelope.h"
#include "filter.h"
#include "modulation.h"
#include "oscillator.h"

#include <qendian.h>

namespace synth {
    
    Controller::Controller()
    {
        channelBytes=0;
        volume_=0.6;
    }

    void Controller::set_volume(float volume)
    {
        volume_=volume;
    }
    void Controller::set_sample_rate(float sample_rate) {
        sample_rate_=sample_rate;
        key_stack_.SetSampleRate(sample_rate);
    }
    
    void Controller::NoteOn(int note, float freq) {
        key_stack_.NoteOn(note, freq);
    }
    
    void Controller::NoteOff(int note) {
        key_stack_.NoteOff(note);
    }
    
    void Controller::set_osc_pw(int voice, float p) {
        key_stack_.setOscPW(voice, p);
    }
    
    void Controller::set_osc1_wave_type_int(int w) {
        key_stack_.setOscWave(w);
    }

    void Controller::set_filter_cutoff(float frequency) {
        key_stack_.setFilterCutoff(frequency);
    }

    void Controller::set_mod_filter_cutoff(float v)
    {
        key_stack_.setModCutoff(v);
    }

    void Controller::set_mod_filter_resonance(float v)
    {
        key_stack_.setModRes(v);
    }

    void Controller::set_modulation(int voice, float v)
    {
        key_stack_.setModulation(voice,v);
    }
    
    void Controller::set_filter_resonance(float value) {
        key_stack_.setFilterRes(value);
    }
        
    void Controller::setADSR(int n, long a, long d, float s, long r) {
        key_stack_.setADSR(n,a,d,s,r);
    }

    void Controller::setAttack(int n, long a) {
        key_stack_.setAttack(n,a);
    }

    void Controller::setDecay(int n, long d) {
        key_stack_.setDecay(n,d);
    }

    void Controller::setSustain(int n, float s) {
        key_stack_.setSustain(n,s);
    }

    void Controller::setRelease(int n, int r) {
        key_stack_.setRelease(n,r);
    }

    void Controller::decVoices()
    {
        if(key_stack_.kMaxSize>4) {
            key_stack_.kMaxSize--;
        }
    }

    void Controller::incVoices()
    {
        if(key_stack_.kMaxSize<10) {
            key_stack_.kMaxSize++;
        }
    }

    void Controller::GetCharSamples(char* buffer, int size) {
        
        if(channelBytes>0) {
            unsigned char *ptr = reinterpret_cast<unsigned char *>(buffer);
            //int sampleBytes = channelCount * channelBytes;

            while (size>0) {
                qreal x=GetSample();
                for (int i=0; i<channelCount; ++i) {
                    if (channelBytes == 1 && sampleType == 0 /* QAudioFormat::UnSignedInt */) {
                        const quint8 value = static_cast<quint8>((1.0 + x) / 2 * 255);
                        *reinterpret_cast<quint8*>(ptr) = value;
                    } else if (channelBytes == 1  && sampleType == 1 /* QAudioFormat::SignedInt */) {
                        const qint8 value = static_cast<qint8>(x * 127);
                        *reinterpret_cast<quint8*>(ptr) = value;
                    } else if (channelBytes == 2 && sampleType == 0 /* QAudioFormat::UnSignedInt */) {
                        quint16 value = static_cast<quint16>((1.0 + x) / 2 * 65535);
                        if (sampleLittleEndian)
                            qToLittleEndian<quint16>(value, ptr);
                        else
                            qToBigEndian<quint16>(value, ptr);
                    } else if (channelBytes == 2 && sampleType == 1 /* QAudioFormat::SignedInt */) {
                        qint16 value = static_cast<qint16>(x * 32767);
                        if (sampleLittleEndian)
                            qToLittleEndian<qint16>(value, ptr);
                        else
                            qToBigEndian<qint16>(value, ptr);
                    }
                    
                    ptr += channelBytes;
                    size -= channelBytes;
                }
            }
        }
    }

    void Controller::GetInt32Sapmles(int* buffer, int size) {
        //qDebug() << "GetInt32Sapmles " <<  size << " from " <<  buffer;
        for (int i = 0; i < size; ++i) {
            buffer[i] = GetSample() * 2147483648L;
        }
    }
    
    void Controller::setFormat(int type, int channelCount, int channelBytes, bool littleEndian )
    {
        this->sampleType = type;
        this->channelCount = channelCount;
        this->channelBytes = channelBytes;
        this->sampleLittleEndian = littleEndian;
    }
    
    float Controller::GetSample() {
        float value=0;
        for(int i=0;i<key_stack_.GetSize();i++) {
            float voiceValue = key_stack_.getFilter(i)->GetValue(key_stack_.getOsc(i)->GetValue()*key_stack_.getEnvelope(0, i)->GetValue());
            voiceValue *= volume_;
            value += voiceValue;
        }

        // Clip!
        value = fmaxf(-1.0f, value);
        value = fminf(1.0f, value);

        // Adjust volume
        for(int i=0;i<key_stack_.GetSize();i++) {
            if(key_stack_.getEnvelope(0, i)->released()) {
                ////qDebug() << "controller: clear note " << key_stack_.GetNote(i);
                key_stack_.NoteClear(key_stack_.GetNote(i));
            }
        }
        return value;
    }
}  // namespace synth

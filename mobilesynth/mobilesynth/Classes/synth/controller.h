// controller.h
// Author: Allen Porter <allen@thebends.org>
// Polyphony by Claudio Zopfi <z@x21.ch>
//
// The controller module returns samples and drives the oscillator and envelope.

#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include <QAudioFormat>

#include "synth/arpeggio.h"
#include "synth/envelope.h"
#include "synth/filter.h"
#include "synth/lag_processor.h"
#include "synth/modulation.h"
#include "synth/oscillator.h"
#include "synth/parameter.h"
#include "synth/key_stack.h"

namespace synth {
    
    class Controller {
    public:
        Controller();
        
        // Volume [0, 1.0]
        void set_volume(float volume);
        
        // Start/Stop playing a note.  These may trigger the Attack and Release of the
        // volume and filter envelopes, depending on the order of the on/off events.
        // It is an error to call NoteOff() for a note that was never the argument of
        // NoteOn();
        void NoteOn(int midi_note, float freq);
        void NoteOff(int midi_note);
        //        void NoteOnFrequency(float frequency);  // For testing
        void NoteOff();  // Invoked when all notes have been released as a fallback
        
        // True when nothing is playing
        bool released() {
            return key_stack_.GetSize()==0;
        }
        
        void set_sample_rate(float sample_rate);  // For testing
        void setADSR(int n, long a, long d, float s, long r);
        
        // A shift in frequency by the specified amount.  The frequency gets
        // multiplied by 2^n
        enum OctaveShift {
            OCTAVE_1 = 1,  // No shift
            OCTAVE_2 = 2,
            OCTAVE_4 = 4,
            OCTAVE_8 = 8,
            OCTAVE_16 = 16
        };
        
        void set_osc1_wave_type_int(int w);
        //void set_modulation_amount(float amount);
        //void set_modulation_amount(int voice, float amount);
        void set_osc_pw(int voice, float p);
        void set_filter_cutoff(float frequency);
        void set_mod_filter_cutoff(float v);
        void set_mod_filter_resonance(float v);
        void set_modulation(int voice, float v);

        // [0.0, 1.0]
        void set_filter_resonance(float value);
        
        // Get a single sample
        float GetSample();
        
        void GetFloatSamples(float* buffer, int size);
        void GetInt32Sapmles(int* buffer, int size);
        void GetCharSamples(char *buffer, int size);

        float getSampleMem(int i) {
            float r=0.0f;
            if(!released()) {
                if(sampleMemory) {
                    if(i<sampleMemorySize) {
                        r=sampleMemory[i];
                    }
                }
            }
            return r;
        }
        
        void setFormat(QAudioFormat * f);
        
        void setAttack(int n, long a);
        void setDecay(int n, long d);
        void setSustain(int n, float s);
        void setRelease(int n, int r);

        int getNVoices() {return key_stack_.GetSize();}
        int getNVoicesMax() {return key_stack_.kMaxSize;}
        float getVF(int i) {return key_stack_.GetFreq(i);}
        void decVoices();
        void incVoices();

    private:
        // Invoked when one of the routing parameters changes, such as the source
        // or destination of modulation.
        // void reset_routing();
        
        KeyStack key_stack_;
        bool osc_sync_;
        float sample_rate_;
        QAudioFormat * format;
        int channelBytes;
        int sampleBytes;

        float volume_;

        float * sampleMemory;
        int sampleMemorySize;
        int sampleMemoryPnt;

    };
    
}  // namespace synth

#endif // __CONTROLLER_H__

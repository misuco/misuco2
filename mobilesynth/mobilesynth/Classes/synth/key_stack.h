// key_stack.h
// Author: Allen Porter <allen@thebends.org>
//
// The key stack keeps track of notes pressed and released, determining when
// the attack phase should be entered or when the note should just change
// without starting a new note attack.

#ifndef __KEY_STACK_H__
#define __KEY_STACK_H__

#include "envelope.h"
#include "filter.h"
#include "oscillator.h"

#include <QDebug>

namespace synth {
    
    class KeyStack {
    public:
        // We can't push more notes on the stack than this
        int kMaxSize;
        static const int kMaxSizeInit = 10;
        static const int kNumEnv = 1;
        
        KeyStack();
        ~KeyStack();
        
        // Returns true if this was the first note pushed on to the key stack
        bool NoteOn(int note, float freq);
        
        // Returns true if this was the last note removed from the key stack
        bool NoteOff(int note);
        bool NoteClear(int note);
        
        // Returns the current not, or 0 if no note is playing.
        int GetCurrentNote();
        
        // Return the note at the specified position in the stack.  num must be less
        // than size.
        int GetNote(int num);
        
        int GetSize();
        float GetFreq(int num);
        long GetPos(int num);
        long GetPeriodSamples(int num);
        void SetPos(int num,long value);
        void SetSampleRate(float s);
        
        bool IsNoteInStack(int note);
        
        int size();
        
        Envelope * getEnvelope(int i,int j) {
            return envelopes[i][j];
        }
        
        Filter * getFilter(int i) {
            return filters[i];
        }
        
        Oscillator * getOsc(int i) {
            return oscs[i];
        }

        float getModAmt(int i) {
            return mod_amt_[i];
        }
        
        //void setFilterCutoff(int note, float f);
        void setFilterCutoff(float f);
        //void setFilterRes(int note, float f);
        void setFilterRes(float f);
        
        void setOscPW(int note, float pw);
        
        void setOscWave(int w) {
            osc_wave=w;
        }
        
        void setADSR(int n, long a, long d, float s, long r) {env_a[n]=a;env_d[n]=d;env_s[n]=s;env_r[n]=r; qDebug() << "setADSR " << r;}
        void setAttack(int n, long a) {env_a[n]=a;}
        void setDecay(int n, long d) {env_d[n]=d;}
        void setSustain(int n, float s) {env_s[n]=s;}
        void setRelease(int n, long r) {env_r[n]=r;}

        void setModulation(int voice, float mod);
        void setModCutoff(float f) {mod_cutoff_=f;}
        void setModRes(float f) {mod_res_=f;}

    private:
        int size_;
        int notes_[kMaxSizeInit+1];
        Envelope * envelopes[kNumEnv][kMaxSizeInit+1];
        ResonantFilter * filters[kMaxSizeInit+1];
        FilterCutoff * cutoffs[kMaxSizeInit+1];
        Oscillator * oscs[kMaxSizeInit+1];
        //waveform * waveform_;
        
        float sample_rate_;
        
        long env_a[kNumEnv];
        long env_d[kNumEnv];
        float env_s[kNumEnv];
        long env_r[kNumEnv];
        
        float mod_amt_[kMaxSizeInit+1];
        float mod_amt_init_=0.0f;
        float osc_pw;
        int osc_wave;
        float filter_res_;
        float filter_cutoff_; // 0.0 ... 1.0
        float mod_cutoff_;
        float mod_res_;
        void initModulation(int i, float mod);
        void initCutoff(int i, float mod);
        void initRes(int i, float mod);

    };
        
}  // namespace synth

#endif  // __KEY_STACK_H__

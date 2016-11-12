// key_stack.cpp
// Author: Allen Porter <allen@thebends.org>

#include "key_stack.h"
#include <math.h>
#include <assert.h>
#include <QDebug>

using namespace std;

namespace synth {
    
    KeyStack::KeyStack() : size_(0) {
        //qDebug() << "setup keystack";
        kMaxSize=10;

        for(int i=0;i<kMaxSize+1;i++) {
            for(int j=0;j<kNumEnv;j++) {
                envelopes[j][i]=new Envelope();
            }
            oscs[i]=new Oscillator();
            oscs[i]->set_sample_rate(sample_rate_);
            mod_amt_[i]=0;
            cutoffs[i]=new FilterCutoff();
            cutoffs[i]->set_envelope(envelopes[0][i]);
            cutoffs[i]->set_cutoff(5000);
            filters[i]=new ResonantFilter();
            filters[i]->set_cutoff(cutoffs[i]);
            filters[i]->set_resonance(0);

        }
        for(int j=0;j<kNumEnv;j++) {
            env_a[j]=1000;
            env_d[j]=1000;
            env_s[j]=0.3;
            env_r[j]=100000;
        }

        mod_amt_init_=0;
        osc_pw=0.5;
        filter_res_=0;
        filter_cutoff_=0.5f;
        //qDebug() << "setup keystack done ";
    }
    
    KeyStack::~KeyStack() {
        //qDebug() << "delete keystack ";
        for(int i=0;i<kMaxSize;i++) {
            for(int j=0;j<kNumEnv;j++) {
                delete(envelopes[j][i]);
            }
            delete(oscs[i]);
            delete(cutoffs[i]);
            delete(filters[i]);
        }
        //qDebug() << "delete keystack done ";
    }
    
    bool KeyStack::NoteOn(int note, float freq) {
        //qDebug() << "KeyStack note on " << note << " f: " << freq << " size: " << size_;
        
        for (int i = 0; i < size_; ++i) {
            if (notes_[i] == note) {
                oscs[i]->set_frequency(freq);
                oscs[i]->set_pulse_width(osc_pw);
                oscs[i]->set_wave_type(osc_wave);
                oscs[i]->set_sample_rate(sample_rate_);
                cutoffs[i]->set_cutoff(freq);
                filters[i]->resetxy();
                mod_amt_[i]=mod_amt_init_;
                initModulation(i,0);
                //qDebug() << "  KeyStack existing note " << note << " size " << size_ << " wave " << osc_wave;
                return false;
            }
        }
        
        
        // if stack full, clear bottom of stack
        // => kill oldest note
        if(size_ >= kMaxSize) {
            NoteClear(notes_[0]);
            //qDebug() << "  KeyStack full, NoteClear " << notes_[0];
        }
        // put new note on top of stack
        notes_[size_] = note;
        oscs[size_]->set_frequency(freq);
        oscs[size_]->set_pulse_width(osc_pw);
        oscs[size_]->set_wave_type(osc_wave);
        oscs[size_]->set_sample_rate(sample_rate_);
        cutoffs[size_]->set_cutoff(freq);
        filters[size_]->resetxy();
        mod_amt_[size_]=mod_amt_init_;
        initModulation(size_,0);

        //qDebug() << "  KeyStack new note " << note << " size " << size_ << " wave " << osc_wave;
        for(int i=0;i<kNumEnv;i++) {
            envelopes[i][size_]->set_attack(env_a[i]);
            envelopes[i][size_]->set_decay(env_d[i]);
            envelopes[i][size_]->set_sustain(env_s[i]);
            envelopes[i][size_]->set_release(env_r[i]);
            envelopes[i][size_]->NoteOn();
            //qDebug() << "set_release " << env_r[i];
        }
        size_++;
        return true;
    }
    
    bool KeyStack::NoteOff(int note) {
        //qDebug() << "KeyStack note off " << note << " size: " << size_;
        for (int i = 0; i < size_; ++i) {
            if (notes_[i] == note) {
                for(int k=0;k<kNumEnv;k++) {
                    envelopes[k][i]->NoteOff();
                }
                //qDebug() << " KeyStack note off " << note << " size: " << size_;
                return true;
            }
        }
        // The note wasn't on the stack.  The multi-touch events on the iphone seem
        // to be flaky, so we don't worry if we were asked to remove something that
        // was not on the stack.  The controller also calls our clear() method when
        // no touch events are left as a fallback.
        //qDebug() << "KeyStack unknown note off " << note << " size: " << size_;
        return false;
    }
    
    bool KeyStack::NoteClear(int note) {
        //qDebug() << "KeyStack  NoteClear " << note << " size " << size_;
        for (int i = 0; i < size_; ++i) {
            if (notes_[i] == note) {
                // Remove this element from the stack -- copy all elements above
                mod_amt_[size_] = mod_amt_[i];
                for(int k=0;k<kNumEnv;k++) {
                    envelopes[k][kMaxSize]=envelopes[k][i];
                }
                filters[kMaxSize]=filters[i];
                cutoffs[kMaxSize]=cutoffs[i];
                oscs[kMaxSize]=oscs[i];
                for (int j = i; j < size_-1; ++j) {
                    notes_[j] = notes_[j + 1];
                    for(int k=0;k<kNumEnv;k++) {
                        envelopes[k][j]=envelopes[k][j + 1];
                    }
                    filters[j]=filters[j + 1];
                    cutoffs[j]=cutoffs[j + 1];
                    oscs[j]=oscs[j + 1];
                    //lfos[j]=lfos[j + 1];
                    mod_amt_[j]=mod_amt_[j + 1];
                }

                for(int k=0;k<kNumEnv;k++) {
                    envelopes[k][size_-1]=envelopes[k][kMaxSize];
                }
                cutoffs[size_-1]=cutoffs[kMaxSize];
                filters[size_-1]=filters[kMaxSize];
                oscs[size_-1]=oscs[kMaxSize];
                mod_amt_[size_-1]=mod_amt_[kMaxSize];

                size_--;
                //qDebug() << "-T NoteClear " << note;
                //qDebug() << "notes_ " << notes_[0] << " " << notes_[1] << " " << notes_[2] << " " << notes_[3] << " " ;
                return true;
            }
        }
        // The note wasn't on the stack.  The multi-touch events on the iphone seem
        // to be flaky, so we don't worry if we were asked to remove something that
        // was not on the stack.  The controller also calls our clear() method when
        // no touch events are left as a fallback.
        //qDebug() << "-F NoteClear " << note;
        return false;
    }

    int KeyStack::size() {
        return size_;
    }
    
    int KeyStack::GetCurrentNote() {
        if (size_ > 0) {
            return notes_[size_ - 1];
        }
        return 0;
    }
    
    int KeyStack::GetSize() {
        return size_;
    }

    float KeyStack::GetFreq(int num)
    {
        return oscs[num]->get_frequency();
    }
    
    int KeyStack::GetNote(int num) {
        if (num >= size_) {
            return 0;
        }
        return notes_[num];
    }

    void KeyStack::SetSampleRate(float s) {
        sample_rate_=s;
        for(int i=0;i<kMaxSize;i++) {
            oscs[i]->set_sample_rate(s);
        }
    }
    
    void KeyStack::setFilterCutoff(float f) {
        filter_cutoff_=f;
        for(int i=0;i<size_;i++) {
            initCutoff(i,mod_amt_[i]);
        }
    }

    void KeyStack::setModulation(int voice, float mod) {
        //qDebug() << "KeyStack::setModulation " << voice << " " << mod;
        for (int i = 0; i < size_; ++i) {
            if (notes_[i] == voice) {
                initModulation(i,mod);
                break;
            }
        }
    }

    void KeyStack::initModulation(int i, float mod) {
        mod-=0.5f;
        mod*=2;
        mod_amt_[i]=mod;
        initCutoff(i,mod);
        initRes(i,mod);
        //qDebug() << "KeyStack::initModulation " << i << " " << mod;
    }

    void KeyStack::initCutoff(int i, float mod)
    {
        float fcf=oscs[i]->get_frequency()*filter_cutoff_*84;
        fcf+=fcf*mod_cutoff_*mod;
        cutoffs[i]->set_cutoff(fcf);
        //qDebug() << "KeyStack::initCutoff " << i << " " << mod << " fcf " << fcf;
    }

    void KeyStack::initRes(int i, float mod)
    {
        float frs=filter_res_+filter_res_*mod_res_*mod;
        filters[i]->set_resonance(frs);
        //qDebug() << "KeyStack::initRes " << i << " " << mod << " frs " << frs;
    }

    void KeyStack::setFilterRes(float f) {
        filter_res_=f;
        for(int i=0;i<size_;i++) {
            initRes(i,mod_amt_[i]);
        }
    }
    
    void KeyStack::setOscPW(int voice, float pw) {
        for (int i = 0; i < size_; ++i) {
            if (notes_[i] == voice) {
                oscs[i]->set_pulse_width(pw);
                break;
            }
        }
        osc_pw=pw;
    }
}  // namespace synth

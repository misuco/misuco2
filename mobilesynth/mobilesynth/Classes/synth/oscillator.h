// oscillator.h
// Author: Allen Porter <allen@thebends.org>
//
// An oscillator generates a signal with a particular frequency and amplitude.

#ifndef __OSCILLATOR_H__
#define __OSCILLATOR_H__

#include "synth/parameter.h"
#include "synth/lag_processor.h"
//#include "synth/waveform.h"

namespace synth {
    
    static const long kDefaultSampleRate = 44100;
    
    class Oscillator : public Parameter {
    public:
        Oscillator();
        virtual ~Oscillator();
        
        enum WaveType {
            SINE,
            SQUARE,
            TRIANGLE,
            SAWTOOTH,
            REVERSE_SAWTOOTH,
            NOISE,
            WAVETABLE,
        };
        void set_wave_type(WaveType wave_type);
        void set_wave_type(int w);
        
        void set_frequency(float frequency);
        float get_frequency() {return frequency_;}
        void set_period_samples(long p);
        void set_pulse_width(float p);

        // Override the default sample rate
        void set_sample_rate(long sample_rate);
        
        // Returns the value at the specific time [0.0, 1.0].  The returned value
        // returned value is in the range [-1.0, 1.0].
        virtual float GetValue();
        
        // Start at the beginning of the period
        void Reset() { sample_num_ = 0; }
        
        // Returns true if this is the first sample in the period
        bool IsStart() { return (sample_num_ == 0); }
        
    private:
        WaveType wave_type_;
        float frequency_;
        float phase_;
        float pi2;
        float pulse_width_;
        float sample_rate_;
        long sample_num_;
        float phase_inc_;
        float value;

    };
    
}  // namespace synth

#endif  // __OSCILLATOR_H__

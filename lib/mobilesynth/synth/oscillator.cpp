// oscillator.cpp
// Author: Allen Porter <allen@thebends.org>

#include "oscillator.h"
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include "parameter.h"
#include <QDebug>

namespace synth {
    
    Oscillator::Oscillator()
    : wave_type_(SINE),
      frequency_(0),
      phase_(0),
      pi2(M_PI*2),
      pulse_width_(M_PI),
      sample_rate_(kDefaultSampleRate),
      sample_num_(0),
      phase_inc_(0),
      value(0)
    {}
    
    Oscillator::~Oscillator() { }
    
    void Oscillator::set_sample_rate(long sample_rate) {
        sample_rate_ = sample_rate;
    }
    
    void Oscillator::set_wave_type(WaveType wave_type) {
        wave_type_ = wave_type;
    }
    
    void Oscillator::set_wave_type(int w) {
        //qDebug() << "set_wave_type " << w;
        switch (w) {
            case 0:
                wave_type_ = Oscillator::SQUARE;
                break;
            case 1:
                wave_type_ = Oscillator::SAWTOOTH;
                break;
            case 2:
                wave_type_ = Oscillator::SINE;
                break;
            case 3:
                wave_type_ = Oscillator::TRIANGLE;
                break;
            case 4:
                wave_type_ = Oscillator::NOISE;
                break;
            default:
                wave_type_ = Oscillator::SINE;
                break;
        }
    }

    void Oscillator::set_frequency(float frequency) {
        if(frequency_!=frequency) {
            frequency_=frequency;
            phase_inc_=pi2*frequency_/ sample_rate_;
        }
        //qDebug() << "Oscilator::set_frequency " << frequency << " t: " << t_ << " sr: " << sample_rate_;
    }
    
    float Oscillator::GetValue() {
        if (frequency_ < 0.01f) {
            return 0.0f;
        }
        phase_+=phase_inc_;
        if(phase_>pi2) {
            phase_-=pi2;
        }
        switch (wave_type_) {
            case SINE: // 2
                value = sinf(phase_);
                break;
            case SQUARE: // 0
                if (phase_ < pulse_width_) {
                    value = 1.0f;
                } else {
                    value = -1.0f;
                }
                break;
            case TRIANGLE:
                if (phase_ < pulse_width_) {
                    value = 1.0f-(2.0f*phase_/M_PI);
                } else {
                    value = (2.0f*(phase_-M_PI)/M_PI)-1.0f;
                }
                break;
            case SAWTOOTH: // 1
                value = 2.0f * phase_ / pi2 - 1.0f;
                break;
            case NOISE:
                value = -1.0f + (float)rand()/((float)RAND_MAX/2.0f);
                break;
            case REVERSE_SAWTOOTH:
                value = 1.0f - 2.0f * phase_ / pi2;
                break;
            default:
                value = 0.0f;
                break;
        }
        sample_num_++;
        return value;
    }
    
    void Oscillator::set_pulse_width(float p) {
        pulse_width_ = p*pi2;
    }
    
}  // namespace synth

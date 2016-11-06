// synth.cpp
// Author: Allen Porter <allen@thebends.org>
// Redesign: Claudio Zopfi <z@x21.ch>

#include <qdebug.h>
#include "synth/envelope.h"

//#include <algorithm>
//#include <cmath>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

namespace synth {

Envelope::Envelope() : attack_(0),
                       attack_slope_(0.0f),
                       decay_(0),
                       decay_end_(0),
                       decay_slope_(0.0f),
                       sustain_(1.0f),
                       release_(0),
                       release_start_(0),
                       release_end_(0),
                       release_slope_(0.0f),
                       min_(0.0f),
                       max_(1.0f),
                       current_(0),
                       state_(DONE) {
}

Envelope::~Envelope() { }

void Envelope::set_attack(long attack) {
  assert(attack >= 0);
  attack_ = attack;
}

void Envelope::set_max(float max) {
  assert(max >= 0);
  max_ = max;
}

void Envelope::set_decay(long decay) {
  assert(decay >= 0);
  decay_ = decay;
}

void Envelope::set_sustain(float sustain) {
  assert(sustain >= 0.0);
  sustain_ = sustain;
}

void Envelope::set_release(long release) {
    qDebug() << "Envelope::set_release " << release;
  assert(release >= 0);
  release_ = release;
}

void Envelope::set_min(float min) {
  assert(min >= 0);
  min_ = min;
}

void Envelope::NoteOn() {
    //qDebug() << "NoteOn at current " << current_;
  current_ = 0;
  decay_end_ = attack_ + decay_;
  if (attack_ == 0) {
    attack_slope_ = 1;
  } else {
    attack_slope_ = (max_ - min_) / attack_;
  }
  if (decay_ == 0) {
    decay_slope_ = 1;
  } else {
    decay_slope_ = (max_ - sustain_) / decay_;
  }
  state_ = ATTACK;
  qDebug() << " NoteOn at current " <<  current_ << " a: " << attack_ << " d: " << decay_end_ << " s: " << sustain_ << " r: " << release_end_ ;
}

void Envelope::NoteOff() {
  state_ = RELEASE;
  release_start_value_ = last_value_;
  if (release_ == 0) {
    release_slope_ = 1; 
  } else {
    release_slope_ = (release_start_value_ - min_) / release_;
  }
  release_start_ = current_; 
  release_end_ = current_ + release_;
  qDebug() << "env state to release at " << current_ << " release end at " << release_end_ << " release " << release_;
}

bool Envelope::released() const {
  return (state_ == DONE);
}    
  
float Envelope::GetValue() {
  current_++;
  float value = 0;
  
  // Check that we haven't transitioned longo the next state
    if (state_ == DECAY && current_ > decay_end_) {
        state_ = SUSTAIN;
        qDebug() << "env state to sustain at " << current_ << " decay_end: " << decay_end_;
    } else if (state_ == ATTACK && current_ > attack_) {
        state_ = DECAY;
        qDebug() << "env state to decay at " << current_ << " attack: " << attack_;
    } else if (state_ == SUSTAIN && sustain_ <= 0.0) {
        state_ = DONE;
        qDebug() << "env state to done in sustain at " << current_;
    } else if (state_ == RELEASE && current_ > release_end_) {
        state_ = DONE;
        qDebug() << "env state to done in release at " << current_;
    }

  switch (state_) {
    case ATTACK:
      value = current_ * attack_slope_ + min_;
      value = (value < max_) ? value : max_;
     break;
    case DECAY:
      value = max_ - (current_ - attack_) * decay_slope_;
      value = (value > sustain_) ? value : sustain_;
      break;
    case SUSTAIN:
      value = sustain_;
      if(value<0.0) {
        fprintf(stderr, "sustain<0: %f", value);
        value=0.0;
      }
      break;
    case RELEASE:
      value = release_start_value_ -
           (current_ - release_start_) * release_slope_;
      value = (value > min_) ? value : min_;
      break;
    case DONE:
      value = min_;
      break;
    default:
      fprintf(stderr, "Unhandled state: %d", state_);
      exit(1);
      break;
  }
  last_value_ = value;
  return value;
}

}  // namespace synth

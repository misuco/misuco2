#ifndef TYPES_H
#define TYPES_H

#include <QString>
#include "freqtriple.h"

#define BSCALE_SIZE 11
#define HUE_NOTES 30

enum MWPlayfieldType {
    NORMAL,
    BEND_VERT,
    BEND_HORIZ,
    BEND_VERT_HORIZ
};

struct MWPlayfield {
    MWPlayfieldType type = NORMAL;
    FreqTriple * f1;      // for type normal
    FreqTriple * f2;      // for type vert/hoiz
    int hue1bent;         // cache for vertical bent hue values
    int hue2bent;
    unsigned int pressed = 0;
    bool hold = false;
    int voiceId = 0;    // cache for hold voiceId
};

struct MWScale {
    int basenote;
    int baseoct;
    int topoct;
    bool bscale[BSCALE_SIZE];
    int size;
};

struct MWMicrotune {
    int tuning[BSCALE_SIZE+1];
};

struct MWSound {
    int wave_type;
    int attack;
    int decay;
    float sustain;
    int release;
    float filter_cutoff;
    float filter_resonance;
    float mod_filter_cutoff;
    float mod_filter_resonance;
    float volume;
};

#endif // TYPES_H


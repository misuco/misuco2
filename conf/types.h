#ifndef TYPES_H
#define TYPES_H

#include <QString>
#include "freqtriple.h"

#define BSCALE_SIZE 11

enum MWPlayfieldType {
    NORMAL,
    BEND_VERT,
    BEND_HORIZ,
    BEND_VERT_HORIZ
};

struct MWPlayfield {
    MWPlayfieldType type = NORMAL;
    FreqTriple f1;      // for type normal
    FreqTriple f2;      // for type vert/hoiz
    FreqTriple f3;      // for type vert+horiz
    FreqTriple f4;      // for type vert+horiz
    unsigned int color1 = 0;      // hue (0-360);
    unsigned int color2 = 0;      // hue (0-360);
    unsigned int color3 = 0;      // hue (0-360);
    unsigned int color4 = 0;      // hue (0-360);
    QString * text;
    unsigned int pressed = 0;
};

struct MWScale {
    int basenote;
    int baseoct;
    int topoct;
    bool bscale[BSCALE_SIZE];
};

#endif // TYPES_H


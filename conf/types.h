/*
 * Copyright (c) 2018 by misuco.org, Claudio Zopfi, Zurich, Switzerland, c@misuco.org
 *
 * This file is part of MISUCO2.
 *
 * MISUCO2 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * MISUCO2 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with MISUCO2.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef TYPES_H
#define TYPES_H

#include <QString>

#define SCALE_SIZE 11

enum MWPlayfieldType {
    NORMAL,
    BEND_VERT,
    BEND_HORIZ,
    BEND_VERT_HORIZ
};

struct MWMicrotune {
    int tuning[SCALE_SIZE+1];
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

struct misuTouchEvent {
    int id;
    int x;
    int y;
    int state;
    long t;
};

#endif // TYPES_H


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

#include "freqtriple.h"
#include "types.h"
#include <math.h>

FreqTriple::FreqTriple(int rootNote, QObject *parent) :QObject(parent),
    _rootNote(rootNote),
    _pitch(0),
    _oct(6)
{
    for(int i=0;i<SCALE_SIZE+1;i++) {
        _pitchTable.append(0);
    }
    initFreq();
}

FreqTriple::~FreqTriple()
{

}

float FreqTriple::getFreq() const
{
    return _freq;
}

int FreqTriple::getMidinote() const
{
    return _midinote;
}

void FreqTriple::setMidinote(int midinote, int pitch)
{
    _midinote = midinote;
    _rootNote = calcRootNoteFromMidinote(midinote);
    _pitch = pitch;
    _freq = calcFreq(_midinote,pitch);
    _oct = calcOctFromMidinote(_midinote);
    calcColor();
}

void FreqTriple::initFreq() {
    _midinote = _rootNote + _oct * 12;
    _freq = calcFreq(_midinote,_pitch);
    //_oct = calcOctFromMidinote(_midinote);
    calcColor();
}

int FreqTriple::getPitch() const
{
    return _pitch;
}

void FreqTriple::setPitch(int value)
{
    _pitch=value;
    _freq = calcFreq(_midinote,_pitch);
    calcColor();
}

float FreqTriple::getHue() const
{
    return _hue;
}

bool FreqTriple::getBW() const
{
    return _pianoWhite;
}

int FreqTriple::getRootNote() const
{
    return _rootNote;
}

QString FreqTriple::getRootNoteString(int lang) const
{
    if(lang==1) {
        switch(_rootNote) {
                case 0: return "सा";
                break;
                case 1: return "<u>रे</u>";
                break;
                case 2: return "रे";
                break;
                case 3: return "<u>ग</u>";
                break;
                case 4: return "ग";
                break;
                case 5: return "म";
                break;
                case 6: return "म'";
                break;
                case 7: return "प";
                break;
                case 8: return "<u>ध</u>";
                break;
                case 9: return "ध";
                break;
                case 10: return "<u>नि</u>";
                break;
                case 11: return "नि";
                break;
        }
    } else if(lang==2) {
        switch(_rootNote) {
                case 0: return "DO";
                break;
                case 1: return "<u>RE</u>";
                break;
                case 2: return "RE";
                break;
                case 3: return "<u>MI</u>";
                break;
                case 4: return "MI";
                break;
                case 5: return "FA";
                break;
                case 6: return "FA#";
                break;
                case 7: return "SO";
                break;
                case 8: return "<u>LA</u>";
                break;
                case 9: return "LA";
                break;
                case 10: return "<u>TI</u>";
                break;
                case 11: return "TI";
                break;
        }
    } else if(lang==3) {
        switch(_rootNote) {
                case 0: return "SA";
                break;
                case 1: return "<u>RE</u>";
                break;
                case 2: return "RE";
                break;
                case 3: return "<u>GA</u>";
                break;
                case 4: return "GA";
                break;
                case 5: return "MA";
                break;
                case 6: return "MA'";
                break;
                case 7: return "PA";
                break;
                case 8: return "<u>DA</u>";
                break;
                case 9: return "DA";
                break;
                case 10: return "<u>NI</u>";
                break;
                case 11: return "NI";
                break;
        }
    } else if(lang==0) {
        switch(_rootNote) {
        case 0: return "C";
        break;
        case 1: return "<u>D</u>";
        break;
        case 2: return "D";
        break;
        case 3: return "<u>E</u>";
        break;
        case 4: return "E";
        break;
        case 5: return "F";
        break;
        case 6: return "F#";
        break;
        case 7: return "G";
        break;
        case 8: return "<u>A</u>";
        break;
        case 9: return "A";
        break;
        case 10: return "<u>B</u>";
        break;
        case 11: return "B";
        break;
        }
    }
    return "";

    /*
    case 0: return "A";
    break;
    case 1: return "N";
    break;
    case 2: return "D";
    break;
    case 3: return "R";
    break;
    case 4: return "E";
    break;
    case 5: return "I";
    break;
    case 6: return "T";
    break;
    case 7: return "A";
    break;
    case 8: return "M";
    break;
    case 9: return "O";
    break;
    case 10: return "R";
    break;
    case 11: return "E";
    break;
    */
}

void FreqTriple::setRootNote(int rootNote)
{
    _pitch = _pitchTable.at(rootNote);
    _rootNote = rootNote;
    _midinote = rootNote+_oct*12;
    _freq = calcFreq(_midinote,_pitch);
    calcColor();
}

int FreqTriple::getOct() const
{
    return _oct;
}

void FreqTriple::setOct(int value)
{
    _oct = value;
    _midinote = _rootNote+_oct*12;
    _freq = calcFreq(_midinote,_pitch);
}

void FreqTriple::onPitchChange(int rootNote, int value)
{
    _pitchTable[rootNote] = value;
    if(rootNote == _rootNote) {
        _pitch=value;
        _freq = calcFreq(_midinote,_pitch);
        calcColor();
    }
}

float FreqTriple::calcFreq(int midinote, int pitch) {
    return pow(2.0,(float)pitch/1200.0)*calcMidi2Fequal(midinote);
}

double FreqTriple::Log2( double n )
{
    return log( n ) / log( 2 );
}

float FreqTriple::calcMidi2Fequal(int x)
{
    return pow(2.0,(float)(x-69)/12)*440.0;
}

int FreqTriple::calcOctFromMidinote(int m) {
    return m/12;
}

int FreqTriple::calcRootNoteFromMidinote(int m) {
    return m%12;
}

void FreqTriple::calcColor()
{
    _pianoWhite = true;
    int bn = _rootNote;
    if(bn == 1 || bn == 3 || bn == 6 || bn == 8 || bn == 10 ) _pianoWhite = false;

    _hue = (float)_rootNote / 12 + (float)_pitch / 2400;
    if(_hue>1) _hue -=1;
    if(_hue<0) _hue +=1;

    emit colorChanged();
}

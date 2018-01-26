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
#include <math.h>
#include <QDebug>

FreqTriple::FreqTriple(Pitch *p, QObject *parent) :QObject(parent)
{
    pitch=p;
    initFreq();
}

FreqTriple::~FreqTriple()
{

}

float FreqTriple::getFreq() const
{
    return freq;
}

int FreqTriple::getMidinote() const
{
    return midinote;
}

void FreqTriple::setMidinote(int m, Pitch * p)
{
    midinote = m;
    pitch = p;
    freq = calcFreq(midinote,pitch->getPitch());
    oct = calcOctFromMidinote(midinote);
    //qDebug() << "FreqTriple::setMidinote " << midinote << " freq " << freq << " hue " << hue;
}

void FreqTriple::initFreq() {
    midinote = pitch->getBasenote();
    freq = calcFreq(midinote,pitch->getPitch());
    oct = calcOctFromMidinote(midinote);
}

int FreqTriple::getPitch() const
{
    return pitch->getPitch();
}

void FreqTriple::setPitch(int value)
{
    pitch->setPitch(value);
    initFreq();
}

float FreqTriple::getHue() const
{
    return pitch->getHue();
}

bool FreqTriple::getBW() const
{
    return pitch->getBW();
}

int FreqTriple::getBasenote() const
{
    return pitch->getBasenote();
}


QString FreqTriple::getBasenoteString(int lang) const
{
    if(lang==1) {
        switch(pitch->getBasenote()) {
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
        switch(pitch->getBasenote()) {
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
        switch(pitch->getBasenote()) {
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
        switch(pitch->getBasenote()) {
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


void FreqTriple::setBasenote(Pitch * p)
{
    pitch = p;
    midinote = pitch->getBasenote()+oct*12;
    freq = calcFreq(midinote,pitch->getPitch());
    //qDebug() << "FreqTriple::setBasenote midinote: " << midinote << " freq: " << freq;
}

int FreqTriple::getOct() const
{
    return oct;
}

void FreqTriple::setOct(int value)
{
    oct = value;
    midinote = pitch->getBasenote()+oct*12;
    freq = calcFreq(midinote,pitch->getPitch());
}

void FreqTriple::pitchChange()
{
    freq=calcFreq(midinote,pitch->getPitch());
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

int FreqTriple::calcBasenoteFromMidinote(int m) {
    return m%12;
}

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
    freq = calcFreq(midinote,pitch->pitch);
    oct = calcOctFromMidinote(midinote);
    //qDebug() << "FreqTriple::setMidinote " << midinote << " freq " << freq << " hue " << hue;
}

void FreqTriple::initFreq() {
    midinote = pitch->basenote;
    freq = calcFreq(midinote,pitch->pitch);
    oct = calcOctFromMidinote(midinote);
}

int FreqTriple::getPitch() const
{
    return pitch->pitch;
}

void FreqTriple::setPitch(int value)
{
    pitch->setPitch(value);
    initFreq();
}

int FreqTriple::getHue() const
{
    return pitch->color;
}

bool FreqTriple::getBW() const
{
    return pitch->getBW();
}

int FreqTriple::getBasenote() const
{
    return pitch->basenote;
}


QString FreqTriple::getBasenoteString(int lang) const
{
    if(lang==1) {
        switch(pitch->basenote) {
                case 0: return "सा";
                break;
                case 1: return "_रे";
                break;
                case 2: return "रे";
                break;
                case 3: return "_ग";
                break;
                case 4: return "ग";
                break;
                case 5: return "म";
                break;
                case 6: return "म'";
                break;
                case 7: return "प";
                break;
                case 8: return "_ध";
                break;
                case 9: return "ध";
                break;
                case 10: return "_नि";
                break;
                case 11: return "नि";
                break;
        }
    } else if(lang==2) {
        switch(pitch->basenote) {
                case 0: return "DO";
                break;
                case 1: return "_RE";
                break;
                case 2: return "RE";
                break;
                case 3: return "_MI";
                break;
                case 4: return "MI";
                break;
                case 5: return "FA";
                break;
                case 6: return "FA#";
                break;
                case 7: return "SO";
                break;
                case 8: return "_LA";
                break;
                case 9: return "LA";
                break;
                case 10: return "_TI";
                break;
                case 11: return "TI";
                break;
        }
    } else if(lang==3) {
        switch(pitch->basenote) {
                case 0: return "SA";
                break;
                case 1: return "_RE";
                break;
                case 2: return "RE";
                break;
                case 3: return "_GA";
                break;
                case 4: return "GA";
                break;
                case 5: return "MA";
                break;
                case 6: return "MA#";
                break;
                case 7: return "PA";
                break;
                case 8: return "_DA";
                break;
                case 9: return "DA";
                break;
                case 10: return "_NI";
                break;
                case 11: return "NI";
                break;
        }
    } else {
        switch(pitch->basenote) {
        case 0: return "C";
        break;
        case 1: return "_D";
        break;
        case 2: return "D";
        break;
        case 3: return "_E";
        break;
        case 4: return "E";
        break;
        case 5: return "F";
        break;
        case 6: return "F#";
        break;
        case 7: return "G";
        break;
        case 8: return "_A";
        break;
        case 9: return "A";
        break;
        case 10: return "_B";
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
    midinote = pitch->basenote+oct*12;
    freq = calcFreq(midinote,pitch->pitch);
    //qDebug() << "FreqTriple::setBasenote midinote: " << midinote << " freq: " << freq;
}

int FreqTriple::getOct() const
{
    return oct;
}

void FreqTriple::setOct(int value)
{
    oct = value;
    midinote = pitch->basenote+oct*12;
    freq = calcFreq(midinote,pitch->pitch);
}

void FreqTriple::pitchChange()
{
    freq=calcFreq(midinote,pitch->pitch);
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

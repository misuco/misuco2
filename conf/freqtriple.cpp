#include "freqtriple.h"
#include <math.h>
#include <QDebug>

FreqTriple::FreqTriple(Pitch *p, QObject *parent)
{
    pitch=p;
    initFreq();
    //setFreq(freq_a);
}

FreqTriple::~FreqTriple()
{

}

float FreqTriple::getFreq() const
{
    return freq;
}
/*
void FreqTriple::setFreq(float f)
{
    freq = f;
    midinote = round(Log2(f/(freq_a/64.0))*12.0-3);
    pitch->setPitch(calcPitch(midinote,freq));
    oct = calcOctFromMidinote(midinote);
    basenote = calcBasenoteFromMidinote(midinote);
    calcHue();
}
*/

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

int FreqTriple::getBasenote() const
{
    return pitch->basenote;
}


QString FreqTriple::getBasenoteString() const
{
    switch(pitch->basenote) {
    case 0: return "C";
    break;
    case 1: return "C#";
    break;
    case 2: return "D";
    break;
    case 3: return "D#";
    break;
    case 4: return "E";
    break;
    case 5: return "F";
    break;
    case 6: return "F#";
    break;
    case 7: return "G";
    break;
    case 8: return "G#";
    break;
    case 9: return "A";
    break;
    case 10: return "A#";
    break;
    case 11: return "B";
    break;

    }
}


void FreqTriple::setBasenote(Pitch * p)
{
    pitch = p;
    midinote = pitch->pitch+oct*12;
    freq = calcFreq(midinote,pitch->pitch);
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

//float FreqTriple::calcPitch(int midinote, float f) {
//    return round(Log2(f/calcMidi2Fequal(midinote))*12*8192/2);
//}

float FreqTriple::calcFreq(int midinote, int pitch) {
//    return pow(2.0,(float)pitch*2.0/12.0/8192.0)*calcMidi2Fequal(midinote);
    return pow(2.0,(float)pitch/1200.0)*calcMidi2Fequal(midinote);
}

double FreqTriple::Log2( double n )
{
    // log(n)/log(2) is log2.
    return log( n ) / log( 2 );
}

float FreqTriple::calcMidi2Fequal(int x)
{
    /*
    int oct=(x+3)/12;
    float p=(x+3)%12;
    return (freq_a / 64.0f) * (pow(2.0 , (double)((float)oct*1200.0f+100.0f*p) / 1200.0f));
    */
    return pow(2.0,(float)(x-69)/12)*440.0;
}

int FreqTriple::calcOctFromMidinote(int m) {
    return m/12;
}

int FreqTriple::calcBasenoteFromMidinote(int m) {
    return m%12;
}

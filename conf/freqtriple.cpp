#include "freqtriple.h"
#include <math.h>
#include <QDebug>

FreqTriple::FreqTriple()
{
    setFreq(freq_a);
}

FreqTriple::~FreqTriple()
{

}

float FreqTriple::getFreq() const
{
    return freq;
}

void FreqTriple::setFreq(float f)
{
    freq = f;
    midinote = round(Log2(f/(freq_a/64.0))*12.0-3);
    pitch = calcPitch(midinote,freq);
    oct = calcOctFromMidinote(midinote);
    basenote = calcBasenoteFromMidinote(midinote);
    calcHue();
}

int FreqTriple::getMidinote() const
{
    return midinote;
}

void FreqTriple::setMidinote(int value)
{
    midinote = value;
    pitch=0;
    freq=calcFreq(midinote,pitch);
    oct = calcOctFromMidinote(midinote);
    basenote = calcBasenoteFromMidinote(midinote);
    calcHue();
    //qDebug() << "FreqTriple::setMidinote " << midinote << " freq " << freq << " hue " << hue;
}

void FreqTriple::setMidinote(int midinote, int pitch)
{
    this->midinote = midinote;
    this->pitch=pitch;
    freq=calcFreq(midinote,pitch);
    oct = calcOctFromMidinote(midinote);
    basenote = calcBasenoteFromMidinote(midinote);
    calcHue();
}

int FreqTriple::getPitch() const
{
    return pitch;
}

void FreqTriple::setPitch(int value)
{
    pitch = value;
    freq=calcFreq(midinote,pitch);
}

int FreqTriple::getHue() const
{
    return hue;
}

int FreqTriple::getBasenote() const
{
    return basenote;
}

void FreqTriple::setBasenote(int value)
{
    basenote = value;
    midinote = basenote+oct*12;
    freq = calcFreq(midinote,pitch);
    calcHue();
}

int FreqTriple::getOct() const
{
    return oct;
}

void FreqTriple::setOct(int value)
{
    oct = value;
    midinote = basenote+oct*12;
    freq = calcFreq(midinote,pitch);
    calcHue();
}



float FreqTriple::calcPitch(int midinote, float f) {
    return round(Log2(f/calcMidi2Fequal(midinote))*12*8192/2);
}

float FreqTriple::calcFreq(int midinote, int pitch) {
//    return pow(2.0,(float)pitch*2.0/12.0/8192.0)*calcMidi2Fequal(midinote);
    return pow(2.0,(float)pitch/12.0/8192.0)*calcMidi2Fequal(midinote);
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

void FreqTriple::calcHue() {
    hue=midinote%12*30;
}

int FreqTriple::calcOctFromMidinote(int m) {
    return m/12;
}

int FreqTriple::calcBasenoteFromMidinote(int m) {
    return m%12;
}

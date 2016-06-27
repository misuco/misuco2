#ifndef FREQTRIPLE_H
#define FREQTRIPLE_H

class FreqTriple
{
public:
    FreqTriple();
    ~FreqTriple();

    float getFreq() const;
    void setFreq(float value);

    int getMidinote() const;
    void setMidinote(int value);
    void setMidinote(int midinote, int pitch);

    int getPitch() const;
    void setPitch(int value);

    int getHue() const;

    int getBasenote() const;
    void setBasenote(int value);

    int getOct() const;
    void setOct(int value);

private:
    const float freq_a=440;

    float freq;
    int midinote;
    int pitch;
    int hue;
    int basenote;
    int oct;

    float calcPitch(int midinote, float f);
    float calcFreq(int midinote, int pitch);
    float calcMidi2Fequal(int x);
    double Log2(double n);
    void calcHue();
    int calcOctFromMidinote(int m);
    int calcBasenoteFromMidinote(int m);
};

#endif // FREQTRIPLE_H

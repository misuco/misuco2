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

    int getPitch() const;
    void setPitch(int value);

    int getHue() const;

private:
    const float freq_a=440;

    float freq;
    int midinote;
    int pitch;
    int hue;

    float calcPitch(int midinote, float f);
    float calcFreq(int midinote, int pitch);
    float calcMidi2Fequal(int x);
    double Log2(double n);
    void calcHue();
};

#endif // FREQTRIPLE_H

#ifndef FREQTRIPLE_H
#define FREQTRIPLE_H

#include "pitch.h"
#include "color.h"

class FreqTriple: public QObject
{
    Q_OBJECT

public:
    explicit FreqTriple(Pitch *p, QObject *parent = 0);
    ~FreqTriple();

    float getFreq() const;

    int getMidinote() const;
    void setMidinote(int m, Pitch *p);

    int getPitch() const;
    void setPitch(int value);

    int getHue() const;
    bool getBW() const;

    int getBasenote() const;
    void setBasenote(Pitch *p);

    int getOct() const;
    void setOct(int value);

    QString getBasenoteString(int lang) const;

public slots:
    void pitchChange();

private:
    const float freq_a=440;

    float freq;
    int midinote;
    Pitch * pitch;
    int oct;

    float calcFreq(int midinote, int pitch);
    float calcMidi2Fequal(int x);
    double Log2(double n);
    int calcOctFromMidinote(int m);
    int calcBasenoteFromMidinote(int m);
    void initFreq();
};

#endif // FREQTRIPLE_H

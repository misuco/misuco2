#include "playfield.h"
#include "lib/misulib/models/colormacros.h"
#include <QDebug>

Playfield::Playfield(QObject *parent) : QObject(parent),
    f1(0,this),
    f2(0,this),
    hue1bent(0),
    hue2bent(0)
{}

void Playfield::setF1rootNote(int r)
{
    f1.setRootNote(r);
    calcColor();
    calcText();
}

void Playfield::setF2rootNote(int r)
{
    f2.setRootNote(r);
    calcColor();
    calcText();
}

void Playfield::setF2midiNote(int midiNote, int pitch)
{
    f2.setMidinote(midiNote,pitch);
    calcColor();
    calcText();
}

float Playfield::getF2freq()
{
    return f2.getFreq();
}

int Playfield::getF2midiNote()
{
    return f2.getMidinote();
}

int Playfield::getF2pitch()
{
    return f2.getPitch();
}

void Playfield::setHue2Bent(float h)
{
    if(h>1) h-=1;
    if(h<0) h+=1;
    hue2bent = h;
    calcColor();
}

float Playfield::getF2Hue()
{
    return f2.getHue();
}

void Playfield::setType(int t)
{
    type=t;
}

int Playfield::getType()
{
    return type;
}

void Playfield::setPressed(int v)
{
    pressed = v;
    calcColor();
}

void Playfield::incPressed()
{
    pressed++;
    calcColor();
}

void Playfield::decPressed()
{
    pressed--;
    calcColor();
}

int Playfield::getPressed()
{
    return pressed;
}

void Playfield::setF1midiNote(int midiNote, int pitch)
{
    f1.setMidinote(midiNote,pitch);
    calcColor();
    calcText();
}

float Playfield::getF1freq()
{
    return f1.getFreq();
}

int Playfield::getF1midiNote()
{
    return f1.getMidinote();
}

int Playfield::getF1pitch()
{
    return f1.getPitch();
}

void Playfield::setHue1Bent(float h)
{
    if(h>1) h-=1;
    if(h<0) h+=1;
    hue1bent = h;
    calcColor();
}

float Playfield::getF1Hue()
{
    return f1.getHue();
}

void Playfield::calcColor()
{
    //qDebug() << "MWPlayfield::calcColor " << pressed << " " << this;

    float l=LIGHTNESS_OFF;
    float s=SATURATION_OFF;
    if(pressed>0) {
        l=LIGHTNESS_ON;
        s=SATURATION_ON;
    }

    if(_bwmode) {
        if(f1.getBW()) {
            if(pressed>0) {
                colorF1 = PIANO_WHITE_HL;
                colorF1b = PIANO_BLACK_HL;
            } else {
                colorF1 = PIANO_WHITE;
                colorF1b = PIANO_BLACK;
            }
        } else {
            if(pressed>0) {
                colorF1 = PIANO_BLACK_HL;
                colorF1b = PIANO_WHITE_HL;
            } else {
                colorF1 = PIANO_BLACK;
                colorF1b = PIANO_WHITE;
            }
        }
        if(f2.getBW()) {
            if(pressed>0) {
                colorF2 = PIANO_WHITE_HL;
                colorF2b = PIANO_BLACK_HL;
            } else {
                colorF2 = PIANO_WHITE;
                colorF2b = PIANO_BLACK;
            }
        } else {
            if(pressed>0) {
                colorF2 = PIANO_BLACK_HL;
                colorF2b = PIANO_WHITE_HL;
            } else {
                colorF2 = PIANO_BLACK;
                colorF2b = PIANO_WHITE;
            }
        }
    } else {
        colorF1 = QColor::fromHslF(f1.getHue(),s,l);
        colorF1b = QColor::fromHslF(hue1bent,s,l);
        colorF2 = QColor::fromHslF(f2.getHue(),s,l);
        colorF2b = QColor::fromHslF(hue2bent,s,l);
    }

    if(pressed>0) {
        fontColor=HLCOLOR;
    } else {
        fontColor=FGCOLOR;
    }
    emit colorChanged();
}

void Playfield::calcText()
{
    text1="";
    text2=f1.getRootNoteString(_noteSymbols);
    if(type==0) text3.sprintf("%d",f1.getOct());
    else text3="";
    if(_showFreqs) {
        text1.sprintf("%4.1f",f1.getFreq());
        if(type==1 || type==2) {
            text1.sprintf("%4.1f",f1.getFreq());
            text3.sprintf("%4.1f",f2.getFreq());
        }
    }
    emit textChanged();
}

void Playfield::onPitchChange(int rootNote, int pitch)
{
    f1.onPitchChange(rootNote,pitch);
    f2.onPitchChange(rootNote,pitch);
}

void Playfield::setBwMode(bool state)
{
    _bwmode = state;
    calcColor();
}

void Playfield::setNoteSymbols(int symbols)
{
    _noteSymbols = symbols;
    calcText();
}

void Playfield::showFreqs(bool state)
{
    _showFreqs = state;
    calcText();
}

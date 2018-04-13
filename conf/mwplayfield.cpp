#include "mwplayfield.h"
#include "conf/colormacros.h"
#include <QDebug>

MWPlayfield::MWPlayfield(QObject *parent) : QObject(parent),
    f1(0,this),
    f2(0,this)
{}

void MWPlayfield::setF1rootNote(int r)
{
    f1.setRootNote(r);
}

void MWPlayfield::setF2rootNote(int r)
{
    f2.setRootNote(r);
}

void MWPlayfield::setF2midiNote(int midiNote, int pitch)
{
    f2.setMidinote(midiNote,pitch);
}

float MWPlayfield::getF2freq()
{
    return f2.getFreq();
}

int MWPlayfield::getF2midiNote()
{
    return f2.getMidinote();
}

int MWPlayfield::getF2pitch()
{
    return f2.getPitch();
}

void MWPlayfield::setHue2Bent(float h)
{
    if(h>1) h-=1;
    if(h<0) h+=1;
    hue2bent = h;
}

float MWPlayfield::getF2Hue()
{
    return f2.getHue();
}

void MWPlayfield::setType(int t)
{
    type=t;
}

int MWPlayfield::getType()
{
    return type;
}

void MWPlayfield::setPressed(int v)
{
    pressed = v;
    calcColor();
}

void MWPlayfield::incPressed()
{
    pressed++;
    calcColor();
}

void MWPlayfield::decPressed()
{
    pressed--;
    calcColor();
}

int MWPlayfield::getPressed()
{
    return pressed;
}

void MWPlayfield::setF1midiNote(int midiNote, int pitch)
{
    f1.setMidinote(midiNote,pitch);
}

float MWPlayfield::getF1freq()
{
    return f1.getFreq();
}

int MWPlayfield::getF1midiNote()
{
    return f1.getMidinote();
}

int MWPlayfield::getF1pitch()
{
    return f1.getPitch();
}

void MWPlayfield::setHue1Bent(float h)
{
    if(h>1) h-=1;
    if(h<0) h+=1;
    hue1bent = h;
}

float MWPlayfield::getF1Hue()
{
    return f1.getHue();
}

void MWPlayfield::calcColor()
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

void MWPlayfield::calcText()
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

void MWPlayfield::onPitchChange(int rootNote, int pitch)
{
    f1.onPitchChange(rootNote,pitch);
    f2.onPitchChange(rootNote,pitch);
}

void MWPlayfield::setBwMode(bool state)
{
    _bwmode = state;
    calcColor();
}

void MWPlayfield::setNoteSymbols(int symbols)
{
    _noteSymbols = symbols;
    calcText();
}

void MWPlayfield::showFreqs(bool state)
{
    _showFreqs = state;
    calcText();
}

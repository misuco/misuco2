#include "mwplayfield.h"
#include "conf/colormacros.h"
#include <QDebug>

MWPlayfield::MWPlayfield(QObject *parent) : QObject(parent)
{

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
        if(f1->getBW()) {
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
        if(f2->getBW()) {
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
        colorF1 = QColor::fromHslF(f1->getHue(),s,l);
        colorF1b = QColor::fromHslF(hue1bent,s,l);
        colorF2 = QColor::fromHslF(f2->getHue(),s,l);
        colorF2b = QColor::fromHslF(hue2bent,s,l);
    }

    if(pressed>0) {
        fontColor=HLCOLOR;
    } else {
        fontColor=FGCOLOR;
    }
    emit colorChanged();

    text1="";
    text2=f1->getRootNoteString(_noteSymbols);
    if(type==0) text3.sprintf("%d",f1->getOct());
    else text3="";
    if(_showFreqs) {
        text1.sprintf("%4.1f",f1->getFreq());
        if(type==1 || type==2) {
            text1.sprintf("%4.1f",f1->getFreq());
            text3.sprintf("%4.1f",f2->getFreq());
        }
    }

    emit textChanged();

}

void MWPlayfield::onBwModeChanged(bool bwMode)
{
    _bwmode = bwMode;
    calcColor();
}

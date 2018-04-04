#include "mwplayfield.h"
#include "conf/mglob.h"
#include <QDebug>

MWPlayfield::MWPlayfield(QObject *parent) : QObject(parent)
{

}

void MWPlayfield::calcColor()
{

    //qDebug() << "MWPlayfield::calcColor " << pressed << " " << this;

    float l=MGlob::lOff;
    float s=MGlob::sOff;
    if(pressed>0) {
        l=MGlob::lOn;
        s=MGlob::sOn;
    }

    colorF1 = QColor::fromHslF(f1->getHue(),s,l);
    colorF1b = QColor::fromHslF(hue1bent,s,l);
    colorF2 = QColor::fromHslF(f2->getHue(),s,l);
    colorF2b = QColor::fromHslF(hue2bent,s,l);

    if(MGlob::bwmode) {
        if(f1->getBW()) {
            if(pressed>0) {
                colorF1 = MGlob::hlwkeycolor;
                colorF1b = MGlob::hlbkeycolor;
            } else {
                colorF1 = MGlob::wkeycolor;
                colorF1b = MGlob::bkeycolor;
            }
        } else {
            if(pressed>0) {
                colorF1 = MGlob::hlbkeycolor;
                colorF1b =MGlob:: hlwkeycolor;
            } else {
                colorF1 = MGlob::bkeycolor;
                colorF1b = MGlob::wkeycolor;
            }
        }
        if(f2->getBW()) {
            if(pressed>0) {
                colorF2 = MGlob::hlwkeycolor;
                colorF2b = MGlob::hlbkeycolor;
            } else {
                colorF2 = MGlob::wkeycolor;
                colorF2b = MGlob::bkeycolor;
            }
        } else {
            if(pressed>0) {
                colorF2 = MGlob::hlbkeycolor;
                colorF2b = MGlob::hlwkeycolor;
            } else {
                colorF2 = MGlob::bkeycolor;
                colorF2b = MGlob::wkeycolor;
            }
        }
    }

    if(pressed>0) {
        fontColor=MGlob::highlightcolor;
    } else {
        fontColor=MGlob::fgcolor;
    }
    emit colorChanged();

    text1="";
    text2=f1->getRootNoteString(MGlob::noteSymbols);
    if(type==0) text3.sprintf("%d",f1->getOct());
    else text3="";
    if(MGlob::showFreqs) {
        text1.sprintf("%4.1f",f1->getFreq());
        if(type==1 || type==2) {
            text1.sprintf("%4.1f",f1->getFreq());
            text3.sprintf("%4.1f",f2->getFreq());
        }
    }

    emit textChanged();

}

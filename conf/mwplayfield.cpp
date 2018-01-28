#include "mwplayfield.h"
#include "widgets/misuwidget.h"
#include <QDebug>

MWPlayfield::MWPlayfield(QObject *parent) : QObject(parent)
{

}

void MWPlayfield::calcColor()
{

    //qDebug() << "MWPlayfield::calcColor " << pressed << " " << this;

    float l=MisuWidget::lOff;
    float s=MisuWidget::sOff;
    if(pressed>0 || hold) {
        l=MisuWidget::lOn;
        s=MisuWidget::sOn;
    }

    colorF1 = QColor::fromHslF(f1->getHue(),s,l);
    colorF1b = QColor::fromHslF(hue1bent,s,l);
    colorF2 = QColor::fromHslF(f2->getHue(),s,l);
    colorF2b = QColor::fromHslF(hue2bent,s,l);

    if(MisuWidget::bwmode) {
        if(f1->getBW()) {
            if(pressed>0 || hold) {
                colorF1 = MisuWidget::hlwkeycolor;
                colorF1b = MisuWidget::hlbkeycolor;
            } else {
                colorF1 = MisuWidget::wkeycolor;
                colorF1b = MisuWidget::bkeycolor;
            }
        } else {
            if(pressed>0 || hold) {
                colorF1 = MisuWidget::hlbkeycolor;
                colorF1b =MisuWidget:: hlwkeycolor;
            } else {
                colorF1 = MisuWidget::bkeycolor;
                colorF1b = MisuWidget::wkeycolor;
            }
        }
        if(f2->getBW()) {
            if(pressed>0 || hold) {
                colorF2 = MisuWidget::hlwkeycolor;
                colorF2b = MisuWidget::hlbkeycolor;
            } else {
                colorF2 = MisuWidget::wkeycolor;
                colorF2b = MisuWidget::bkeycolor;
            }
        } else {
            if(pressed>0 || hold) {
                colorF2 = MisuWidget::hlbkeycolor;
                colorF2b = MisuWidget::hlwkeycolor;
            } else {
                colorF2 = MisuWidget::bkeycolor;
                colorF2b = MisuWidget::wkeycolor;
            }
        }
    }

    if(pressed>0 || hold) {
        fontColor=MisuWidget::highlightcolor;
    } else {
        fontColor=MisuWidget::fgcolor;
    }
    emit colorChanged();

    text1="";
    text2=f1->getRootNoteString(MisuWidget::noteSymbols);
    if(type==0) text3.sprintf("%d",f1->getOct());
    else text3="";
    if(MisuWidget::showFreqs) {
        text1.sprintf("%4.1f",f1->getFreq());
        if(type==1 || type==2) {
            text1.sprintf("%4.1f",f1->getFreq());
            text3.sprintf("%4.1f",f2->getFreq());
        }
    }

    emit textChanged();

}

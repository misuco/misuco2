#include <QDebug>
#include "pitchcolor.h"
#include "lib/misulib/models/colormacros.h"

PitchColor::PitchColor(int rootNote, QObject *parent) : QObject(parent),
    _rootNote(rootNote),
    _pitch(0),
    _bwMode(false),
    _pianoWhite(true)
{
    if( _rootNote == 1 ||
        _rootNote == 3 ||
        _rootNote == 6 ||
        _rootNote == 8 ||
        _rootNote == 10 ) _pianoWhite = false;
    calcColor();
}

void PitchColor::onPitchChange(int rootNote, int value)
{
    if(rootNote == _rootNote) {
        _pitch = value;
        calcColor();
    }

}

void PitchColor::onBwModeChange(bool bwMode)
{
    _bwMode = bwMode;
    calcColor();
}

void PitchColor::calcColor()
{
    float hue = (float)_rootNote / 12 + (float)_pitch / 2400;
    if(hue>1) hue -=1;
    if(hue<0) hue +=1;

    if(_bwMode) {
        if(_pianoWhite) {
            _colorOff = PIANO_WHITE;
            _colorOn = PIANO_WHITE_HL;
        } else {
            _colorOff = PIANO_BLACK;
            _colorOn = PIANO_BLACK_HL;
        }
    } else {
        //qDebug() << "PitchColor::calcColor " << hue;
        _colorOff = QColor::fromHslF(hue,SATURATION_OFF,LIGHTNESS_OFF);
        _colorOn = QColor::fromHslF(hue,SATURATION_ON,LIGHTNESS_ON);
    }
    emit colorChanged();
}

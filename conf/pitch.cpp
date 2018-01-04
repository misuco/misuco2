#include <QDebug>
#include "pitch.h"

Pitch::Pitch(QObject *parent) : QObject(parent)
{
    basenote=0;
    color=0;
}

void Pitch::setBasenote(int value)
{
    basenote = value;
    calcColor();
    emit change();
}

int Pitch::getHue() const
{
    return color;
}

int Pitch::getBW() const
{
    bool white = true;
    int bn = basenote;
    if(bn == 1 || bn == 3 || bn == 6 || bn == 8 || bn == 10 ) white = false;
    return white;
}

void Pitch::calcColor()
{
    color = basenote * 30 + pitch * 15 / 100;
    if(color>360) color -=360;
    if(color<0) color +=360;

    //qDebug() << "Pitch::calcColor " << color << " from basenote " << basenote << " and pitch " << pitch;
}

void Pitch::setPitch(int value)
{
    pitch = value;
    calcColor();
    emit change();
}

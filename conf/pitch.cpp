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

void Pitch::calcColor()
{
    color=basenote*30;
}

void Pitch::setPitch(int value)
{
    pitch = value;
    basenote = value;
    calcColor();
    emit change();
}

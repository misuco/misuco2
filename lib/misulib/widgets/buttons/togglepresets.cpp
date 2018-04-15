#include "togglepresets.h"

TogglePresets::TogglePresets(QString text, int state, QObject * parent) : ButtonBase(text,state,parent)
{}

void TogglePresets::releaseAction()
{
    if(_state == 0) {
        _state = 1;
    } else {
        _state = 0;
    }
    emit togglePresets(_state);
}

void TogglePresets::setAction()
{
    emit togglePresets(_state);
}

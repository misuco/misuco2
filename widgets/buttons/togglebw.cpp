#include "togglebw.h"

ToggleBw::ToggleBw(QString text, int state, QObject * parent) : ButtonBase(text,state,parent)
{}

void ToggleBw::releaseAction()
{
    if(_state == 0) {
        _state = 1;
    } else {
        _state = 0;
    }
    emit toggleBW(_state);
}

void ToggleBw::setAction()
{
    emit toggleBW(_state);
}

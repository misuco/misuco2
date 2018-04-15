#include "togglemenu.h"

ToggleMenu::ToggleMenu(QString text, int state, QObject * parent) : ButtonBase(text,state,parent)
{}

void ToggleMenu::releaseAction()
{
    if(_state == 0) {
        _state = 1;
    } else {
        _state = 0;
    }
    emit toggleMenu(_state);
}

void ToggleMenu::setAction()
{
    emit toggleMenu(_state);
}

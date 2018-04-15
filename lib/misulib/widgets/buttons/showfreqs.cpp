#include "showfreqs.h"

ShowFreqs::ShowFreqs(QString text, int state, QObject * parent) : ButtonBase(text,state,parent)
{}

void ShowFreqs::releaseAction()
{
    if(_state == 0) {
        _state = 1;
    } else {
        _state = 0;
    }
    emit toggleShowFreqs(_state);
}

void ShowFreqs::setAction()
{
    emit toggleShowFreqs(_state);
}

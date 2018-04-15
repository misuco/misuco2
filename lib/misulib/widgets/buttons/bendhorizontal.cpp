#include "bendhorizontal.h"

BendHorizontal::BendHorizontal(QString text, int state, QObject * parent) : ButtonBase(text,state,parent)
{}

void BendHorizontal::releaseAction()
{
    if(_state == 0) {
        _state = 1;
    } else {
        _state = 0;
    }
    emit setBendHori(_state);
}

void BendHorizontal::setAction()
{
    emit setBendHori(_state);
}

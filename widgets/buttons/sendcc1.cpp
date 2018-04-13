#include "sendcc1.h"

SendCc1::SendCc1(QString text, int state, QObject * parent) : ButtonBase(text,state,parent)
{}

void SendCc1::releaseAction()
{
    if(_state == 0) {
        _state = 1;
    } else {
        _state = 0;
    }
    emit sendCc1(_state);
}

void SendCc1::setAction()
{
    emit sendCc1(_state);
}

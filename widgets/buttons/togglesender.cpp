#include "togglesender.h"

ToggleSender::ToggleSender(QString text, int senderId, int state, QObject * parent) : ButtonBase(text,state,parent),
    _senderId(senderId)
{}

void ToggleSender::releaseAction()
{
    if(_state == 0) {
        _state = 1;
    } else {
        _state = 0;
    }
    emit toggleSender(_senderId,_state);
}

void ToggleSender::setAction()
{
    emit toggleSender(_senderId,_state);
}

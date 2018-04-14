#include "octaveshift.h"

OctaveShift::OctaveShift(QString text, bool up, QObject * parent) : ButtonBase(text,0,parent),
    _up(up)
{}

void OctaveShift::releaseAction()
{
    if(_up) emit octUp();
    else emit octDown();
}

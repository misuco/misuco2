#include "toggleheader.h"

ToggleHeader::ToggleHeader(QString text, int headerId, QObject * parent) : ButtonBase(text,0,parent),
    _headerId(headerId)
{}

void ToggleHeader::releaseAction()
{
    emit toggleHeader(_headerId);
}

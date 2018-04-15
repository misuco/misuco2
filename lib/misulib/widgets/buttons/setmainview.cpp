#include "setmainview.h"

SetMainView::SetMainView(QString text, int viewId, QObject * parent) : ButtonBase(text,0,parent),
    _viewId(viewId)
{}

void SetMainView::releaseAction()
{
    emit setMainView(_viewId);
}

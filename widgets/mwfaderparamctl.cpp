#include "mwfaderparamctl.h"
#include <QDebug>

MWFaderParamCtl::MWFaderParamCtl(QWidget *parent, Color * col,int cc, int c) : MWFadder(parent, col), cc(cc), chan(c)
{
}

MWFaderParamCtl::~MWFaderParamCtl()
{
}

void MWFaderParamCtl::setOut(ISender *value)
{
    out = value;
}

void MWFaderParamCtl::setChan(int c)
{
    chan=c;
}

void MWFaderParamCtl::processTouchEvent(misuTouchEvent e)
{
    MWFadder::processTouchEvent(e);
    switch(e.state) {
    case Qt::TouchPointPressed:
        out->cc(chan,0,cc,getValue(),getValue());
        qDebug() << "MWFaderParamCtl::processTouchEvent TouchPointPressed " << out << " cc:" << cc << " value: " << getValue();
        pressed++;
        update();
        break;
    case Qt::TouchPointMoved:
        out->cc(chan,0,cc,getValue(),getValue());
        update();
        break;
    case Qt::TouchPointReleased:
        qDebug() << "MWFaderParamCtl::processTouchEvent TouchPointReleased cc:" << cc;
        pressed--;
        update();
        break;
    }
}

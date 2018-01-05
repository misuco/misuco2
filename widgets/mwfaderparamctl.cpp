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

void MWFaderParamCtl::setValue(int v)
{
    MWFadder::setValue(v);
    propagateValueChange();
    update();
}

void MWFaderParamCtl::setChan(int c)
{
    chan=c;
}

void MWFaderParamCtl::propagateValueChange() {
    out->cc(chan,0,cc,getValue(),getValue());
    switch(cc) {
    case 102:
        Sound.wave_type=getValue();
        break;
    case 103:
        Sound.attack=getValue();
        break;
    case 104:
        Sound.decay=getValue();
        break;
    case 105:
        Sound.sustain=getValue();
        break;
    case 106:
        Sound.release=getValue();
        break;
    case 107:
        Sound.filter_cutoff=getValue();
        break;
    case 108:
        Sound.filter_resonance=getValue();
        break;
    case 109:
        Sound.mod_filter_cutoff=getValue();
        break;
    case 110:
        Sound.mod_filter_resonance=getValue();
        break;
    case 111:
        Sound.volume=getValue();
        break;
    }
}

void MWFaderParamCtl::processTouchEvent(misuTouchEvent e)
{
    MWFadder::processTouchEvent(e);
    switch(e.state) {
    case Qt::TouchPointPressed:
        //qDebug() << "MWFaderParamCtl::processTouchEvent TouchPointPressed " << out << " cc:" << cc << " value: " << getValue();
        propagateValueChange();
        pressed++;
        update();
        break;
    case Qt::TouchPointMoved:
        out->cc(chan,0,cc,getValue(),getValue());
        update();
        break;
    case Qt::TouchPointReleased:
        //qDebug() << "MWFaderParamCtl::processTouchEvent TouchPointReleased cc:" << cc;
        pressed--;
        update();
        break;
    }
}

#include "mwsoundpreset.h"
#include <QPainter>
#include <QDebug>

MWSoundPreset::MWSoundPreset(QWidget *parent): MisuWidget(parent)
{
    PresetSound.volume=800;
    PresetSound.wave_type=qrand() % 4;
    PresetSound.attack=qrand() % 100;
    PresetSound.decay=qrand() % 700;
    PresetSound.sustain=qrand() % 1000;
    PresetSound.release=qrand() % 1000;
    PresetSound.filter_cutoff=qrand() % 1000;
    PresetSound.filter_resonance=qrand() % 100;
    PresetSound.mod_filter_cutoff=50;
    PresetSound.mod_filter_resonance=0;

}

void MWSoundPreset::processTouchEvent(MisuWidget::misuTouchEvent e)
{
    switch(e.state) {
    case Qt::TouchPointPressed:
        if(overwrite) {
            PresetSound.volume=Sound.volume;
            PresetSound.wave_type=Sound.wave_type;
            PresetSound.attack=Sound.attack;
            PresetSound.decay=Sound.decay;
            PresetSound.sustain=Sound.sustain;
            PresetSound.release=Sound.release;
            PresetSound.filter_cutoff=Sound.filter_cutoff;
            PresetSound.filter_resonance=Sound.filter_resonance;
            PresetSound.mod_filter_cutoff=Sound.mod_filter_cutoff;
            PresetSound.mod_filter_resonance=Sound.mod_filter_resonance;
        }
        else {
            emit setSound(&PresetSound);
        }
        pressed++;
        break;
    case Qt::TouchPointReleased:
        pressed--;
        break;
    }
    update();
}

void MWSoundPreset::paintEvent(QPaintEvent *E)
{
    QPainter painter(this);

    int xr=width()/12;
    int h=height();

    painter.setPen(fgcolor);
    painter.setBrush(bgcolor);
    painter.drawRect(0,0,width(),height());

    switch(PresetSound.wave_type) {
    case 0:
        painter.drawLine(0,h,xr,0);
        painter.drawLine(xr,0,xr*2,h);
        break;
    case 1:
        painter.drawLine(xr,h,xr*2,0);
        painter.drawLine(xr*3,0,xr*4,h);
        break;
    case 2:
        painter.drawLine(0,0,xr*2,h);
        break;
    case 3:
        painter.drawLine(0,h,xr*2,0);
        painter.drawLine(xr*2,0,xr*2,h);
        break;
    default:
        painter.drawLine(0,h,xr,0);
        painter.drawLine(xr,0,xr*2,h);
        painter.drawLine(xr*2,h,xr*3,0);
        break;
    }

    int xenv = xr*4;
    int xdiff=xr*PresetSound.attack/1000;
    painter.drawLine(xenv,h,xenv+xdiff,0);

    xenv+=xdiff;
    xdiff=xr*PresetSound.decay/1000;
    int ydiff=PresetSound.sustain*h/1000;
    painter.drawLine(xenv,0,xenv+xdiff,ydiff);

    xenv+=xdiff;
    painter.drawLine(xenv,ydiff,xenv+xr,ydiff);

    xenv+=xr;
    xdiff=xr*PresetSound.release/1000;
    painter.drawLine(xenv,ydiff,xenv+xdiff,h);




}

void MWSoundPreset::resizeEvent(QResizeEvent *E)
{

}

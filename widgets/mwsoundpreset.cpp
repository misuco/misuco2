#include "mwsoundpreset.h"
#include <QPainter>
#include <QDebug>

MWSoundPreset::MWSoundPreset(QWidget *parent): MisuWidget(parent)
{
    PresetSound.volume=80;
    PresetSound.wave_type=1;
    PresetSound.attack=1;
    PresetSound.decay=20;
    PresetSound.sustain=60;
    PresetSound.release=40;
    PresetSound.filter_cutoff=50;
    PresetSound.filter_resonance=10;
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
}

void MWSoundPreset::resizeEvent(QResizeEvent *E)
{

}

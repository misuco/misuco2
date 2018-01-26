/*
 * Copyright (c) 2018 by misuco.org, Claudio Zopfi, Zurich, Switzerland, c@misuco.org
 *
 * This file is part of MISUCO2.
 *
 * MISUCO2 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * MISUCO2 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with MISUCO2.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "mwpreset.h"
#include <QPainter>
#include <QDebug>

MWPreset::MWPreset(QWidget *parent): MisuWidget(parent)
{
    PresetScale.rootNote=qrand() % (BSCALE_SIZE+1);
    PresetScale.baseoct=4;
    PresetScale.topoct=5;
    PresetScale.size=2;
    for(int i=0;i<BSCALE_SIZE;i++) {
        if(qrand() % 2) {
            PresetScale.bscale[i]=true;
            PresetScale.size+=PresetScale.topoct-PresetScale.baseoct;
        } else {
            PresetScale.bscale[i]=false;
        }
    }
}

MWPreset::MWPreset(int rootNote, int baseoct, int topoct, bool bscale[], QObject *parent): MisuWidget(parent)
{
    PresetScale.rootNote=rootNote;
    PresetScale.baseoct=baseoct;
    PresetScale.topoct=topoct;
    PresetScale.size=2;
    for(int i=0;i<BSCALE_SIZE;i++) {
        if(bscale[i]) {
            PresetScale.bscale[i]=true;
            PresetScale.size+=PresetScale.topoct-PresetScale.baseoct;
        } else {
            PresetScale.bscale[i]=false;
        }
    }
}

QStringList MWPreset::bscale()
{
    QStringList l;
    for(int i=0;i<BSCALE_SIZE;i++) {
        if(PresetScale.bscale[i]) l.append("1");
        else l.append("0");
    }
    return l;
}

int MWPreset::bscalesize()
{
    return PresetScale.size;
}

int MWPreset::rootnote()
{
    return PresetScale.rootNote;
}

void MWPreset::onPressed()
{
    if(overwrite) {
        PresetScale.rootNote = MisuWidget::Scale.rootNote;
        PresetScale.baseoct = MisuWidget::Scale.baseoct;
        PresetScale.topoct = MisuWidget::Scale.topoct;
        PresetScale.size = 2;

        //qDebug() << "MWPreset::processTouchEvent " << Scale.rootNote << " note " << Scale.size << " bo " << Scale.baseoct << " to " << Scale.topoct;
        for(int i=0;i<BSCALE_SIZE;i++) {
            PresetScale.bscale[i]=MisuWidget::Scale.bscale[i];
            if(PresetScale.bscale[i]) {
                PresetScale.size+=PresetScale.topoct-PresetScale.baseoct;
            }
            //qDebug() << i << " " << Scale.bscale[i];
        }
    }
    else {
        emit setScale(&PresetScale);
        emit scaleupdate();
    }
    pressed++;
}

void MWPreset::onReleased()
{
    pressed--;
}

void MWPreset::initialSet()
{
    emit setScale(&PresetScale);
    emit scaleupdate();
}

void MWPreset::playAreaChanged()
{
    emit presetChanged();
}

/*
void MWPreset::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    float colwidth=(float)width()/(float)PresetScale.size;
    float x=0;
    int l=lOff;
    int s=sOff;
    if(pressed>0 || isSelected()) {
        l=lOn;
        s=sOn;
    }
    painter.setPen(Qt::NoPen);
    for(int i=PresetScale.baseoct;i<PresetScale.topoct;i++) {
        setBrush(MWPitch[PresetScale.rootNote],s,l,painter);
        painter.drawRect(x,0,colwidth,height());
        x+=colwidth;
        for(int j=0;j<BSCALE_SIZE;j++) {
            if(PresetScale.bscale[j]) {
                setBrush(MWPitch[(PresetScale.rootNote+1+j)%(BSCALE_SIZE+1)],s,l,painter);
                painter.drawRect(x,0,colwidth+1,height());
                x+=colwidth;
            }
        }
    }
    //painter.setBrush(QColor::fromHsl(MWPitch[Scale.rootNote]->color,s,l));
    setBrush(MWPitch[PresetScale.rootNote],s,l,painter);
    painter.drawRect(x,0,colwidth,height());
}

void MWPreset::setBrush(Pitch * p, int s, int l, QPainter &painter) {
    if(bwmode) {
        if(isSelected()) {
            if(p->getBW()) {
                painter.setBrush(hlwkeycolor);
            } else {
                painter.setBrush(hlbkeycolor);
            }
        } else if(p->getBW()) {
            painter.setBrush(wkeycolor);
        } else {
            painter.setBrush(bkeycolor);
        }
    } else {
        painter.setBrush(QColor::fromHsl(p->color,s,l));
    }
}

void MWPreset::resizeEvent(QResizeEvent *)
{

}
*/

bool MWPreset::isSelected()
{
    if( PresetScale.rootNote!=Scale.rootNote ||
        PresetScale.baseoct!=Scale.baseoct ||
        PresetScale.topoct!=Scale.topoct ) return false;
    for(int i=0;i<BSCALE_SIZE;i++) {
        if( PresetScale.bscale[i]!=Scale.bscale[i] ) return false;
    }
    return true;
}

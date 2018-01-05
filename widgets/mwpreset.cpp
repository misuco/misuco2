#include "mwpreset.h"
#include <QPainter>
#include <QDebug>

MWPreset::MWPreset(Pitch *p[], QWidget *parent): MisuWidget(parent)
{
    MWPitch=p;

    PresetScale.basenote=qrand() % (BSCALE_SIZE+1);
    PresetScale.baseoct=3;
    PresetScale.topoct=4;
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

void MWPreset::processTouchEvent(MisuWidget::misuTouchEvent e)
{
    switch(e.state) {
    case Qt::TouchPointPressed:
        if(overwrite) {
            PresetScale.basenote = MisuWidget::Scale.basenote;
            PresetScale.baseoct = MisuWidget::Scale.baseoct;
            PresetScale.topoct = MisuWidget::Scale.topoct;
            PresetScale.size = 2;

            //qDebug() << "MWPreset::processTouchEvent " << Scale.basenote << " note " << Scale.size << " bo " << Scale.baseoct << " to " << Scale.topoct;
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
        }
        pressed++;
        break;
    case Qt::TouchPointReleased:
        pressed--;
        break;
    }
    update();
}

void MWPreset::paintEvent(QPaintEvent *E)
{
    QPainter painter(this);
    float colwidth=(float)width()/(float)PresetScale.size;
    float x=0;
    int l=lOff;
    int s=sOff;
    if(pressed>0) {
        l=lOn;
        s=sOn;
    }
    painter.setPen(Qt::NoPen);
    for(int i=PresetScale.baseoct;i<PresetScale.topoct;i++) {
        setBrush(MWPitch[PresetScale.basenote],s,l,painter);
        painter.drawRect(x,0,colwidth,height());
        x+=colwidth;
        for(int j=0;j<BSCALE_SIZE;j++) {
            if(PresetScale.bscale[j]) {
                setBrush(MWPitch[(PresetScale.basenote+1+j)%(BSCALE_SIZE+1)],s,l,painter);
                painter.drawRect(x,0,colwidth,height());
                x+=colwidth;
            }
        }
    }
    //painter.setBrush(QColor::fromHsl(MWPitch[Scale.basenote]->color,s,l));
    setBrush(MWPitch[PresetScale.basenote],s,l,painter);
    painter.drawRect(x,0,colwidth,height());
}

void MWPreset::setBrush(Pitch * p, int s, int l, QPainter &painter) {
    if(bwmode) {
        if(p->getBW()) {
            painter.setBrush(wkeycolor);
        } else {
            painter.setBrush(bkeycolor);
        }
    } else {
        painter.setBrush(QColor::fromHsl(p->color,s,l));
    }
}


void MWPreset::resizeEvent(QResizeEvent *E)
{

}

#include "mwpreset.h"
#include <QPainter>

MWPreset::MWPreset(Pitch *p[], QWidget *parent): MisuWidget(parent)
{
    MWPitch=p;

    Scale.basenote=qrand() % (BSCALE_SIZE+1);
    Scale.baseoct=3;
    Scale.topoct=4;
    Scale.size=2;
    for(int i=0;i<BSCALE_SIZE;i++) {
        if(qrand() % 2) {
            Scale.bscale[i]=true;
            Scale.size+=Scale.topoct-Scale.baseoct;
        } else {
            Scale.bscale[i]=false;
        }
    }
}

void MWPreset::processTouchEvent(MisuWidget::misuTouchEvent e)
{
    switch(e.state) {
    case Qt::TouchPointPressed:
        emit setScale(&Scale);
        pressed++;
        break;
    case Qt::TouchPointReleased:
        pressed--;
        break;
    }
}

void MWPreset::paintEvent(QPaintEvent *E)
{
    QPainter painter(this);
    float colwidth=(float)width()/(float)Scale.size;
    float x=0;
    int l=180;
    if(pressed>0) l=230;
    painter.setPen(Qt::NoPen);
    for(int i=Scale.baseoct;i<Scale.topoct;i++) {
        setBrush(MWPitch[Scale.basenote],l,painter);
        painter.drawRect(x,0,colwidth,height());
        x+=colwidth;
        for(int j=0;j<BSCALE_SIZE;j++) {
            if(Scale.bscale[j]) {
                setBrush(MWPitch[(Scale.basenote+1+j)%(BSCALE_SIZE+1)],l,painter);
                painter.drawRect(x,0,colwidth,height());
                x+=colwidth;
            }
        }
    }
    painter.setBrush(QColor::fromHsl(MWPitch[Scale.basenote]->color,180,l));
    setBrush(MWPitch[Scale.basenote],l,painter);
    painter.drawRect(x,0,colwidth,height());
}

void MWPreset::setBrush(Pitch * p, int l, QPainter &painter) {
    if(bwmode) {
        if(p->getBW()) {
            painter.setBrush(wkeycolor);
        } else {
            painter.setBrush(bkeycolor);
        }
    } else {
        painter.setBrush(QColor::fromHsl(p->color,180,l));
    }
}


void MWPreset::resizeEvent(QResizeEvent *E)
{

}

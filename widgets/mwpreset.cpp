#include "mwpreset.h"
#include <QPainter>

MWPreset::MWPreset(Pitch *p[], QWidget *parent): MisuWidget(parent)
{
    MWPitch=p;

    Scale.basenote=qrand() % (BSCALE_SIZE+1);
    Scale.baseoct=3;
    Scale.topoct=4;
    Scale.size=3;
    for(int i=0;i<BSCALE_SIZE;i++) {
        if(qrand() % 2) {
            Scale.bscale[i]=true;
            Scale.size+=2;
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
    painter.setPen(Qt::black);
    for(int i=Scale.baseoct;i<=Scale.topoct;i++) {
        painter.setBrush(QColor::fromHsl(MWPitch[Scale.basenote]->color,180,180));
        painter.drawRect(x,0,colwidth,height());
        x+=colwidth;
        for(int j=0;j<BSCALE_SIZE;j++) {
            if(Scale.bscale[j]) {
                painter.setBrush(QColor::fromHsl(MWPitch[(Scale.basenote+1+j)%BSCALE_SIZE]->color,180,180));
                painter.drawRect(x,0,colwidth,height());
                x+=colwidth;
            }
        }
    }
}

void MWPreset::resizeEvent(QResizeEvent *E)
{

}

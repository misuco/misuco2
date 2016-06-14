#include "mwplayarea.h"
#include <QDebug>
#include <QLinearGradient>
#include <QRadialGradient>
#include "comm/senderdebug.h"

MWPlayArea::MWPlayArea(QWidget *parent) : MisuWidget(parent),
    linearGrad(QPointF(0,1),QPointF(0,1))
{
    Scale.basenote=1;
    Scale.baseoct=3;
    Scale.topoct=4;
    for(int i=0;i<BSCALE_SIZE;i++) {
        Scale.bscale[i]=false;
    }
    Scale.bscale[3]=true;
    Scale.bscale[6]=true;
    Scale.bscale[9]=true;

    bendHoriz=true;
    bendVertTop=-1;
    bendVertBot=-1;

    for(int i=0;i<EVENT_STACK_SIZE;i++) {
        eventStack[i].eventId=-1;
        eventStack[i].row=-1;
        eventStack[i].col=-1;
        eventStack[i].f=0;
    }

    QObject::connect(this,SIGNAL(touchEvent(misuTouchEvent)),this,SLOT(processTouchEvent(misuTouchEvent)));

    out = new SenderDebug();

    config();
}

MWPlayArea::~MWPlayArea()
{

}

void MWPlayArea::config()
{
    /*
     *  |.|.||.|.|.| |
     */
    cols=0;
    for(int oct=Scale.baseoct;oct<=Scale.topoct;oct++) {
        setColumn(cols,Scale.basenote+oct*12);
        if(bendHoriz) {
            cols+=2;
        } else {
            cols++;
        }
        for(int note=0;note<BSCALE_SIZE;note++) {
            if(Scale.bscale[note]) {
                setColumn(cols,Scale.basenote+oct*12+note+1);
                if(bendHoriz) {
                    cols+=2;
                } else {
                    cols++;
                }
            }
        }
    }
    int topnote=Scale.basenote+(Scale.topoct+1)*12;
    qDebug() << "basenote: " << Scale.basenote << "topnote: " << topnote;
    setColumn(cols,topnote);
    cols++;
    calcGeo();
}

void MWPlayArea::setColumn(int col, int midinote) {
    qDebug() << "setColumn " << col << " " << midinote;
    rows=0;
    if(bendVertTop!=0) {
        fields[rows][col].type=BEND_VERT;
        if(bendVertTop>0) {
            fields[rows][col].f1.setMidinote(midinote);
            fields[rows][col].f2.setMidinote(midinote+bendVertTop);
        } else {
            fields[rows][col].f2.setMidinote(midinote);
            fields[rows][col].f1.setMidinote(midinote+bendVertTop);
        }
        if(col>1 && bendHoriz) {
            fields[rows][col-1].type=BEND_VERT_HORIZ;
            fields[rows][col-1].f1.setMidinote(fields[rows][col-2].f2.getMidinote());
            fields[rows][col-1].f2.setMidinote(fields[rows][col-2].f2.getMidinote()+bendVertTop);
            fields[rows][col-1].f3.setMidinote(midinote);
            fields[rows][col-1].f4.setMidinote(midinote+bendVertTop);
            fields[rows][col-1].pressed=0;
        }
        fields[rows][col].pressed=0;
        rows++;
    }
    fields[rows][col].type=NORMAL;
    fields[rows][col].f1.setMidinote(midinote);
    fields[rows][col].pressed=0;
    if(col>1 && bendHoriz) {
        fields[rows][col-1].type=BEND_HORIZ;
        fields[rows][col-1].f1.setMidinote(fields[rows][col-2].f1.getMidinote());
        fields[rows][col-1].f2.setMidinote(midinote);
        fields[rows][col-1].pressed=0;
    }
    rows++;
    if(bendVertBot!=0) {
        fields[rows][col].type=BEND_VERT;
        if(bendVertTop>0) {
            fields[rows][col].f2.setMidinote(midinote);
            fields[rows][col].f1.setMidinote(midinote+bendVertBot);
        } else {
            fields[rows][col].f1.setMidinote(midinote);
            fields[rows][col].f2.setMidinote(midinote+bendVertBot);
        }
        if(col>1 && bendHoriz) {
            fields[rows][col-1].type=BEND_VERT_HORIZ;
            if(bendVertTop>0) {
                fields[rows][col-1].f2.setMidinote(fields[rows][col-2].f1.getMidinote());
                fields[rows][col-1].f1.setMidinote(fields[rows][col-2].f1.getMidinote()+bendVertBot);
                fields[rows][col-1].f3.setMidinote(midinote);
                fields[rows][col-1].f4.setMidinote(midinote+bendVertBot);
            } else {
                fields[rows][col-1].f1.setMidinote(fields[rows][col-2].f1.getMidinote());
                fields[rows][col-1].f2.setMidinote(fields[rows][col-2].f1.getMidinote()+bendVertBot);
                fields[rows][col-1].f3.setMidinote(midinote);
                fields[rows][col-1].f4.setMidinote(midinote+bendVertBot);
            }
            fields[rows][col-1].pressed=0;
        }
        fields[rows][col].pressed=0;
        rows++;
    }
}

void MWPlayArea::calcGeo()
{
    int cw=width()/cols;
    int rh=height()/rows;
    for(int i=0;i<cols;i++) {
        colwidth[i]=cw;
    }
    for(int i=0;i<rows;i++) {
        rowheight[i]=rh;
    }
}

void MWPlayArea::paintField(int r, int c, int x, int y) {
    QPainter painter(this);
    int l=127;
    if(fields[r][c].pressed>0) l=180;
    switch(fields[r][c].type) {
    case NORMAL:
        painter.setPen(Qt::black);
        painter.setBrush(QColor::fromHsl(fields[r][c].f1.getHue(),180,l));
        painter.drawRect(x,y,colwidth[c],rowheight[r]);
        cap.sprintf("%d\n%5.2f",fields[r][c].f1.getMidinote(),fields[r][c].f1.getFreq());
        painter.drawText(x,y,colwidth[c],rowheight[r],Qt::AlignCenter,cap);
        break;

    case BEND_VERT:
        linearGrad.setStart(QPointF(x,y));
        linearGrad.setFinalStop(QPointF(x, y+rowheight[r]));
        linearGrad.setColorAt(0, QColor::fromHsl(fields[r][c].f1.getHue(),180,l));
        linearGrad.setColorAt(1, QColor::fromHsl(fields[r][c].f2.getHue(),180,l));
        painter.setPen(Qt::black);
        painter.setBrush(linearGrad);
        painter.drawRect(x,y,colwidth[c],rowheight[r]);
        cap.sprintf("%d",fields[r][c].f1.getMidinote());
        painter.drawText(x,y,colwidth[c],rowheight[r],Qt::AlignHCenter|Qt::AlignTop,cap);
        cap.sprintf("%d",fields[r][c].f2.getMidinote());
        painter.drawText(x,y,colwidth[c],rowheight[r],Qt::AlignHCenter|Qt::AlignBottom,cap);
        break;

    case BEND_HORIZ:
        linearGrad.setStart(QPointF(x,y));
        linearGrad.setFinalStop(QPointF(QPointF(x+colwidth[c], y)));
        linearGrad.setColorAt(0, QColor::fromHsl(fields[r][c].f1.getHue(),180,l));
        linearGrad.setColorAt(1, QColor::fromHsl(fields[r][c].f2.getHue(),180,l));
        painter.setPen(Qt::black);
        painter.setBrush(linearGrad);

        painter.drawRect(x,y,colwidth[c],rowheight[r]);
        cap.sprintf("%d",fields[r][c].f1.getMidinote());
        painter.drawText(x,y,colwidth[c],rowheight[r],Qt::AlignLeft,cap);
        cap.sprintf("%d",fields[r][c].f2.getMidinote());
        painter.drawText(x,y,colwidth[c],rowheight[r],Qt::AlignRight|Qt::AlignBottom,cap);

        break;
    case BEND_VERT_HORIZ:
        int y2=y+rowheight[r];
        linearGrad.setStart(QPointF(x,y));
        linearGrad.setFinalStop(QPointF(QPointF(x+colwidth[c], y)));
        float hue1=fields[r][c].f1.getHue();
        float hue2=fields[r][c].f3.getHue();
        float inchue1=(fields[r][c].f2.getHue()-hue1)/(float)rowheight[r];
        float inchue2=(fields[r][c].f4.getHue()-hue2)/(float)rowheight[r];

        if(r==0) {
            hue1=fields[r][c].f2.getHue();
            hue2=fields[r][c].f4.getHue();
        }
        painter.setPen(Qt::NoPen);
        for(int y1=y;y1<=y2;y1++) {
            linearGrad.setColorAt(0, QColor::fromHsl(hue1,180,l));
            linearGrad.setColorAt(1, QColor::fromHsl(hue2,180,l));
            painter.setBrush(linearGrad);
            painter.drawRect(x,y1,colwidth[c],1);
            if(r==0) {
                hue1-=inchue1;
                hue2-=inchue2;
            } else {
                hue1+=inchue1;
                hue2+=inchue2;
            }

        }
        painter.setPen(Qt::black);
        painter.setBrush(Qt::NoBrush);
        painter.drawRect(x,y,colwidth[c],rowheight[r]);

        if(r==0) {
            cap.sprintf("%d",fields[r][c].f1.getMidinote());
            painter.drawText(x,y,colwidth[c],rowheight[r],Qt::AlignLeft|Qt::AlignBottom,cap);
            cap.sprintf("%d",fields[r][c].f2.getMidinote());
            painter.drawText(x,y,colwidth[c],rowheight[r],Qt::AlignLeft|Qt::AlignTop,cap);
            cap.sprintf("%d",fields[r][c].f3.getMidinote());
            painter.drawText(x,y,colwidth[c],rowheight[r],Qt::AlignRight|Qt::AlignBottom,cap);
            cap.sprintf("%d",fields[r][c].f4.getMidinote());
            painter.drawText(x,y,colwidth[c],rowheight[r],Qt::AlignRight|Qt::AlignTop,cap);
        } else {
            cap.sprintf("%d",fields[r][c].f1.getMidinote());
            painter.drawText(x,y,colwidth[c],rowheight[r],Qt::AlignLeft|Qt::AlignTop,cap);
            cap.sprintf("%d",fields[r][c].f2.getMidinote());
            painter.drawText(x,y,colwidth[c],rowheight[r],Qt::AlignLeft|Qt::AlignBottom,cap);
            cap.sprintf("%d",fields[r][c].f3.getMidinote());
            painter.drawText(x,y,colwidth[c],rowheight[r],Qt::AlignRight|Qt::AlignTop,cap);
            cap.sprintf("%d",fields[r][c].f4.getMidinote());
            painter.drawText(x,y,colwidth[c],rowheight[r],Qt::AlignRight|Qt::AlignBottom,cap);
        }
        break;
    }
}

void MWPlayArea::paintField(int r, int c)
{
    int x=r*colwidth[0];
    int y=c*rowheight[0];
    paintField(r,c,x,y);
}

void MWPlayArea::paintEvent(QPaintEvent *E)
{
    int x,y=0;
    for(int r=0;r<rows;r++) {
        x=0;
        for(int c=0;c<cols;c++) {
            paintField(r,c,x,y);
            x+=colwidth[c];
        }
        y+=rowheight[r];
    }
}

void MWPlayArea::resizeEvent(QResizeEvent *E)
{
    calcGeo();
}

void MWPlayArea::processTouchEvent(misuTouchEvent e)
{
    //qDebug() << "MWPlayArea::processPoint " << e.id << " x " << e.x << " y " << e.y << " t " << e.t;
    int eventHash=e.id%64;
    eventStackElement * es = &eventStack[eventHash];

    int row=e.y*rows/height();
    int col=e.x*cols/width();
    MWPlayfield * pf = &fields[row][col];
    //qDebug() << "row " << row << " col " << col << " eventHash " << eventHash;

    float freq=pf->f1.getFreq();
    int midinote=pf->f1.getMidinote();
    int pitch=pf->f1.getPitch();
    int velocity=127;

    switch(e.state) {
    case Qt::TouchPointPressed:
        es->eventId=e.id;
        es->voiceId=nextVoiceId++;
        es->midinote=midinote;
        es->row=row;
        es->col=col;
        pf->pressed++;
        out->noteOn(chan,es->voiceId,freq,midinote,pitch,velocity);
        //paintField(row,col);
        update();
        break;
    case Qt::TouchPointMoved:
        if(row!=es->row || col!=es->col) {
            MWPlayfield * ppf = &fields[es->row][es->col];
            ppf->pressed--;
            out->noteOff(chan,es->voiceId,es->midinote);

            es->voiceId=nextVoiceId++;
            es->midinote=midinote;
            out->noteOn(chan,es->voiceId,freq,midinote,pitch,velocity);

            es->row=row;
            es->col=col;
            pf->pressed++;
            //paintField(row,col);
            update();
        }
        break;
    case Qt::TouchPointReleased:
        out->noteOff(chan,es->voiceId,es->midinote);
        es->eventId=-1;
        es->row=-1;
        es->col=-1;
        pf->pressed--;
        //paintField(row,col);
        update();
        break;
    }
}

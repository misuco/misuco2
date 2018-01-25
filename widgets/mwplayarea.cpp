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

#include "mwplayarea.h"
#include <QDebug>
#include <QLinearGradient>
#include <QRadialGradient>
#include "comm/senderdebug.h"
#include "comm/sendermobilesynth.h"

MWPlayArea::MWPlayArea(Pitch *p[], QObject *parent) : MisuWidget(parent),
    linearGrad(QPointF(0,1),QPointF(0,1)),
    font3(font1),
    font8(font1),
    pcalc(0,this),
    fcalc(&pcalc,this)
{

    MWPitch=p;

    Scale.basenote=0;
    Scale.baseoct=3;
    Scale.topoct=4;
    for(int i=0;i<BSCALE_SIZE;i++) {
        Scale.bscale[i]=false;
    }

    bendHoriz=false;
    bendVertTop=0;
    bendVertBot=0;

    for(int i=0;i<EVENT_STACK_SIZE;i++) {
        eventStack[i].eventId=-1;
        eventStack[i].row=-1;
        eventStack[i].col=-1;
        eventStack[i].f=0;
    }

    for(int r=0;r<MAX_ROWS;r++) {
        for(int c=0;c<MAX_COLS;c++) {
            fields[r][c].f1=new FreqTriple(MWPitch[c%(BSCALE_SIZE+1)],this);
            fields[r][c].f2=new FreqTriple(MWPitch[c%(BSCALE_SIZE+1)],this);
        }
    }

    out = new SenderDebug();
    config();
}

MWPlayArea::~MWPlayArea()
{
    for(int r=0;r<MAX_ROWS;r++) {
        for(int c=0;c<MAX_COLS;c++) {
            delete(fields[r][c].f1);
            delete(fields[r][c].f2);
        }
    }
}

void MWPlayArea::config()
{
    /*
     *  |.|.||.|.|.| |
     */

    for(int r=0;r<MAX_ROWS;r++) {
        for(int c=0;c<MAX_COLS;c++) {
            if(fields[r][c].hold) {
                out->noteOff(fields[r][c].voiceId);
            }
        }
    }

    cols=0;
    for(int oct=Scale.baseoct;oct<Scale.topoct;oct++) {
        setColumn(cols,Scale.basenote+oct*12,Scale.basenote);
        if(bendHoriz) {
            cols+=2;
        } else {
            cols++;
        }
        for(int note=0;note<BSCALE_SIZE;note++) {
            //qDebug() << "MWPlayArea::config " << note;
            if(Scale.bscale[note]) {
                setColumn(cols,Scale.basenote+oct*12+note+1,(Scale.basenote+note+1)%(BSCALE_SIZE+1));
                //qDebug() << "set column ";
                if(bendHoriz) {
                    cols+=2;
                } else {
                    cols++;
                }
            }
        }
    }
    int topnote=Scale.basenote+(Scale.topoct)*12;
    //qDebug() << "basenote: " << Scale.basenote << "topnote: " << topnote;
    setColumn(cols,topnote,Scale.basenote);
    cols++;

    for(int r=0;r<rows;r++) {
        for(int c=0;c<cols;c++) {
            if(fields[r][c].hold) {
                fields[r][c].voiceId=out->noteOn(channel,fields[r][c].f1->getFreq(),fields[r][c].f1->getMidinote(),fields[r][c].f1->getPitch(),127);
            }
        }
    }

    calcGeo();
    //update();
}

void MWPlayArea::setColumn(int col, int midinote, int basenote) {
    //qDebug() << "setColumn " << col << " midinote " << midinote << " basenote " << basenote;
    rows=0;
    if(bendVertTop!=0) {
        fields[rows][col].type=BEND_VERT;
        fields[rows][col].f1->setMidinote(midinote,MWPitch[basenote]);
        fields[rows][col].hue1bent=fields[rows][col].f1->getHue()+HUE_NOTES*bendVertTop;
        if(fields[rows][col].hue1bent>359) fields[rows][col].hue1bent-=359;
        if(fields[rows][col].hue1bent<0) fields[rows][col].hue1bent+=359;
        fields[rows][col].pressed=0;
        if(col>1 && bendHoriz) {
            fields[rows][col-1].type=BEND_VERT_HORIZ;
            fields[rows][col-1].f1->setMidinote(fields[rows][col-2].f1->getMidinote(),MWPitch[fields[rows][col-2].f1->getBasenote()]);
            fields[rows][col-1].hue1bent=fields[rows][col-1].f1->getHue()+HUE_NOTES*bendVertTop;
            if(fields[rows][col-1].hue1bent>359) fields[rows][col-1].hue1bent-=359;
            if(fields[rows][col-1].hue1bent<0) fields[rows][col-1].hue1bent+=359;
            fields[rows][col-1].f2->setMidinote(midinote,MWPitch[basenote]);
            fields[rows][col-1].hue2bent=fields[rows][col-1].f2->getHue()+HUE_NOTES*bendVertTop;
            if(fields[rows][col-1].hue2bent>359) fields[rows][col-1].hue2bent-=359;
            if(fields[rows][col-1].hue2bent<0) fields[rows][col-1].hue2bent+=359;
            fields[rows][col-1].pressed=0;
        }
        rows++;
    }
    fields[rows][col].type=NORMAL;
    fields[rows][col].f1->setMidinote(midinote,MWPitch[basenote]);
    //qDebug() << "set f1 " << midinote << " " << fields[rows][col].f1;
    fields[rows][col].pressed=0;
    if(col>1 && bendHoriz) {
        fields[rows][col-1].type=BEND_HORIZ;
        fields[rows][col-1].f1->setMidinote(fields[rows][col-2].f1->getMidinote(),MWPitch[fields[rows][col-2].f1->getBasenote()]);
        fields[rows][col-1].f2->setMidinote(midinote,MWPitch[basenote]);
        fields[rows][col-1].pressed=0;
    }
    rows++;
    if(bendVertBot!=0) {
        fields[rows][col].type=BEND_VERT;
        fields[rows][col].f1->setMidinote(midinote,MWPitch[basenote]);
        fields[rows][col].hue1bent=fields[rows][col].f1->getHue()+HUE_NOTES*bendVertBot;
        if(fields[rows][col].hue1bent>359) fields[rows][col].hue1bent-=359;
        if(fields[rows][col].hue1bent<0) fields[rows][col].hue1bent+=359;
        fields[rows][col].pressed=0;
        if(col>1 && bendHoriz) {
            fields[rows][col-1].type=BEND_VERT_HORIZ;
            fields[rows][col-1].f1->setMidinote(fields[rows][col-2].f1->getMidinote(),MWPitch[fields[rows][col-2].f1->getBasenote()]);
            fields[rows][col-1].hue1bent=fields[rows][col-1].f1->getHue()+HUE_NOTES*bendVertBot;
            if(fields[rows][col-1].hue1bent>359) fields[rows][col-1].hue1bent-=359;
            if(fields[rows][col-1].hue1bent<0) fields[rows][col-1].hue1bent+=359;
            fields[rows][col-1].f2->setMidinote(midinote,MWPitch[basenote]);
            fields[rows][col-1].hue2bent=fields[rows][col-1].f2->getHue()+HUE_NOTES*bendVertBot;
            if(fields[rows][col-1].hue2bent>359) fields[rows][col-1].hue2bent-=359;
            if(fields[rows][col-1].hue2bent<0) fields[rows][col-1].hue2bent+=359;
            fields[rows][col-1].pressed=0;
        }
        rows++;
    }
}

void MWPlayArea::calcGeo()
{
    int cw=50;//width()/cols;
    int rh=50;//height()/rows;
    for(int i=0;i<cols;i++) {
        colwidth[i]=cw;
    }
    for(int i=0;i<rows;i++) {
        rowheight[i]=rh;
    }

    /* TODO
    int menubottonsize = width()/20;
    if(height()>width()) {
        menubottonsize = height()/20;
    }
    */
    int menubottonsize = 50;

    menux1 = menubottonsize;
    menux2 = 2*menubottonsize;
    menuy1 = 0;
    menuy2 = menubottonsize;

    presetsx1 = 0;
    presetsx2 = menubottonsize;
    presetsy1 = 0;
    presetsy2 = menubottonsize;

    font3.setPixelSize(cw/2);
    font8.setPixelSize(cw/4);
}

/*
void MWPlayArea::paintField(int r, int c, int x, int y) {
    QPainter painter(this);

    painter.setFont(font3);

    //qDebug() << "MWPlayArea::paintField r " << r << " c " << c;
    int l=lOff;
    int s=sOff;
    if(fields[r][c].pressed>0 || fields[r][c].hold) {
        l=lOn;
        s=sOn;
    }

    QColor colorF1 = QColor::fromHsl(fields[r][c].f1->getHue(),s,l);
    QColor colorF1b = QColor::fromHsl(fields[r][c].hue1bent,s,l);
    QColor colorF2 = QColor::fromHsl(fields[r][c].f2->getHue(),s,l);
    QColor colorF2b = QColor::fromHsl(fields[r][c].hue2bent,s,l);
    if(bwmode) {
        if(fields[r][c].f1->getBW()) {
            if(fields[r][c].pressed>0 || fields[r][c].hold) {
                colorF1 = hlwkeycolor;
                colorF1b = hlbkeycolor;
            } else {
                colorF1 = wkeycolor;
                colorF1b = bkeycolor;
            }
        } else {
            if(fields[r][c].pressed>0 || fields[r][c].hold) {
                colorF1 = hlbkeycolor;
                colorF1b = hlwkeycolor;
            } else {
                colorF1 = bkeycolor;
                colorF1b = wkeycolor;
            }
        }
        if(fields[r][c].f2->getBW()) {
            if(fields[r][c].pressed>0 || fields[r][c].hold) {
                colorF2 = hlwkeycolor;
                colorF2b = hlbkeycolor;
            } else {
                colorF2 = wkeycolor;
                colorF2b = bkeycolor;
            }
        } else {
            if(fields[r][c].pressed>0 || fields[r][c].hold) {
                colorF2 = hlbkeycolor;
                colorF2b = hlwkeycolor;
            } else {
                colorF2 = bkeycolor;
                colorF2b = wkeycolor;
            }
        }
    }

    QString basenote;
    switch(fields[r][c].type) {
    case NORMAL:
        if(fields[r][c].pressed>0 || fields[r][c].hold) {
            painter.setPen(highlightcolor);
        } else {
            painter.setPen(fgcolor);
        }
        painter.setBrush(colorF1);
        //qDebug() << "setBrush hue " << fields[r][c].f1->getHue();
        painter.drawRect(x,y,colwidth[c],rowheight[r]);
        basenote = fields[r][c].f1->getBasenoteString(noteSymbols);
        if(basenote.startsWith("_")) {
            font3.setUnderline(true);
            painter.setFont(font3);
            basenote.remove(0,1);
        }
        cap.sprintf("%s",basenote.toStdString().c_str());
        painter.drawText(x,y,colwidth[c],rowheight[r],Qt::AlignCenter,cap);        
        font3.setUnderline(false);

        if(showFreqs) {
            painter.setFont(font8);
            // no text on padw with menu buttons (first and last col in first row)
            if( (c!=0 && c!=cols-1) || r!=0) {
                cap.sprintf("%d %d %d", (fields[r][c].f1->getBasenote()-Scale.basenote+12)%12, fields[r][c].f1->getMidinote(), fields[r][c].f1->getOct());
                painter.drawText(x,y,colwidth[c],rowheight[r],Qt::AlignTop,cap);
            }
            cap.sprintf("%5.2f",fields[r][c].f1->getFreq());
            painter.drawText(x,y+rowheight[r]/3,colwidth[c],rowheight[r],Qt::AlignCenter,cap);
        }

        break;

    case BEND_VERT:
        linearGrad.setStart(QPointF(x,y));
        linearGrad.setFinalStop(QPointF(x, y+rowheight[r]));
        if(0==r) {
            linearGrad.setColorAt(0, colorF1b);
            linearGrad.setColorAt(1, colorF1);
        } else {
            linearGrad.setColorAt(0, colorF1);
            linearGrad.setColorAt(1, colorF1b);
        }
        painter.setPen(fgcolor);
        painter.setBrush(linearGrad);
        painter.drawRect(x,y,colwidth[c],rowheight[r]);

        if(showFreqs) {
            painter.setFont(font8);
            if(0==r) {
                // no text on padw with menu buttons (first and last col in first row)
                if( (c!=0 && c!=cols-1) || r!=0) {
                    cap.sprintf("%d",fields[r][c].f1->getMidinote()+bendVertTop);
                    painter.drawText(x,y,colwidth[c],rowheight[r],Qt::AlignHCenter|Qt::AlignTop,cap);
                }
                cap.sprintf("%d",fields[r][c].f1->getMidinote());
                painter.drawText(x,y,colwidth[c],rowheight[r],Qt::AlignHCenter|Qt::AlignBottom,cap);
            } else {
                // no text on padw with menu buttons (first and last col in first row)
                if( (c!=0 && c!=cols-1) || r!=0) {
                    cap.sprintf("%d",fields[r][c].f1->getMidinote()+bendVertBot);
                    painter.drawText(x,y,colwidth[c],rowheight[r],Qt::AlignHCenter|Qt::AlignBottom,cap);
                }
                cap.sprintf("%d",fields[r][c].f1->getMidinote());
                painter.drawText(x,y,colwidth[c],rowheight[r],Qt::AlignHCenter|Qt::AlignTop,cap);
            }
        }
        break;

    case BEND_HORIZ:
        linearGrad.setStart(QPointF(x,y));
        linearGrad.setFinalStop(QPointF(QPointF(x+colwidth[c], y)));
        linearGrad.setColorAt(0, colorF1);
        linearGrad.setColorAt(1, colorF2);
        painter.setPen(fgcolor);
        painter.setBrush(linearGrad);
        painter.drawRect(x,y,colwidth[c],rowheight[r]);

        if(showFreqs) {
            painter.setFont(font8);
            cap.sprintf("%d",fields[r][c].f1->getMidinote());
            painter.drawText(x,y,colwidth[c],rowheight[r],Qt::AlignLeft|Qt::AlignBottom,cap);
            cap.sprintf("%d",fields[r][c].f2->getMidinote());
            painter.drawText(x,y,colwidth[c],rowheight[r],Qt::AlignRight|Qt::AlignBottom,cap);
        }

        break;

    case BEND_VERT_HORIZ:
        if(bwmode) {
            int hue1;
            int sat1;
            int lum1;
            float lum1f;
            int alp1;
            colorF1.getHsl(&hue1,&sat1,&lum1,&alp1);

            int hue2;
            int sat2;
            int lum2;
            float lum2f;
            int alp2;
            colorF2.getHsl(&hue2,&sat2,&lum2,&alp2);

            linearGrad.setStart(QPointF(x,y));
            linearGrad.setFinalStop(QPointF(QPointF(x+colwidth[c], y)));

            float inclum1;
            if(lum1>127) inclum1 = -lum1;
            else inclum1 = 255-lum1;
            inclum1=inclum1/(float)rowheight[r];

            float inclum2;
            if(lum2>127) inclum2 = -lum2;
            else inclum2 = 255-lum2;
            inclum2=inclum2/(float)rowheight[r];

            lum1f=lum1;
            lum2f=lum2;

            painter.setPen(Qt::NoPen);
            if(0==r) {
                for(int y1=y+rowheight[r];y1>=y;y1--) {
                    linearGrad.setColorAt(0, QColor::fromHsl(hue1,sat1,lum1f,alp1));
                    linearGrad.setColorAt(1, QColor::fromHsl(hue2,sat2,lum2f,alp2));
                    painter.setBrush(linearGrad);
                    painter.drawRect(x,y1,colwidth[c],1);
                    lum1f+=inclum1;
                    lum2f+=inclum2;
                }
            } else {
                for(int y1=y;y1<=y+rowheight[r];y1++) {
                    linearGrad.setColorAt(0, QColor::fromHsl(hue1,sat1,lum1f,alp1));
                    linearGrad.setColorAt(1, QColor::fromHsl(hue2,sat2,lum2f,alp2));
                    painter.setBrush(linearGrad);
                    painter.drawRect(x,y1,colwidth[c],1);
                    lum1f+=inclum1;
                    lum2f+=inclum2;
                }
            }

        } else {
            linearGrad.setStart(QPointF(x,y));
            linearGrad.setFinalStop(QPointF(QPointF(x+colwidth[c], y)));
            float hue1=fields[r][c].f1->getHue();
            float hue2=fields[r][c].f2->getHue();
            float inchue;

            painter.setPen(Qt::NoPen);
            if(0==r) {
                inchue=(float)(bendVertTop*HUE_NOTES)/(float)rowheight[r];
                //qDebug() << "bendVertTop inchue " << inchue;
                for(int y1=y+rowheight[r];y1>=y;y1--) {
                    //qDebug() << "hue 1 " << hue1 << " hue2 " << hue2;
                    linearGrad.setColorAt(0, QColor::fromHsl(hue1,s,l));
                    linearGrad.setColorAt(1, QColor::fromHsl(hue2,s,l));
                    painter.setBrush(linearGrad);
                    painter.drawRect(x,y1,colwidth[c],1);
                    hue1+=inchue;
                    if(hue1>359) hue1-=359;
                    if(hue1<0) hue1+=359;
                    hue2+=inchue;
                    if(hue2>359) hue2-=359;
                    if(hue2<0) hue2+=359;
                }
            } else {
                inchue=(float)(bendVertBot*HUE_NOTES)/(float)rowheight[r];
                //qDebug() << "bendVertBot inchue " << inchue;
                for(int y1=y;y1<=y+rowheight[r];y1++) {
                    //qDebug() << "hue 1 " << hue1 << " hue2 " << hue2;
                    linearGrad.setColorAt(0, QColor::fromHsl(hue1,s,l));
                    linearGrad.setColorAt(1, QColor::fromHsl(hue2,s,l));
                    painter.setBrush(linearGrad);
                    painter.drawRect(x,y1,colwidth[c],1);
                    hue1+=inchue;
                    if(hue1>359) hue1-=359;
                    if(hue1<0) hue1+=359;
                    hue2+=inchue;
                    if(hue2>359) hue2-=359;
                    if(hue2<0) hue2+=359;
                }
            }

        }


        painter.setPen(fgcolor );
        painter.setBrush(Qt::NoBrush);
        painter.drawRect(x,y,colwidth[c],rowheight[r]);
        break;
    }
}

void MWPlayArea::paintField(int r, int c)
{
    int x=r*colwidth[0];
    int y=c*rowheight[0];
    paintField(r,c,x,y);
}

void MWPlayArea::paintEvent(QPaintEvent *)
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

    QPainter painter(this);
    painter.setPen(fgcolor);
    painter.setBrush(fgcolor);

    QFont font(font1);
    font.setPixelSize(font1size/2);
    painter.setFont(font);

    for(int i=0;i<EVENT_STACK_SIZE;i++) {
        eventStackElement * es = &eventStack[i];
        if(es && es->eventId>0) {
            int r = colwidth[0]/4;
            //painter.drawRect(es->x-50,es->y-50,100,100);
            painter.drawEllipse(es->x-r,es->y-r,2*r,2*r);
            painter.drawText(es->x-r,es->y-2*r,4*r,5*r,0,QString("%1\n%2\n\n").arg(es->f).arg(es->voiceId));
        }
    }

    painter.setPen(fgcolor);
    // fill round space with fgcolor
    painter.drawRect(x,0,width()-x,height());

    int barHeight = (menuy2-menuy1)/7;
    painter.drawRect(menux1,menuy1,menux2-menux1,barHeight);
    painter.drawRect(menux1,menuy1+2*barHeight,menux2-menux1,barHeight);
    painter.drawRect(menux1,menuy1+4*barHeight,menux2-menux1,barHeight);
    painter.drawRect(menux1,menuy1+6*barHeight,menux2-menux1,barHeight);

    painter.drawRect(presetsx1,presetsy1,presetsx2-presetsx1,barHeight);
    painter.drawRect(presetsx1,presetsy1+2*barHeight,presetsx2-presetsx1,barHeight);
    painter.drawRect(presetsx1,presetsy1+4*barHeight,presetsx2-presetsx1,barHeight);
    painter.drawRect(presetsx1,presetsy1+6*barHeight,presetsx2-presetsx1,barHeight);
}

void MWPlayArea::resizeEvent(QResizeEvent *)
{
    calcGeo();
}
    */

void MWPlayArea::processTouchEvent(misuTouchEvent e)
{
    //qDebug() << "MWPlayArea::processPoint " << e.id << " x " << e.x << " y " << e.y << " t " << e.t;

    if (e.state==Qt::TouchPointReleased &&  e.x>=menux1 && e.x<=menux2 && e.y>=menuy1 && e.y<=menuy2) {
        emit menuTouch();
    }

    if (e.state==Qt::TouchPointReleased &&  e.x>=presetsx1 && e.x<=presetsx2 && e.y>=presetsy1 && e.y<=presetsy2) {
        emit presetsTouch();
    }

    int eventHash=e.id%64;
    eventStackElement * es = &eventStack[eventHash];
    es->x=e.x;
    es->y=e.y;
    int row=0;

    /* TODO
    if(height()>0) row = e.y*rows/height();
    int col=e.x*cols/width();
    */
    int col=0;

    float yrel=(float)(e.y-row*rowheight[row])/(float)rowheight[row];
    float xrel=(float)(e.x-col*colwidth[col])/(float)colwidth[col];

    MWPlayfield * pf = &fields[row][col];
    //qDebug() << "row " << row << " col " << col << " eventHash " << eventHash;

    float freq;
    int midinote;
    int pitch;
    int velocity=127;

    float freqdiff;
    int pitchdiff;

    switch(pf->type) {
    case BEND_HORIZ:
        freqdiff=pf->f2->getFreq()-pf->f1->getFreq();
        freqdiff*=xrel;
        freq=pf->f1->getFreq()+freqdiff;
        pitchdiff=pf->f2->getMidinote()*8192-pf->f1->getMidinote()*8192;
        pitchdiff+=pf->f2->getPitch();
        pitchdiff-=pf->f1->getMidinote();
        pitchdiff*=xrel;
        pitchdiff+=pf->f1->getMidinote()*8192;
        midinote=round(pitchdiff/8192);
        pitch=pitchdiff-midinote*8192;
        break;

    case BEND_VERT_HORIZ:
        pitchdiff=pf->f2->getMidinote()*100-pf->f1->getMidinote()*8192;
        pitchdiff+=pf->f2->getPitch();
        pitchdiff-=pf->f1->getMidinote();
        pitchdiff*=xrel;
        pitchdiff+=pf->f1->getMidinote()*100;
        if(0==row) {
            pitchdiff+=bendVertTop*100*(1-yrel);
        } else {
            pitchdiff+=bendVertBot*100*yrel;
        }
        midinote=round(pitchdiff/100);
        pcalc.setPitch(pitchdiff-midinote*100);
        fcalc.setMidinote(midinote,&pcalc);
        freq=fcalc.getFreq();
        break;

    case BEND_VERT:
        if(0==row) {
            pitchdiff=bendVertTop*100;
            yrel=1-yrel;
        } else {
            pitchdiff=bendVertBot*100;
        }
        pitchdiff*=yrel;
        pitchdiff+=pf->f1->getMidinote()*100;
        midinote=round(pitchdiff/100);
        pitch = pitchdiff-midinote*100;
        pcalc.setPitch(pitch);
        fcalc.setMidinote(midinote,&pcalc);
        freq=fcalc.getFreq();
        //qDebug() << "midinote " << midinote << " pitch " << pitch << " freq " << freq  << " bendVert " << bendVertTop << " " << bendVertBot << " pitchdiff " << pitchdiff;
        break;

    default:
        freq=pf->f1->getFreq();
        midinote=pf->f1->getMidinote();
        pitch=pf->f1->getPitch();
        break;
    }

    switch(e.state) {
    case Qt::TouchPointPressed:
        es->eventId=e.id;
        es->midinote=midinote;
        es->row=row;
        es->col=col;
        es->f=freq;
        if(holdMode && pf->pressed==0) {
            if(pf->hold) {
                out->noteOff(pf->voiceId);
                pf->hold=false;
            } else {
                pf->voiceId=out->noteOn(channel,freq,midinote,pitch,velocity);
                pf->hold=true;
            }
        } else {
            es->voiceId=out->noteOn(channel,freq,midinote,pitch,velocity);
            pf->pressed++;
        }
        break;

    case Qt::TouchPointMoved:
        if(!holdMode) {
            if(row!=es->row || col!=es->col) {
                MWPlayfield * ppf = &fields[es->row][es->col];
                ppf->pressed--;
                out->noteOff(es->voiceId);

                es->midinote=midinote;
                es->voiceId=out->noteOn(channel,freq,midinote,pitch,velocity);

                es->row=row;
                es->col=col;
                es->f=freq;
                pf->pressed++;
                //update();
            } else if(freq!=es->f) {
                out->pitch(channel,es->voiceId,freq,midinote,pitch);
                es->f=freq;
            }
        }
        if(sendCC1) {
            out->cc(channel,es->voiceId,1,1.0f-yrel,1.0f-yrel);
        }
        break;

    case Qt::TouchPointReleased:
        if(!holdMode) {
            out->noteOff(es->voiceId);
            pf->pressed--;
        }
        es->eventId=-1;
        es->row=-1;
        es->col=-1;
        break;
    }
    //update();

}

void MWPlayArea::setBaseNote(Pitch *p)
{
    Scale.basenote=p->getBasenote();
    config();
}


void MWPlayArea::setOctConf(int bottom, int top)
{
    Scale.baseoct=bottom;
    Scale.topoct=top;
    config();

}

void MWPlayArea::setBscale(int n, bool v)
{
    //qDebug() << "MWPlayArea::setBscale " << n << " " << v;
    Scale.bscale[n-1]=v;
    config();
}

void MWPlayArea::setScale(MWScale * s)
{
    Scale.basenote=s->basenote;
    Scale.baseoct=s->baseoct;
    for(int i=0;i<BSCALE_SIZE;i++) {
        Scale.bscale[i]=s->bscale[i];
    }
    Scale.size=s->size;
    Scale.topoct=s->topoct;
    config();
}

void MWPlayArea::setBendHori(bool b)
{
    bendHoriz=b;
    config();
}

void MWPlayArea::setBendVertTop(int b)
{
    bendVertTop=b;
    config();
}

void MWPlayArea::setBendVertBot(int b)
{
    bendVertBot=b;
    config();
}

void MWPlayArea::pitchChange()
{
    config();
}

void MWPlayArea::setOut(ISender *value)
{
    out = value;
    //qDebug() << "MWPlayArea::setOut:" << out;
}

void MWPlayArea::onscaleupdate()
{
    //update();
}

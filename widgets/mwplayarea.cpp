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
#include <QTouchEvent>
#include <QDateTime>
#include "comm/senderdebug.h"
#include "comm/sendermobilesynth.h"

MWPlayArea::MWPlayArea(QObject *parent) : MisuWidget(parent),
    pcalc(0,this),
    fcalc(&pcalc,this)
{
    Scale.rootNote=0;
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
        setColumn(cols,Scale.rootNote+oct*12,Scale.rootNote);
        if(bendHoriz) {
            cols+=2;
        } else {
            cols++;
        }
        for(int note=0;note<BSCALE_SIZE;note++) {
            //qDebug() << "MWPlayArea::config " << note;
            if(Scale.bscale[note]) {
                setColumn(cols,Scale.rootNote+oct*12+note+1,(Scale.rootNote+note+1)%(BSCALE_SIZE+1));
                //qDebug() << "set column ";
                if(bendHoriz) {
                    cols+=2;
                } else {
                    cols++;
                }
            }
        }
    }
    int topnote=Scale.rootNote+(Scale.topoct)*12;
    //qDebug() << "rootNote: " << Scale.rootNote << "topnote: " << topnote;
    setColumn(cols,topnote,Scale.rootNote);
    cols++;

    for(int r=0;r<rows;r++) {
        for(int c=0;c<cols;c++) {
            if(fields[r][c].hold) {
                fields[r][c].voiceId=out->noteOn(channel,fields[r][c].f1->getFreq(),fields[r][c].f1->getMidinote(),fields[r][c].f1->getPitch(),127);
            }
        }
    }

    calcGeo();

    emit playRowsChanged();
}

void MWPlayArea::setColumn(int col, int midinote, int rootNote) {
    //qDebug() << "setColumn " << col << " midinote " << midinote << " rootNote " << rootNote;
    rows=0;

    float huePerNote = 1.0/12.0;
    if(bendVertTop!=0) {
        fields[rows][col].type=BEND_VERT;
        fields[rows][col].f1->setMidinote(midinote,MWPitch[rootNote]);
        fields[rows][col].hue1bent=fields[rows][col].f1->getHue()+huePerNote*(float)bendVertTop;
        if(fields[rows][col].hue1bent>1) fields[rows][col].hue1bent-=1;
        if(fields[rows][col].hue1bent<0) fields[rows][col].hue1bent+=1;
        fields[rows][col].pressed=0;
        if(col>1 && bendHoriz) {
            fields[rows][col-1].type=BEND_VERT_HORIZ;
            fields[rows][col-1].f1->setMidinote(fields[rows][col-2].f1->getMidinote(),MWPitch[fields[rows][col-2].f1->getRootNote()]);
            fields[rows][col-1].hue1bent=fields[rows][col-1].f1->getHue()+huePerNote*(float)bendVertTop;
            if(fields[rows][col-1].hue1bent>1) fields[rows][col-1].hue1bent-=1;
            if(fields[rows][col-1].hue1bent<0) fields[rows][col-1].hue1bent+=1;
            fields[rows][col-1].f2->setMidinote(midinote,MWPitch[rootNote]);
            fields[rows][col-1].hue2bent=fields[rows][col-1].f2->getHue()+huePerNote*(float)bendVertTop;
            if(fields[rows][col-1].hue2bent>1) fields[rows][col-1].hue2bent-=1;
            if(fields[rows][col-1].hue2bent<0) fields[rows][col-1].hue2bent+=1;
            fields[rows][col-1].pressed=0;
        }
        rows++;
    }
    fields[rows][col].type=NORMAL;
    fields[rows][col].f1->setMidinote(midinote,MWPitch[rootNote]);
    //qDebug() << "set f1 " << midinote << " " << fields[rows][col].f1;
    fields[rows][col].pressed=0;
    if(col>1 && bendHoriz) {
        fields[rows][col-1].type=BEND_HORIZ;
        fields[rows][col-1].f1->setMidinote(fields[rows][col-2].f1->getMidinote(),MWPitch[fields[rows][col-2].f1->getRootNote()]);
        fields[rows][col-1].f2->setMidinote(midinote,MWPitch[rootNote]);
        fields[rows][col-1].pressed=0;
    }
    rows++;
    if(bendVertBot!=0) {
        fields[rows][col].type=BEND_VERT;
        fields[rows][col].f1->setMidinote(midinote,MWPitch[rootNote]);
        fields[rows][col].hue1bent=fields[rows][col].f1->getHue()+huePerNote*(float)bendVertBot;
        if(fields[rows][col].hue1bent>1) fields[rows][col].hue1bent-=1;
        if(fields[rows][col].hue1bent<0) fields[rows][col].hue1bent+=1;
        fields[rows][col].pressed=0;
        if(col>1 && bendHoriz) {
            fields[rows][col-1].type=BEND_VERT_HORIZ;
            fields[rows][col-1].f1->setMidinote(fields[rows][col-2].f1->getMidinote(),MWPitch[fields[rows][col-2].f1->getRootNote()]);
            fields[rows][col-1].hue1bent=fields[rows][col-1].f1->getHue()+huePerNote*(float)bendVertBot;
            if(fields[rows][col-1].hue1bent>1) fields[rows][col-1].hue1bent-=1;
            if(fields[rows][col-1].hue1bent<0) fields[rows][col-1].hue1bent+=1;
            fields[rows][col-1].f2->setMidinote(midinote,MWPitch[rootNote]);
            fields[rows][col-1].hue2bent=fields[rows][col-1].f2->getHue()+huePerNote*(float)bendVertBot;
            if(fields[rows][col-1].hue2bent>1) fields[rows][col-1].hue2bent-=1;
            if(fields[rows][col-1].hue2bent<0) fields[rows][col-1].hue2bent+=1;
            fields[rows][col-1].pressed=0;
        }
        rows++;
    }
}

void MWPlayArea::calcGeo()
{
    int cw=width/cols;
    int rh=height/rows;
    for(int i=0;i<cols;i++) {
        colwidth[i]=cw;
    }
    for(int i=0;i<rows;i++) {
        rowheight[i]=rh;
    }
}

/*
void MWPlayArea::paintField(int r, int c, int x, int y) {
    QPainter painter(this);

    painter.setFont(font3);





    //qDebug() << "MWPlayArea::paintField r " << r << " c " << c;
    QString rootNote;
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
        rootNote = fields[r][c].f1->getRootNoteString(noteSymbols);
        if(rootNote.startsWith("_")) {
            font3.setUnderline(true);
            painter.setFont(font3);
            rootNote.remove(0,1);
        }
        cap.sprintf("%s",rootNote.toStdString().c_str());
        painter.drawText(x,y,colwidth[c],rowheight[r],Qt::AlignCenter,cap);        
        font3.setUnderline(false);

        if(showFreqs) {
            painter.setFont(font8);
            // no text on padw with menu buttons (first and last col in first row)
            if( (c!=0 && c!=cols-1) || r!=0) {
                cap.sprintf("%d %d %d", (fields[r][c].f1->getRootNote()-Scale.rootNote+12)%12, fields[r][c].f1->getMidinote(), fields[r][c].f1->getOct());
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

    /*
    if (e.state==Qt::TouchPointReleased &&  e.x>=menux1 && e.x<=menux2 && e.y>=menuy1 && e.y<=menuy2) {
        emit menuTouch();
    }

    if (e.state==Qt::TouchPointReleased &&  e.x>=presetsx1 && e.x<=presetsx2 && e.y>=presetsy1 && e.y<=presetsy2) {
        emit presetsTouch();
    }
    */

    int eventHash=e.id%64;
    eventStackElement * es = &eventStack[eventHash];
    es->x=e.x;
    es->y=e.y;
    int row=0;

    if(height>0) row = e.y*rows/height;
    int col=e.x*cols/width;

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
                ppf->calcColor();
                out->noteOff(es->voiceId);

                es->midinote=midinote;
                es->voiceId=out->noteOn(channel,freq,midinote,pitch,velocity);

                es->row=row;
                es->col=col;
                es->f=freq;
                pf->pressed++;
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

    pf->calcColor();

}

void MWPlayArea::setRootNote(Pitch *p)
{
    Scale.rootNote=p->getRootNote();
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
    Scale.rootNote=s->rootNote;
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

/*
void MWPlayArea::pitchChange()
{
    config();
}
*/

void MWPlayArea::setOut(ISender *value)
{
    delete(out);
    out = value;
    //qDebug() << "MWPlayArea::setOut:" << out;
}

void MWPlayArea::resize(int w, int h)
{
    //qDebug() << "MWPlayArea::resize w: " << w << " h: " << h;
    width=w;
    height=h;
    calcGeo();
}

void MWPlayArea::onPressed(int id, int x, int y)
{
    //qDebug() << "MWPlayArea::onPressed id: " << id << " x: " << x << " y: " << y;
    misuTouchEvent e;
    e.id=id;
    e.x=x;
    e.y=y;
    e.state=Qt::TouchPointPressed;
    e.t=QDateTime::currentMSecsSinceEpoch();
    processTouchEvent(e);
}

void MWPlayArea::onUpdated(int id, int x, int y)
{
    //qDebug() << "MWPlayArea::onUpdated id: " << id << " x: " << x << " y: " << y;
    misuTouchEvent e;
    e.id=id;
    e.x=x;
    e.y=y;
    e.state=Qt::TouchPointMoved;
    e.t=QDateTime::currentMSecsSinceEpoch();
    processTouchEvent(e);
}

void MWPlayArea::onReleased(int id, int x, int y)
{
    //qDebug() << "MWPlayArea::onReleased id: " << id << " x: " << x << " y: " << y;
    misuTouchEvent e;
    e.id=id;
    e.x=x;
    e.y=y;
    e.state=Qt::TouchPointReleased;
    e.t=QDateTime::currentMSecsSinceEpoch();
    processTouchEvent(e);
}

QList<QObject *> MWPlayArea::row0()
{
    QList<QObject*> colsList;
    for(int j=0;j<cols;j++) {
        fields[0][j].calcColor();
        colsList.append(&fields[0][j]);
    }
    return colsList;
}

QList<QObject *> MWPlayArea::row1()
{
    QList<QObject*> colsList;
    for(int j=0;j<cols;j++) {
        fields[1][j].calcColor();
        colsList.append(&fields[1][j]);
    }
    return colsList;
}

QList<QObject *> MWPlayArea::row2()
{
    QList<QObject*> colsList;
    for(int j=0;j<cols;j++) {
        fields[2][j].calcColor();
        colsList.append(&fields[2][j]);
    }
    return colsList;
}


void MWPlayArea::onscaleupdate()
{
    //update();
}

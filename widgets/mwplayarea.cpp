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

MWPlayArea::MWPlayArea(QObject *parent) : QObject(parent),
    pcalc(0,this),
    fcalc(&pcalc,this)
{
    /*
    Scale.rootNote=0;
    Scale.baseoct=3;
    Scale.topoct=4;
    for(int i=0;i<BSCALE_SIZE;i++) {
        Scale.bscale[i]=false;
    }
    */

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
            fields[r][c].f1=new FreqTriple(MisuWidget::MWPitch[c%(BSCALE_SIZE+1)],this);
            fields[r][c].f2=new FreqTriple(MisuWidget::MWPitch[c%(BSCALE_SIZE+1)],this);
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

    /*
    for(int r=0;r<MAX_ROWS;r++) {
        for(int c=0;c<MAX_COLS;c++) {
            if(fields[r][c].hold) {
                out->noteOff(fields[r][c].voiceId);
            }
        }
    }
    */

    cols=0;
    for(int oct=MisuWidget::Scale.baseoct;oct<MisuWidget::Scale.topoct;oct++) {
        setColumn(cols,MisuWidget::Scale.rootNote+oct*12,MisuWidget::Scale.rootNote);
        if(bendHoriz) {
            cols+=2;
        } else {
            cols++;
        }
        for(int note=0;note<BSCALE_SIZE;note++) {
            //qDebug() << "MWPlayArea::config " << note;
            if(MisuWidget::Scale.bscale[note]) {
                setColumn(cols,MisuWidget::Scale.rootNote+oct*12+note+1,(MisuWidget::Scale.rootNote+note+1)%(BSCALE_SIZE+1));
                //qDebug() << "set column ";
                if(bendHoriz) {
                    cols+=2;
                } else {
                    cols++;
                }
            }
        }
    }
    int topnote=MisuWidget::Scale.rootNote+(MisuWidget::Scale.topoct)*12;
    //qDebug() << "rootNote: " << Scale.rootNote << "topnote: " << topnote;
    setColumn(cols,topnote,MisuWidget::Scale.rootNote);
    cols++;

    /*
    for(int r=0;r<rows;r++) {
        for(int c=0;c<cols;c++) {
            if(fields[r][c].hold) {
                fields[r][c].voiceId=out->noteOn(channel,fields[r][c].f1->getFreq(),fields[r][c].f1->getMidinote(),fields[r][c].f1->getPitch(),127);
            }
        }
    }
    */

    calcGeo();

    emit playRowsChanged();
}

void MWPlayArea::setColumn(int col, int midinote, int rootNote) {
    //qDebug() << "setColumn " << col << " midinote " << midinote << " rootNote " << rootNote;
    rows=0;

    float huePerNote = 1.0/12.0;
    if(bendVertTop!=0) {
        fields[rows][col].type=BEND_VERT;
        fields[rows][col].f1->setMidinote(midinote,MisuWidget::MWPitch[rootNote]);
        fields[rows][col].hue1bent=fields[rows][col].f1->getHue()+huePerNote*(float)bendVertTop;
        if(fields[rows][col].hue1bent>1) fields[rows][col].hue1bent-=1;
        if(fields[rows][col].hue1bent<0) fields[rows][col].hue1bent+=1;
        fields[rows][col].pressed=0;
        if(col>1 && bendHoriz) {
            fields[rows][col-1].type=BEND_VERT_HORIZ;
            fields[rows][col-1].f1->setMidinote(fields[rows][col-2].f1->getMidinote(),MisuWidget::MWPitch[fields[rows][col-2].f1->getRootNote()]);
            fields[rows][col-1].hue1bent=fields[rows][col-1].f1->getHue()+huePerNote*(float)bendVertTop;
            if(fields[rows][col-1].hue1bent>1) fields[rows][col-1].hue1bent-=1;
            if(fields[rows][col-1].hue1bent<0) fields[rows][col-1].hue1bent+=1;
            fields[rows][col-1].f2->setMidinote(midinote,MisuWidget::MWPitch[rootNote]);
            fields[rows][col-1].hue2bent=fields[rows][col-1].f2->getHue()+huePerNote*(float)bendVertTop;
            if(fields[rows][col-1].hue2bent>1) fields[rows][col-1].hue2bent-=1;
            if(fields[rows][col-1].hue2bent<0) fields[rows][col-1].hue2bent+=1;
            fields[rows][col-1].pressed=0;
        }
        rows++;
    }
    fields[rows][col].type=NORMAL;
    fields[rows][col].f1->setMidinote(midinote,MisuWidget::MWPitch[rootNote]);
    //qDebug() << "set f1 " << midinote << " " << fields[rows][col].f1;
    fields[rows][col].pressed=0;
    if(col>1 && bendHoriz) {
        fields[rows][col-1].type=BEND_HORIZ;
        fields[rows][col-1].f1->setMidinote(fields[rows][col-2].f1->getMidinote(),MisuWidget::MWPitch[fields[rows][col-2].f1->getRootNote()]);
        fields[rows][col-1].f2->setMidinote(midinote,MisuWidget::MWPitch[rootNote]);
        fields[rows][col-1].pressed=0;
    }
    rows++;
    if(bendVertBot!=0) {
        fields[rows][col].type=BEND_VERT;
        fields[rows][col].f1->setMidinote(midinote,MisuWidget::MWPitch[rootNote]);
        fields[rows][col].hue1bent=fields[rows][col].f1->getHue()+huePerNote*(float)bendVertBot;
        if(fields[rows][col].hue1bent>1) fields[rows][col].hue1bent-=1;
        if(fields[rows][col].hue1bent<0) fields[rows][col].hue1bent+=1;
        fields[rows][col].pressed=0;
        if(col>1 && bendHoriz) {
            fields[rows][col-1].type=BEND_VERT_HORIZ;
            fields[rows][col-1].f1->setMidinote(fields[rows][col-2].f1->getMidinote(),MisuWidget::MWPitch[fields[rows][col-2].f1->getRootNote()]);
            fields[rows][col-1].hue1bent=fields[rows][col-1].f1->getHue()+huePerNote*(float)bendVertBot;
            if(fields[rows][col-1].hue1bent>1) fields[rows][col-1].hue1bent-=1;
            if(fields[rows][col-1].hue1bent<0) fields[rows][col-1].hue1bent+=1;
            fields[rows][col-1].f2->setMidinote(midinote,MisuWidget::MWPitch[rootNote]);
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
    MisuWidget::playFieldWidth=MisuWidget::playAreaWidth/cols;
    MisuWidget::playFieldHeight=MisuWidget::playAreaHeight/rows;
    for(int i=0;i<cols;i++) {
        colwidth[i]=MisuWidget::playFieldWidth;
    }
    for(int i=0;i<rows;i++) {
        rowheight[i]=MisuWidget::playFieldHeight;
    }
}

int MWPlayArea::getMidinoteAtField(int i)
{
    return fields[0][i].f1->getMidinote();
}

void MWPlayArea::processTouchEvent(misuTouchEvent e)
{
    //qDebug() << "MWPlayArea::processPoint " << e.id << " x " << e.x << " y " << e.y << " t " << e.t;

    if(e.id<0) {
        qDebug() << "ignoring touch event with negative id " << e.id;
        return;
    }

    int eventHash=e.id%64;
    eventStackElement * es = &eventStack[eventHash];
    es->x=e.x;
    es->y=e.y;
    int row=0;

    if(MisuWidget::playAreaHeight>0) row = e.y*rows/MisuWidget::playAreaHeight;
    int col=e.x*cols/MisuWidget::playAreaWidth;

    float yrel=(float)(e.y-row*rowheight[row])/(float)rowheight[row];
    float xrel=(float)(e.x-col*colwidth[col])/(float)colwidth[col];

    MWPlayfield * pf = &fields[row][col];
    //qDebug() << "row " << row << " col " << col << " eventHash " << eventHash;

    float freq;
    int midinote;
    int pitch=0;
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
        es->voiceId=out->noteOn(MisuWidget::channel,freq,midinote,pitch,velocity);
        pf->pressed++;
        break;

    case Qt::TouchPointMoved:
        if(row!=es->row || col!=es->col) {
            MWPlayfield * ppf = &fields[es->row][es->col];
            ppf->pressed--;
            ppf->calcColor();
            out->noteOff(es->voiceId);

            es->midinote=midinote;
            es->voiceId=out->noteOn(MisuWidget::channel,freq,midinote,pitch,velocity);

            es->row=row;
            es->col=col;
            es->f=freq;
            pf->pressed++;
        } else if(freq!=es->f) {
            out->pitch(MisuWidget::channel,es->voiceId,freq,midinote,pitch);
            es->f=freq;
        }

        if(MisuWidget::sendCC1) {
            out->cc(MisuWidget::channel,es->voiceId,1,1.0f-yrel,1.0f-yrel);
        }
        break;

    case Qt::TouchPointReleased:

        out->noteOff(es->voiceId);
        if(pf->pressed>0) pf->pressed--;

        es->eventId=-1;
        es->row=-1;
        es->col=-1;
        break;
    }
    pf->calcColor();
}

void MWPlayArea::setRootNote(Pitch *p)
{
    MisuWidget::Scale.rootNote=p->getRootNote();
    config();
}


void MWPlayArea::setOctConf(int bottom, int top)
{
    MisuWidget::Scale.baseoct=bottom;
    MisuWidget::Scale.topoct=top;
    config();

}

void MWPlayArea::setBscale(int n, bool v)
{
    //qDebug() << "MWPlayArea::setBscale " << n << " " << v;
    MisuWidget::Scale.bscale[n-1]=v;
    config();
}

void MWPlayArea::setScale(MWScale * s)
{
    MisuWidget::Scale.rootNote=s->rootNote;
    MisuWidget::Scale.size=2;
    for(int i=0;i<BSCALE_SIZE;i++) {
        MisuWidget::Scale.bscale[i]=s->bscale[i];
        if(MisuWidget::Scale.bscale[i]) MisuWidget::Scale.size+=MisuWidget::Scale.topoct-MisuWidget::Scale.baseoct;
    }
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

void MWPlayArea::setOut(ISender *value)
{
    delete(out);
    out = value;
    //qDebug() << "MWPlayArea::setOut:" << out;
}

void MWPlayArea::resize(int w, int h)
{
    //qDebug() << "MWPlayArea::resize w: " << w << " h: " << h;
    MisuWidget::playAreaWidth=w;
    MisuWidget::playAreaHeight=h;
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


void MWPlayArea::onToggleBW()
{
    config();
}

void MWPlayArea::onSymbolsChanged()
{
    config();
}

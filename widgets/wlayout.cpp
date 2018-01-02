#include "wlayout.h"
#include <QPushButton>
#include <QDebug>
#include <QStackedWidget>
#include "mwbscaleswitch.h"
#include "mwheadersetter.h"
#include "mwpreset.h"
#include "mwfaderpitch.h"
#include "mwfaderparamctl.h".h"
#include <conf/color.h>

wlayout::wlayout(QWidget *parent) : QWidget(parent)
{
    setAttribute(Qt::WA_AcceptTouchEvents,true);

    qDebug() << "wlayout width: " << width() ;
    //QString cap;

    ISender * out=new SenderMobileSynth();
    //ISender * out=new SenderDebug();
    out->cc(0,0,105,1,1);


    for(int i=0;i<BSCALE_SIZE+1;i++) {
        MWPitch[i]=new Pitch(this);
        MWPitch[i]->setBasenote(i);
        MWPitch[i]->setPitch(0);
    }

    M[0] = new MWPlayArea(MWPitch,this);
    ((MWPlayArea *)M[0])->setOut(out);
    connect(M[0],SIGNAL(menuTouch()), this, SLOT(toggleMenu()));

    for(int i=0;i<BSCALE_SIZE+1;i++) {
        connect( MWPitch[i], SIGNAL(change()), (MWPlayArea *)M[0], SLOT(pitchChange()));
    }

    H[0] = new MWOctaveRanger(this);
    connect(H[0],SIGNAL(setOctConf(int,int)),M[0],SLOT(setOctConf(int,int)));

    M[1] = new QWidget(this);
    QGridLayout * lPitch = new QGridLayout(M[1]);
    lPitch->setContentsMargins(0,0,0,0);
    lPitch->setHorizontalSpacing(0);
    lPitch->setVerticalSpacing(0);
    for(int i=0;i<BSCALE_SIZE+1;i++) {
        MWFaderPitch * mwf = new MWFaderPitch(M[1],MWPitch[i],1);
        mwf->setOut(out);
        connect (mwf,SIGNAL(valueChange(int)),MWPitch[i],SLOT(setPitch(int)));
        connect( MWPitch[i], SIGNAL(change()), mwf, SLOT(pitchChange()));
        connect(H[0],SIGNAL(setOctMid(int)),mwf,SLOT(setOctMid(int)));
        lPitch->addWidget(mwf,0,i);
    }

    Color * synthCtlColor=new Pitch();

    M[2] = new QWidget(this);
    QGridLayout * lSynthCtl = new QGridLayout(M[2]);
    lSynthCtl->setContentsMargins(0,0,0,0);
    lSynthCtl->setHorizontalSpacing(0);
    lSynthCtl->setVerticalSpacing(0);
    for(int i=102;i<=111;i++) {
        MWFaderParamCtl * mwf = new MWFaderParamCtl(M[2],synthCtlColor,i,1);
        mwf->setOut(out);
        mwf->setMinValue(0);
        if(i==102) {
            mwf->setMaxValue(4);
        } else {
            mwf->setMaxValue(127);
        }
        mwf->setInverted(true);
        lSynthCtl->addWidget(mwf,0,i-102);
    }

    mainArea = new QStackedWidget(this);

    H[1] = new QWidget(this);
    QGridLayout * lBaseNoteSetter=new QGridLayout(H[1]);
    lBaseNoteSetter->setContentsMargins(0,0,0,0);
    lBaseNoteSetter->setHorizontalSpacing(0);
    lBaseNoteSetter->setVerticalSpacing(0);
    for(int i=0;i<12;i++) {
        BaseNoteSetter[i] = new MWBaseNoteSetter(MWPitch[i],this);
        BaseNoteSetter[i]->setOut(out);
        connect(BaseNoteSetter[i],SIGNAL(setBaseNote(Pitch *)),M[0],SLOT(setBaseNote(Pitch *)));
        connect(H[0],SIGNAL(setOctMid(int)),BaseNoteSetter[i],SLOT(setOctMid(int)));
        connect(MWPitch[i], SIGNAL(change()) ,BaseNoteSetter[i], SLOT(pitchChange()));
        lBaseNoteSetter->addWidget(BaseNoteSetter[i],0,i);
        //l2->setColumnStretch(i,10);
        //BaseNoteSetter[i]->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
        //BaseNoteSetter[i]->setMinimumWidth(100);
    }
    //parentLayout2->setLayout(l2);

    H[2] = new QWidget(this);
    QGridLayout * lBScaleSwitch=new QGridLayout(H[2]);
    lBScaleSwitch->setContentsMargins(0,0,0,0);
    lBScaleSwitch->setHorizontalSpacing(0);
    lBScaleSwitch->setVerticalSpacing(0);
    for(int i=1;i<12;i++) {
        MWBScaleSwitch * bsw = new MWBScaleSwitch(i,MWPitch);
        bsw->setOut(out);
        connect(bsw,SIGNAL(setBscale(int,bool)),M[0],SLOT(setBscale(int,bool)));
        connect(H[0],SIGNAL(setOctMid(int)),bsw,SLOT(setOctMid(int)));
        for(int j=0;j<12;j++) {
            connect(BaseNoteSetter[j],SIGNAL(setBaseNote(Pitch *)),bsw,SLOT(setBaseNote(Pitch *)));
        }
        lBScaleSwitch->addWidget(bsw,0,i);
    }

    layout = new QGridLayout(this);
    //layout->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    layout->setSizeConstraint(QLayout::SetMinimumSize);
    layout->setContentsMargins(0,0,0,0);
    layout->setMargin(0);
    layout->setHorizontalSpacing(0);
    layout->setVerticalSpacing(0);

    for(int i=0;i<3;i++) {
        H[i]->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
        //H[i]->setSizeConstraint(QLayout::SetMinimumSize);
        H[i]->setContentsMargins(0,0,0,0);
        //H[i]->setMargin(0);
        //H[i]->setHorizontalSpacing(0);
        //H[i]->setVerticalSpacing(0);
        //layout->addWidget(H[i],i,2,1,10);
    }

    M[0]->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    M[0]->setContentsMargins(0,0,0,0);
    //layout->addWidget(M[0],3,2,4,10);
    M[1]->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    M[1]->setContentsMargins(0,0,0,0);
    //layout->addWidget(M[1],7,2,4,10);
    M[2]->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    M[2]->setContentsMargins(0,0,0,0);
    //layout->addWidget(M[2],11,2,4,10);

    //qDebug() << "wlayout::wlayout new out " << out;

    for(int i=0;i<15;i++) {
        //QPushButton * pb = new QPushButton(this);
        PB[i] = new MWPreset(MWPitch,this);
        connect(PB[i],SIGNAL(setScale(MWScale*)),(MWPlayArea *)M[0],SLOT(setScale(MWScale*)));
        //cap.sprintf("%d",i);
        //pb->setText(cap);
        PB[i]->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

        int fctId=i;
        HS[i] = new MWHeaderSetter(fctId,this);
        HS[i]->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
        if(fctId<3) {
            connect(HS[i],SIGNAL(currentHeader(int)),this,SLOT(currentHeader(int)));
        } else if(fctId==3) {
            connect(HS[i],SIGNAL(setBendHori(bool)),(MWPlayArea *)M[0],SLOT(setBendHori(bool)));
        } else if(fctId==4) {
            connect(HS[i],SIGNAL(setBendVertTop(int)),(MWPlayArea *)M[0],SLOT(setBendVertTop(int)));
        } else if(fctId==5) {
            connect(HS[i],SIGNAL(setBendVertBot(int)),(MWPlayArea *)M[0],SLOT(setBendVertBot(int)));
        } else if(fctId==6) {
            connect(HS[i],SIGNAL(currentMainView(int)),this,SLOT(currentMainView(int)));
        } else if(fctId==7) {
            connect(HS[i],SIGNAL(currentMainView(int)),this,SLOT(currentMainView(int)));
        } else if(fctId==8) {
            connect(HS[i],SIGNAL(currentMainView(int)),this,SLOT(currentMainView(int)));
        } else if(fctId==9) {
            connect(HS[i],SIGNAL(togglePresets()),this,SLOT(togglePresets()));
        } else if(fctId==10) {
            connect(HS[i],SIGNAL(toggleMenu()),this,SLOT(toggleMenu()));
        }

        //layout->addWidget(PB[i],i,0,1,2);
        //layout->addWidget(HS[i],i,12,1,2);
    }


    //layout->addWidget(mainArea,3,2,10,10);
    layout->setContentsMargins(0,0,0,0);
    layout->setMargin(0);
    layout->setHorizontalSpacing(0);
    layout->setVerticalSpacing(0);


    H[0]->hide();
    M[1]->hide();
    M[2]->hide();

    recalcMainView();

    this->setLayout(layout);

}

wlayout::~wlayout()
{

}

void wlayout::resizeEvent(QResizeEvent *E)
{
    qDebug() << "wlayout::resizeEvent " << width() << " " << height();
    /*
    int w=width()/12;
    for(int i=0;i<12;i++) {
        BaseNoteSetter[i]->setMinimumWidth(w-4);
    }
    */
}

void wlayout::currentHeader(int i)
{
    if(H[i]->isHidden()) {
        H[i]->show();
    } else {
        H[i]->hide();
    }
    recalcMainView();
}

void wlayout::currentMainView(int i)
{
    //mainArea->setCurrentIndex(i);
    if(M[i]->isHidden()) {
        M[i]->show();
    } else {
        M[i]->hide();
    }
    recalcMainView();
}

void wlayout::recalcMainView()
{
    int mainCnt=0;
    int headerCnt=0;
    for(int i=0;i<3;i++) {
        layout->removeWidget(M[i]);
        if(!M[i]->isHidden()) {mainCnt++;}

        layout->removeWidget(H[i]);
        if(!H[i]->isHidden()) {headerCnt++;}
    }
    for(int i=0;i<15;i++) {
        layout->removeWidget(HS[i]);
        layout->removeWidget(PB[i]);
    }

    if(mainCnt==0) {
        M[0]->show();
        mainCnt=1;
    }

    int height=(15-headerCnt)/mainCnt;
    int roundDiff=15-headerCnt-mainCnt*height;
    int top=0;

    int xpos = 0;
    int width = 14;

    if(!PB[0]->isHidden()) {
        width-=2;
        xpos+=2;
    }
    if(!HS[0]->isHidden()) {
        width-=2;
    }

    for(int i=0;i<3;i++) {
        if(!H[i]->isHidden()) {
            qDebug() << "layout->addWidget "  << i << " top " << top << " xpos " << xpos << " h " << height << " w " << width;
            layout->addWidget(H[i],top,xpos,1,width);
            top++;
        }
    }

    for(int i=0;i<3;i++) {
        if(!M[i]->isHidden()) {
            qDebug() << "layout->addWidget "  << i << " top " << top << " xpos " << xpos << " h " << height << " w " << width;
            layout->addWidget(M[i],top,xpos,height+roundDiff,width);
            top+=height+roundDiff;
            roundDiff=0;
        }
    }

    for(int i=0;i<15;i++) {
        if(!PB[i]->isHidden()) {
            layout->addWidget(PB[i],i,0,1,2);
            qDebug() << "layout->addWidget Preset Button "  << i;
        }
    }

    for(int i=0;i<15;i++) {
        if(!HS[i]->isHidden()) {
            layout->addWidget(HS[i],i,xpos+width,1,2);
        }
    }

}

void wlayout::changePitch(int v)
{
    qDebug() << "changePitch " << v;
}

void wlayout::togglePresets()
{
    for(int i=0;i<15;i++) {
        if(PB[i]->isHidden()) {
            PB[i]->show();
        } else {
            PB[i]->hide();
        }
    }
    recalcMainView();
}

void wlayout::toggleMenu()
{
    for(int i=0;i<15;i++) {
        if(HS[i]->isHidden()) {
            HS[i]->show();
        } else {
            HS[i]->hide();
        }
    }
    recalcMainView();
}

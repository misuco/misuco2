#include "wlayout.h"
#include <QPushButton>
#include <QDebug>
#include <QStackedWidget>
#include "mwbscaleswitch.h"
#include "mwheadersetter.h"
#include "mwpreset.h"
//#include <QSlider>
#include "mwfaderpitch.h"
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

    for(int i=0;i<BSCALE_SIZE+1;i++) {
        connect( MWPitch[i], SIGNAL(change()), (MWPlayArea *)M[0], SLOT(pitchChange()));
    }


    H[0] = new MWOctaveRanger(this);
    //OctaveRanger->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    connect(H[0],SIGNAL(setOctConf(int,int)),M[0],SLOT(setOctConf(int,int)));


    M[1] = new QWidget(this);
    QGridLayout * lPitch = new QGridLayout(M[1]);
    for(int i=0;i<BSCALE_SIZE+1;i++) {
        /*
        //QHSlider
        QSlider * qs = new QSlider(M[1]);
        qs->setRange(-1200,1200);
        qs->setValue(0);
        //qs->set
        connect(qs,SIGNAL(valueChanged(int)),this,SLOT(changePitch(int)));
        lPitch->addWidget(qs,0,i);
        */

        /*
        MWFadder * mwf = new MWFadder(M[1],static_cast<Color *>(MWPitch[i]));
        connect (mwf,SIGNAL(valueChange(int)),MWPitch[i],SLOT(setPitch(int)));
        */

        MWFaderPitch * mwf = new MWFaderPitch(M[1],MWPitch[i],1);
        mwf->setOut(out);
        connect (mwf,SIGNAL(valueChange(int)),MWPitch[i],SLOT(setPitch(int)));
        connect( MWPitch[i], SIGNAL(change()), mwf, SLOT(pitchChange()));
        connect(H[0],SIGNAL(setOctMid(int)),mwf,SLOT(setOctMid(int)));

        lPitch->addWidget(mwf,0,i);
    }

    /*
    QStackedWidget* stackedWidget = new QStackedWidget(this);
    stackedWidget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    */

    mainArea = new QStackedWidget(this);
    //mainArea->addWidget(M[0]);
    //mainArea->addWidget(M[1]);

    //mainArea->setCurrentIndex(0);


    H[1] = new QWidget(this);
    QGridLayout * lBaseNoteSetter=new QGridLayout(H[1]);
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

    /*
    header = new QStackedWidget(this);

    for(int i=0;i<3;i++) {
        H[i]->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
        //H[i]->setSizeConstraint(QLayout::SetMinimumSize);
        H[i]->setContentsMargins(0,0,0,0);
        //H[i]->setMargin(0);
        //H[i]->setHorizontalSpacing(0);
        //H[i]->setVerticalSpacing(0);
        header->addWidget(H[i]);
    }
    */

    layout = new QGridLayout(this);
    //layout->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    layout->setSizeConstraint(QLayout::SetMinimumSize);
    layout->setContentsMargins(0,0,0,0);
    layout->setMargin(0);
    layout->setHorizontalSpacing(0);
    layout->setVerticalSpacing(0);

    //layout->addWidget(header,0,0,1,14);
    for(int i=0;i<3;i++) {
        H[i]->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
        //H[i]->setSizeConstraint(QLayout::SetMinimumSize);
        H[i]->setContentsMargins(0,0,0,0);
        //H[i]->setMargin(0);
        //H[i]->setHorizontalSpacing(0);
        //H[i]->setVerticalSpacing(0);
        layout->addWidget(H[i],i,0,1,14);
    }

    M[0]->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    M[0]->setContentsMargins(0,0,0,0);
    layout->addWidget(M[0],3,2,5,10);
    M[1]->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    M[1]->setContentsMargins(0,0,0,0);
    layout->addWidget(M[1],8,2,5,10);

    //qDebug() << "wlayout::wlayout new out " << out;

    for(int i=1;i<11;i++) {
        //QPushButton * pb = new QPushButton(this);
        MWPreset * pb = new MWPreset(MWPitch,this);
        connect(pb,SIGNAL(setScale(MWScale*)),(MWPlayArea *)M[0],SLOT(setScale(MWScale*)));
        //cap.sprintf("%d",i);
        //pb->setText(cap);
        pb->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

        int fctId=i-1;
        MWHeaderSetter * hs = new MWHeaderSetter(fctId,this);
        hs->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
        if(fctId<3) {
            connect(hs,SIGNAL(currentHeader(int)),this,SLOT(currentHeader(int)));
        } else if(fctId==3) {
            connect(hs,SIGNAL(setBendHori(bool)),(MWPlayArea *)M[0],SLOT(setBendHori(bool)));
        } else if(fctId==4) {
            connect(hs,SIGNAL(setBendVertTop(int)),(MWPlayArea *)M[0],SLOT(setBendVertTop(int)));
        } else if(fctId==5) {
            connect(hs,SIGNAL(setBendVertBot(int)),(MWPlayArea *)M[0],SLOT(setBendVertBot(int)));
        } else if(fctId==6) {
            connect(hs,SIGNAL(currentMainView(int)),this,SLOT(currentMainView(int)));
        } else if(fctId==7) {
            connect(hs,SIGNAL(currentMainView(int)),this,SLOT(currentMainView(int)));
        }

        layout->addWidget(pb,i+2,0,1,2);
        layout->addWidget(hs,i+2,12,1,2);
    }

    //layout->addWidget(mainArea,3,2,10,10);
    layout->setContentsMargins(0,0,0,0);
    layout->setMargin(0);
    layout->setHorizontalSpacing(0);
    layout->setVerticalSpacing(0);

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

    //header->setCurrentIndex(i);

    if(H[i]->isHidden()) {
        H[i]->show();
    } else {
        H[i]->hide();
    }


    /*
     *
    if(i>0 && i<4) {
        layout->replaceWidget(header,H[i-1]);
        header=H[i-1];
        H[i-1]->show();
        for(int j=0;j<3;j++) {
            if(j!=i-1) {
                H[j]->hide();
            }
        }
    }
     *
     */
}

void wlayout::currentMainView(int i)
{
    //mainArea->setCurrentIndex(i);
    if(M[i]->isHidden()) {
        M[i]->show();
    } else {
        M[i]->hide();
    }
    layout->removeWidget(M[0]);
    layout->removeWidget(M[1]);
    if(!M[0]->isHidden() && !M[1]->isHidden() ) {
        layout->addWidget(M[0],3,2,5,10);
        layout->addWidget(M[1],8,2,5,10);
    } else if(!M[0]->isHidden() ) {
        layout->addWidget(M[0],3,2,10,10);
    } else if(!M[1]->isHidden() ) {
        layout->addWidget(M[1],3,2,10,10);
    }
}

void wlayout::changePitch(int v)
{
    qDebug() << "changePitch " << v;
}


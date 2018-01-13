#include "wlayout.h"
#include <QPushButton>
#include <QDebug>
#include <QStackedWidget>
#include "mwpreset.h"
#include "mwsoundpreset.h"
#include "mwfaderpitch.h"
#include <conf/color.h>

wlayout::wlayout(QWidget *parent) : QWidget(parent)
{
    setAttribute(Qt::WA_AcceptTouchEvents,true);

    //qDebug() << "wlayout width: " << width() ;

    //ISender * out=new SenderMobileSynth();
    out=new SenderMulti();
    out->cc(0,0,105,1,1);

    for(int i=0;i<nPresetBtn+nSoundPresetBtn;i++) {
        PB[i] = new MWPreset(MWPitch,this);
    }

    for(int i=0;i<BSCALE_SIZE+1;i++) {
        MWPitch[i]=new Pitch(this);
        MWPitch[i]->setBasenote(i);
        MWPitch[i]->setPitch(0);
    }

    M[0] = new MWPlayArea(MWPitch,this);
    ((MWPlayArea *)M[0])->setOut(out);
    connect(M[0],SIGNAL(menuTouch()), this, SLOT(toggleMenu()));
    connect(M[0],SIGNAL(presetsTouch()), this, SLOT(togglePresets()));

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
        MWFaderPitch * mwf = new MWFaderPitch(M[1],MWPitch[i]);
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
    for(int i=0;i<10;i++) {
        faderParamCtl[i] = new MWFaderParamCtl(M[2],synthCtlColor,i+102);
        faderParamCtl[i]->setOut(out);
        faderParamCtl[i]->setMinValue(0);
        if(i==0) {
            faderParamCtl[i]->setMaxValue(4);
        } else {
            faderParamCtl[i]->setMaxValue(1000);
        }
        faderParamCtl[i]->setInverted(true);
        lSynthCtl->addWidget(faderParamCtl[i],0,i);

        // update fadders on sustain update
        if(i==3) {
            connect(faderParamCtl[i],SIGNAL(valueChange(int)),this,SLOT(onSoundSustainUpdate(int)));
        }
        // update fadders on filter res update
        if(i==6) {
            connect(faderParamCtl[i],SIGNAL(valueChange(int)),this,SLOT(onSoundSustainUpdate(int)));
        }
    }

    M[3] = new QWidget(this);
    QGridLayout * lPlayAreaCtl = new QGridLayout(M[3]);
    lPlayAreaCtl->setContentsMargins(0,0,0,0);
    lPlayAreaCtl->setHorizontalSpacing(0);
    lPlayAreaCtl->setVerticalSpacing(0);

    faderPitchTopRange = new MWFaderParamCtl(M[2],synthCtlColor,1);
    faderPitchTopRange->setOut(out);
    faderPitchTopRange->setMinValue(-5);
    faderPitchTopRange->setMaxValue(5);
    lPlayAreaCtl->addWidget(faderPitchTopRange,0,0);
    connect(faderPitchTopRange,SIGNAL(valueChange(int)),M[0],SLOT(setBendVertTop(int)));

    faderPitchBottomRange = new MWFaderParamCtl(M[2],synthCtlColor,2);
    faderPitchBottomRange->setOut(out);
    faderPitchBottomRange->setMinValue(-5);
    faderPitchBottomRange->setMaxValue(5);
    lPlayAreaCtl->addWidget(faderPitchBottomRange,0,1);
    connect(faderPitchBottomRange,SIGNAL(valueChange(int)),M[0],SLOT(setBendVertBot(int)));

    pitchVert = new MWHeaderSetter(15,this);
    lPlayAreaCtl->addWidget(pitchVert,0,2);
    connect(pitchVert,SIGNAL(setBendHori(bool)),M[0],SLOT(setBendHori(bool)));

    faderChannel = new MWFaderParamCtl(M[2],synthCtlColor,3);
    faderChannel->setOut(out);
    faderChannel->setMinValue(1);
    faderChannel->setMaxValue(16);
    faderChannel->setInverted(true);
    lPlayAreaCtl->addWidget(faderChannel,0,3);
    connect(faderChannel,SIGNAL(valueChange(int)),this,SLOT(onChannelChange(int)));

    enableCc1 = new MWHeaderSetter(16,1,this);
    MisuWidget::sendCC1 = true;
    lPlayAreaCtl->addWidget(enableCc1,0,4);

    bwMode = new MWHeaderSetter(11,this);
    lPlayAreaCtl->addWidget(bwMode,0,5);
    connect(bwMode,SIGNAL(toggleBW()),M[0],SLOT(toggleBW()));
    connect(bwMode,SIGNAL(toggleBW()),this,SLOT(toggleBW()));

    enableMobilesynth = new MWHeaderSetter(17,1,this);
    lPlayAreaCtl->addWidget(enableMobilesynth,0,6);
    connect(enableMobilesynth,SIGNAL(toggleSender(int)),this,SLOT(onToggleSender(int)));

    enablePuredata = new MWHeaderSetter(18,this);
    lPlayAreaCtl->addWidget(enablePuredata,0,7);
    connect(enablePuredata,SIGNAL(toggleSender(int)),this,SLOT(onToggleSender(int)));

    enableReaktor = new MWHeaderSetter(19,1,this);
    lPlayAreaCtl->addWidget(enableReaktor,0,8);
    connect(enableReaktor,SIGNAL(toggleSender(int)),this,SLOT(onToggleSender(int)));

    enableSupercollider = new MWHeaderSetter(20,this);
    lPlayAreaCtl->addWidget(enableSupercollider,0,9);
    connect(enableSupercollider,SIGNAL(toggleSender(int)),this,SLOT(onToggleSender(int)));

    holdMode = new MWHeaderSetter(21,this);
    lPlayAreaCtl->addWidget(holdMode,0,10);

    overwritePreset = new MWHeaderSetter(12,this);
    overwritePreset->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

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
        connect(BaseNoteSetter[i],SIGNAL(setBaseNote(Pitch *)),this,SLOT(onSetBaseNote(Pitch *)));
        connect(BaseNoteSetter[i],SIGNAL(scaleUpdate()),this,SLOT(onScaleUpdate()));
        connect(this,SIGNAL(setBaseNote(Pitch*)),BaseNoteSetter[i],SLOT(onSetBaseNote(Pitch*)));
        connect(H[0],SIGNAL(setOctMid(int)),BaseNoteSetter[i],SLOT(setOctMid(int)));
        connect(MWPitch[i], SIGNAL(change()) ,BaseNoteSetter[i], SLOT(pitchChange()));
        lBaseNoteSetter->addWidget(BaseNoteSetter[i],0,i);
    }

    H[2] = new QWidget(this);
    QGridLayout * lBScaleSwitch=new QGridLayout(H[2]);
    lBScaleSwitch->setContentsMargins(0,0,0,0);
    lBScaleSwitch->setHorizontalSpacing(0);
    lBScaleSwitch->setVerticalSpacing(0);
    for(int i=1;i<12;i++) {
        bScaleSwitch[i-1] = new MWBScaleSwitch(i,MWPitch);
        bScaleSwitch[i-1]->setOut(out);
        connect(bScaleSwitch[i-1],SIGNAL(setBscale(int,bool)),M[0],SLOT(setBscale(int,bool)));
        connect(bScaleSwitch[i-1],SIGNAL(scaleUpdate()),this,SLOT(onScaleUpdate()));
        connect(H[0],SIGNAL(setOctMid(int)),bScaleSwitch[i-1],SLOT(setOctMid(int)));
        for(int j=0;j<12;j++) {
            connect(BaseNoteSetter[j],SIGNAL(setBaseNote(Pitch *)),bScaleSwitch[i-1],SLOT(setBaseNote(Pitch *)));
        }
        lBScaleSwitch->addWidget(bScaleSwitch[i-1],0,i);
    }

    openScalesArchive = new MWHeaderSetter(13,1,this);
    lBScaleSwitch->addWidget(openScalesArchive,0,12);

    faderSymbols = new MWFaderParamCtl(M[2],synthCtlColor,4);
    faderSymbols->setOut(out);
    faderSymbols->setMinValue(0);
    faderSymbols->setMaxValue(4);
    faderSymbols->setInverted(true);
    lPlayAreaCtl->addWidget(faderSymbols,0,11);
    connect(faderSymbols,SIGNAL(valueChange(int)),this,SLOT(onSymbolsChange(int)));
    connect(this,SIGNAL(scaleUpdate()),M[0],SLOT(onScaleUpdate()));
    connect(this,SIGNAL(scaleUpdate()),BaseNoteSetter[0],SLOT(onScaleUpdate()));
    for(int j=1;j<12;j++) {
        connect(this,SIGNAL(scaleUpdate()),BaseNoteSetter[j],SLOT(onScaleUpdate()));
        connect(this,SIGNAL(scaleUpdate()),bScaleSwitch[j-1],SLOT(onScaleUpdate()));
    }

    showFreqs = new MWHeaderSetter(22,this);
    lPlayAreaCtl->addWidget(showFreqs,0,12);
    connect(showFreqs,SIGNAL(toggleShowFreqs()),this,SLOT(onShowFreqsChange()));

    layout = new QGridLayout(this);
    layout->setSizeConstraint(QLayout::SetMinimumSize);
    layout->setContentsMargins(0,0,0,0);
    layout->setMargin(0);
    layout->setHorizontalSpacing(0);
    layout->setVerticalSpacing(0);

    for(int i=0;i<3;i++) {
        H[i]->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
        H[i]->setContentsMargins(0,0,0,0);
    }

    M[0]->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    M[0]->setContentsMargins(0,0,0,0);
    M[1]->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    M[1]->setContentsMargins(0,0,0,0);
    M[2]->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    M[2]->setContentsMargins(0,0,0,0);

    //qDebug() << "wlayout::wlayout new out " << out;

    for(int i=0;i<nPresetBtn;i++) {
        connect(PB[i],SIGNAL(setScale(MWScale*)),(MWPlayArea *)M[0],SLOT(setScale(MWScale*)));
        connect(PB[i],SIGNAL(scaleUpdate()),this,SLOT(onScaleUpdate()));
        connect(PB[i],SIGNAL(setScale(MWScale*)),BaseNoteSetter[0],SLOT(onScaleSet(MWScale*)));
        for(int j=1;j<12;j++) {
            connect(PB[i],SIGNAL(setScale(MWScale*)),BaseNoteSetter[j],SLOT(onScaleSet(MWScale*)));
            connect(PB[i],SIGNAL(setScale(MWScale*)),bScaleSwitch[j-1],SLOT(onScaleSet(MWScale*)));
        }
        PB[i]->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    }

    for(int i=nPresetBtn;i<nPresetBtn+nSoundPresetBtn;i++) {
        PB[i] = new MWSoundPreset(this);
        connect(PB[i],SIGNAL(setSound(MWSound*)),this,SLOT(setSound(MWSound*)));
        PB[i]->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    }

    for(int i=0;i<7;i++) {
        int fctId=i;
        if(i>=3) fctId+=3;
        HS[i] = new MWHeaderSetter(fctId,this);
        HS[i]->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
        if(i<3) {
            connect(HS[i],SIGNAL(currentHeader(int)),this,SLOT(currentHeader(int)));
        } else if(i==3) {
            connect(HS[i],SIGNAL(currentMainView(int)),this,SLOT(currentMainView(int)));
        } else if(i==4) {
            connect(HS[i],SIGNAL(currentMainView(int)),this,SLOT(currentMainView(int)));
        } else if(i==5) {
            connect(HS[i],SIGNAL(currentMainView(int)),this,SLOT(currentMainView(int)));
        } else if(i==6) {
            connect(HS[i],SIGNAL(currentMainView(int)),this,SLOT(currentMainView(int)));
        }
    }

    layout->setContentsMargins(0,0,0,0);
    layout->setMargin(0);
    layout->setHorizontalSpacing(0);
    layout->setVerticalSpacing(0);

    H[0]->hide();
    M[1]->hide();
    M[2]->hide();

    connect(this,SIGNAL(initialSet()),PB[0],SLOT(initialSet()));
    connect(this,SIGNAL(initialSet()),PB[nPresetBtn],SLOT(initialSet()));
    emit initialSet();

    recalcMainView();

    this->setLayout(layout);

}

wlayout::~wlayout()
{

}

void wlayout::resizeEvent(QResizeEvent *)
{
    //qDebug() << "wlayout::resizeEvent " << width() << " " << height();
    MisuWidget::font1size=width()/40;
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

    for(int i=0;i<4;i++) {
        layout->removeWidget(M[i]);
        if(!M[i]->isHidden()) {mainCnt++;}
    }

    for(int i=0;i<3;i++) {
        layout->removeWidget(H[i]);
        if(!H[i]->isHidden()) {headerCnt+=2;}
    }

    for(int i=0;i<7;i++) {
        layout->removeWidget(HS[i]);
    }

    layout->removeWidget(overwritePreset);
    for(int i=1;i<nPresetBtn+nSoundPresetBtn;i++) {
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
            //qDebug() << "layout->addWidget "  << i << " top " << top << " xpos " << xpos << " h " << height << " w " << width;
            layout->addWidget(H[i],top,xpos,2,width);
            top+=2;
        }
    }

    for(int i=0;i<4;i++) {
        if(!M[i]->isHidden()) {
            //qDebug() << "layout->addWidget "  << i << " top " << top << " xpos " << xpos << " h " << height << " w " << width;
            layout->addWidget(M[i],top,xpos,height+roundDiff,width);
            top+=height+roundDiff;
            roundDiff=0;
        }
    }

    layout->addWidget(overwritePreset,0,0,1,2);
    for(int i=1;i<nPresetBtn+nSoundPresetBtn;i++) {
        if(!PB[i]->isHidden()) {
            layout->addWidget(PB[i],i*2,0,1,2);
            //qDebug() << "layout->addWidget Preset Button "  << i;
        }
    }

    for(int i=0;i<7;i++) {
        if(!HS[i]->isHidden()) {
            layout->addWidget(HS[i],i*2,xpos+width,2,2);
        }
    }

}

void wlayout::changePitch(int v)
{
    qDebug() << "changePitch " << v;
}

void wlayout::togglePresets()
{
    if(overwritePreset->isHidden()) {
        overwritePreset->show();
    } else {
        overwritePreset->hide();
    }
    for(int i=0;i<nPresetBtn+nSoundPresetBtn;i++) {
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
    for(int i=0;i<7;i++) {
        if(HS[i]->isHidden()) {
            HS[i]->show();
        } else {
            HS[i]->hide();
        }
    }
    recalcMainView();
}

void wlayout::toggleBW()
{
    for(int i=0;i<nPresetBtn+nSoundPresetBtn;i++) {
        PB[i]->update();
    }
    for(int i=0;i<3;i++) {
        H[i]->update();
        M[i]->update();
    }
}

void wlayout::onSetBaseNote(Pitch *p)
{
    emit setBaseNote(p);
}

void wlayout::setSound(MWSound *s)
{
    faderParamCtl[0]->setValue(s->wave_type);
    faderParamCtl[1]->setValue(s->attack);
    faderParamCtl[2]->setValue(s->decay);
    faderParamCtl[3]->setValue(s->sustain);
    faderParamCtl[4]->setValue(s->release);
    faderParamCtl[5]->setValue(s->filter_cutoff);
    faderParamCtl[6]->setValue(s->filter_resonance);
    faderParamCtl[7]->setValue(s->mod_filter_cutoff);
    faderParamCtl[8]->setValue(s->mod_filter_resonance);
    faderParamCtl[9]->setValue(s->volume);
    for(int i=nPresetBtn;i<nPresetBtn+nSoundPresetBtn;i++) {
        PB[i]->update();
    }
}

void wlayout::onScaleUpdate()
{
    for(int i=0;i<nPresetBtn+nSoundPresetBtn;i++) {
        PB[i]->update();
    }
}

void wlayout::onSoundSustainUpdate(int)
{
    faderParamCtl[2]->update();
    faderParamCtl[4]->update();
    faderParamCtl[5]->update();
}

void wlayout::onChannelChange(int v)
{
    MisuWidget::channel = v;
}

void wlayout::onToggleSender(int v)
{
    if(out->senderEnabled[v]) out->senderEnabled[v]=false;
    else out->senderEnabled[v]=true;
}

void wlayout::onSymbolsChange(int v)
{
    MisuWidget::noteSymbols = v;
    emit scaleUpdate();
}

void wlayout::onShowFreqsChange()
{
    emit scaleUpdate();
}


#include "wlayout.h"
#include <QPushButton>
#include <QDebug>
#include <QStackedWidget>
#include "mwbscaleswitch.h"
#include "mwheadersetter.h"

wlayout::wlayout(QWidget *parent) : QWidget(parent)
{
    qDebug() << "wlayout width: " << width() ;
    QString cap;    

    ISender * out=new SenderMobileSynth();
    out->cc(0,0,105,1,1);
    PlayArea = new MWPlayArea(this);
    PlayArea->setOut(out);

    /*
    QStackedWidget* stackedWidget = new QStackedWidget(this);
    stackedWidget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    */


    H[0] = new MWOctaveRanger(this);
    //OctaveRanger->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    connect(H[0],SIGNAL(setOctConf(int,int)),PlayArea,SLOT(setOctConf(int,int)));

    H[1] = new QWidget(this);
    QGridLayout * lBaseNoteSetter=new QGridLayout(H[1]);
    for(int i=0;i<12;i++) {
        BaseNoteSetter[i] = new MWBaseNoteSetter(i,this);
        BaseNoteSetter[i]->setOut(out);
        connect(BaseNoteSetter[i],SIGNAL(setBaseNote(int)),PlayArea,SLOT(setBaseNote(int)));
        connect(H[0],SIGNAL(setOctMid(int)),BaseNoteSetter[i],SLOT(setOctMid(int)));
        lBaseNoteSetter->addWidget(BaseNoteSetter[i],0,i);
        //l2->setColumnStretch(i,10);
        //BaseNoteSetter[i]->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
        //BaseNoteSetter[i]->setMinimumWidth(100);
    }
    //parentLayout2->setLayout(l2);

    H[2] = new QWidget(this);
    QGridLayout * lBScaleSwitch=new QGridLayout(H[2]);
    for(int i=0;i<11;i++) {
        MWBScaleSwitch * bsw = new MWBScaleSwitch(i);
        bsw->setOut(out);
        connect(bsw,SIGNAL(setBscale(int,bool)),PlayArea,SLOT(setBscale(int,bool)));
        connect(H[0],SIGNAL(setOctMid(int)),bsw,SLOT(setOctMid(int)));
        for(int j=0;j<12;j++) {
            connect(BaseNoteSetter[j],SIGNAL(setBaseNote(int)),bsw,SLOT(setBaseNote(int)));
        }
        lBScaleSwitch->addWidget(bsw,0,i);
    }

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

    layout = new QGridLayout(this);
    //layout->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    layout->setSizeConstraint(QLayout::SetMinimumSize);
    layout->setContentsMargins(0,0,0,0);
    layout->setMargin(0);
    layout->setHorizontalSpacing(0);
    layout->setVerticalSpacing(0);

    layout->addWidget(header,0,0,1,14);

    //qDebug() << "wlayout::wlayout new out " << out;

    for(int i=1;i<12;i++) {
        QPushButton * pb = new QPushButton(this);
        cap.sprintf("%d",i);
        pb->setText(cap);
        pb->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

        MWHeaderSetter * hs = new MWHeaderSetter(i-1,this);
        hs->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
        connect(hs,SIGNAL(currentHeader(int)),this,SLOT(currentHeader(int)));

        layout->addWidget(pb,i,0,1,2);
        layout->addWidget(hs,i,12,1,2);
    }

    layout->addWidget(PlayArea,1,2,10,10);
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

wlayout::currentHeader(int i)
{
    header->setCurrentIndex(i);

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


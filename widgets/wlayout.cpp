#include "wlayout.h"
#include <QPushButton>
#include <QDebug>
#include <QStackedWidget>
#include "mwbscaleswitch.h"
#include "mwheadersetter.h"

wlayout::wlayout(QWidget *parent) : QWidget(parent)
{
    qDebug() << "wlayout width: " << width() ;
    layout = new QGridLayout(this);
    QString cap;    

    ISender * out=new SenderMobileSynth();
    out->cc(0,0,105,1,1);
    PlayArea = new MWPlayArea(this);
    PlayArea->setOut(out);

    /*
    QStackedWidget* stackedWidget = new QStackedWidget(this);
    stackedWidget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    */

    for(int i=0;i<3;i++) {
        H[i]=new QWidget(this);
        H[i]->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

        //H[i]->setSizeConstraint(QLayout::SetMinimumSize);
        H[i]->setContentsMargins(0,0,0,0);
        /*
        H[i]->setMargin(0);
        H[i]->setHorizontalSpacing(0);
        H[i]->setVerticalSpacing(0);
         */
    }

    QGridLayout* l1 = new QGridLayout(H[0]);
    OctaveRanger = new MWOctaveRanger(this);
    //OctaveRanger->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    connect(OctaveRanger,SIGNAL(setOctConf(int,int)),PlayArea,SLOT(setOctConf(int,int)));
    l1->addWidget(OctaveRanger,0,0);

    QGridLayout* l2 = new QGridLayout(H[1]);

    for(int i=0;i<12;i++) {
        BaseNoteSetter[i] = new MWBaseNoteSetter(i,this);
        BaseNoteSetter[i]->setOut(out);
        connect(BaseNoteSetter[i],SIGNAL(setBaseNote(int)),PlayArea,SLOT(setBaseNote(int)));
        connect(OctaveRanger,SIGNAL(setOctMid(int)),BaseNoteSetter[i],SLOT(setOctMid(int)));
        l2->addWidget(BaseNoteSetter[i],0,i);
        //l2->setColumnStretch(i,10);
        //BaseNoteSetter[i]->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
        //BaseNoteSetter[i]->setMinimumWidth(100);
    }
    //parentLayout2->setLayout(l2);

    QBoxLayout* l3 = new QBoxLayout(QBoxLayout::LeftToRight,H[2]);
    for(int i=0;i<11;i++) {
        MWBScaleSwitch * bsw = new MWBScaleSwitch(i);
        bsw->setOut(out);
        connect(bsw,SIGNAL(setBscale(int,bool)),PlayArea,SLOT(setBscale(int,bool)));
        connect(OctaveRanger,SIGNAL(setOctMid(int)),bsw,SLOT(setOctMid(int)));
        for(int j=0;j<12;j++) {
            connect(BaseNoteSetter[j],SIGNAL(setBaseNote(int)),bsw,SLOT(setBaseNote(int)));
        }
        l3->addWidget(bsw);
    }

    /*
    stackedWidget->addWidget(parentLayout2);
    stackedWidget->addWidget(parentLayout1);
    stackedWidget->addWidget(parentLayout3);
    */

    H[0]->hide();
    H[2]->hide();
    layout->addWidget(H[1],0,0,1,14);
    header=H[1];

    //stackedWidget->widget(0)->show();
    //stackedWidget->widget(1)->hide();

    //qDebug() << "wlayout::wlayout new out " << out;

    for(int i=1;i<12;i++) {
        QPushButton * pb = new QPushButton(this);
        cap.sprintf("%d",i);
        pb->setText(cap);
        pb->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
        layout->addWidget(pb,i,0,1,2);
    }

    for(int i=1;i<12;i++) {
        MWHeaderSetter * hs = new MWHeaderSetter(i,this);
        hs->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
        layout->addWidget(hs,i,12,1,2);
        connect(hs,SIGNAL(currentHeader(int)),this,SLOT(currentHeader(int)));
        /*
        QPushButton * pb = new QPushButton(this);
        cap.sprintf("%d",i);
        pb->setText(cap);
        pb->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
        layout->addWidget(pb,i,12,1,2);
        connect(pb,SIGNAL(clicked()),this,SLOT(currentHeader(i)));
        */
    }

    //widgets[n]=new wNote(this);
    //((wNote)(widgets[0])).color.setHsl(n*10,180,127);
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
    if(i>0 && i<4) {
        for(int j=0;j<3;j++) {
            H[j]->hide();
        }
        layout->replaceWidget(header,H[i-1]);
        header=H[i-1];
        H[i-1]->show();
    }
}


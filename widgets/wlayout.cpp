#include "wlayout.h"
#include <QPushButton>
#include <QDebug>

wlayout::wlayout(QWidget *parent) : QWidget(parent),
    PlayArea(this)
{
    layout = new QGridLayout();
    int n=0;
    QString cap;

    ISender * out=new SenderMobileSynth();
    PlayArea.setOut(out);


    //qDebug() << "wlayout::wlayout new out " << out;

    /*
    for(int i=0;i<12;i++) {
        BaseNoteSetter[i] = new MWBaseNoteSetter(i);
        BaseNoteSetter[i]->setOut(out);
        connect(BaseNoteSetter[i],SIGNAL(setBaseNote(int)),&PlayArea,SLOT(setBaseNote(int)));
        layout->addWidget(BaseNoteSetter[i],0,i);
    }

    layout->addWidget(new QPushButton(this),0,12);
    layout->addWidget(new QPushButton(this),0,13);
    layout->addWidget(new QPushButton(this),0,14);
    */

    OctaveRanger = new MWOctaveRanger(this);
    connect(OctaveRanger,SIGNAL(setOctConf(int,int,int)),&PlayArea,SLOT(setOctConf(int,int,int)));
    layout->addWidget(OctaveRanger,0,0,1,14);

    for(int i=1;i<12;i++) {
        QPushButton * pb = new QPushButton(this);
        cap.sprintf("%d",i);
        pb->setText(cap);
        pb->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
        widgets[n]= pb;
        layout->addWidget(widgets[n],i,0,1,2);
        n++;
    }
    for(int i=1;i<12;i++) {
        QPushButton * pb = new QPushButton(this);
        cap.sprintf("%d",i);
        pb->setText(cap);
        pb->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
        widgets[n]= pb;
        layout->addWidget(widgets[n],i,12,1,2);
        n++;
    }
    //widgets[n]=new wNote(this);
    //((wNote)(widgets[0])).color.setHsl(n*10,180,127);
    layout->addWidget(&PlayArea,1,2,10,10);

    layout->setContentsMargins(0,0,0,0);
    layout->setMargin(0);
    layout->setHorizontalSpacing(0);
    layout->setVerticalSpacing(0);

    this->setLayout(layout);

}

wlayout::~wlayout()
{

}


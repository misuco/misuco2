#include "wlayout.h"
#include <QPushButton>
#include <QDebug>
#include <QStackedWidget>

wlayout::wlayout(QWidget *parent) : QWidget(parent)
{
    layout = new QGridLayout();
    QString cap;
    for(int i=0;i<128;i++) {
        note[i]=new FreqTriple();
        note[i]->setMidinote(i);
    }

    ISender * out=new SenderMobileSynth();
    out->cc(0,0,105,1,1);
    PlayArea = new MWPlayArea(this);
    PlayArea->setOut(out);

    QStackedWidget* stackedWidget = new QStackedWidget;
    QWidget* parentLayout1 = new QWidget;
    QWidget* parentLayout2 = new QWidget;
    parentLayout1->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    parentLayout2->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    QGridLayout* l1 = new QGridLayout(parentLayout1);
    OctaveRanger = new MWOctaveRanger(this);
    connect(OctaveRanger,SIGNAL(setOctConf(int,int,int)),PlayArea,SLOT(setOctConf(int,int,int)));
    l1->addWidget(OctaveRanger,0,0);

    QGridLayout* l2 = new QGridLayout(parentLayout2);
    for(int i=0;i<12;i++) {
        BaseNoteSetter[i] = new MWBaseNoteSetter(note[i+48]);
        BaseNoteSetter[i]->setOut(out);
        connect(BaseNoteSetter[i],SIGNAL(setBaseNote(int)),PlayArea,SLOT(setBaseNote(int)));
        l2->addWidget(BaseNoteSetter[i],0,i);
    }

    stackedWidget->addWidget(parentLayout1);
    stackedWidget->addWidget(parentLayout2);

    layout->addWidget(stackedWidget,0,0,1,-1);
    stackedWidget->widget(0)->show();
    stackedWidget->widget(1)->hide();


    //qDebug() << "wlayout::wlayout new out " << out;

    for(int i=1;i<12;i++) {
        QPushButton * pb = new QPushButton(this);
        cap.sprintf("%d",i);
        pb->setText(cap);
        pb->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
        layout->addWidget(pb,i,0,1,2);
    }

    for(int i=1;i<12;i++) {
        QPushButton * pb = new QPushButton(this);
        cap.sprintf("%d",i);
        pb->setText(cap);
        pb->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
        layout->addWidget(pb,i,12,1,2);
        if(1==i) {
            connect(pb,SIGNAL(clicked()),stackedWidget->widget(0),SLOT(show()));
            connect(pb,SIGNAL(clicked()),stackedWidget->widget(1),SLOT(hide()));
        }
        if(2==i) {
            connect(pb,SIGNAL(clicked()),stackedWidget->widget(0),SLOT(hide()));
            connect(pb,SIGNAL(clicked()),stackedWidget->widget(1),SLOT(show()));
        }
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

FreqTriple *wlayout::getNote(int i)
{
    return note[i];
}


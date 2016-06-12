#include "wlayout.h"
#include "widgets/wnote.h"
#include <QPushButton>
#include <widgets/mwplayarea.h>

wlayout::wlayout(QWidget *parent) : QWidget(parent)
{
    layout = new QGridLayout();
    int n=0;
    QString cap;
    for(int i=0;i<14;i++) {
        QPushButton * pb = new QPushButton(this);
        cap.sprintf("%d",i);
        pb->setText(cap);
        pb->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
        //pb->setSizeIncrement(1,1);
        widgets[n]= pb;
        //wNote(this);
        //((wNote)(widgets[n])).color.setHsl(n*10,180,127);
        layout->addWidget(widgets[n],0,i);
        n++;
    }
    for(int i=1;i<11;i++) {
        QPushButton * pb = new QPushButton(this);
        cap.sprintf("%d",i);
        pb->setText(cap);
        pb->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
        widgets[n]= pb;
        layout->addWidget(widgets[n],i,0,1,2);
        n++;
    }
    for(int i=1;i<11;i++) {
        QPushButton * pb = new QPushButton(this);
        cap.sprintf("%d",i);
        pb->setText(cap);
        pb->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
        widgets[n]= pb;
        layout->addWidget(widgets[n],i,12,1,2);
        n++;
    }
    //widgets[n]=new wNote(this);
    widgets[n]=new MWPlayArea(this);
    ((wNote)(widgets[n])).color.setHsl(n*10,180,127);
    layout->addWidget(widgets[n],1,2,10,10);

    layout->setContentsMargins(0,0,0,0);
    layout->setMargin(0);
    layout->setHorizontalSpacing(0);
    layout->setVerticalSpacing(0);

    this->setLayout(layout);

}

wlayout::~wlayout()
{

}


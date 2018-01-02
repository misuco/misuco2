#include "mwheadersetter.h"
#include <QPainter>

MWHeaderSetter::MWHeaderSetter(int headerId, QWidget *parent) : MisuWidget(parent)
{
    this->headerId=headerId;
    pressed=0;
    state=0;
}

void MWHeaderSetter::processTouchEvent(misuTouchEvent e)
{
    switch(e.state) {
    case Qt::TouchPointPressed:
        switch(headerId) {
        case 0:
        case 1:
        case 2:
            emit currentHeader(headerId);
            break;
        case 3:
            if(state==0) {
                state=1;
            } else {
                state=0;
            }
            emit setBendHori(state);
            break;
        case 4:
            if(state==0) {
                state=2;
            } else {
                state=0;
            }
            emit setBendVertTop(state);
            break;
        case 5:
            if(state==0) {
                state=-2;
            } else {
                state=0;
            }
            emit setBendVertBot(state);
            break;
        case 6:
            emit currentMainView(0);
            break;
        case 7:
            emit currentMainView(1);
            break;
        case 8:
            emit currentMainView(2);
            break;
        case 9:
            emit togglePresets();
            break;
        case 10:
            emit toggleMenu();
            break;
        }

        pressed++;
        update();
        break;
    case Qt::TouchPointReleased:
        pressed--;
        update();
        break;
    }
}

void MWHeaderSetter::paintEvent(QPaintEvent *E)
{
    QPainter painter(this);
    QString cap;
    painter.setFont(font1);
    painter.setPen(fgcolor);
    if(state!=0) painter.setBrush(highlightcolor);
    else painter.setBrush(bgcolor);
    painter.drawRect(0,0,width(),height());
    switch(headerId) {
    case 0:
        cap.sprintf("%d Octaves",headerId);
        break;
    case 1:
        cap.sprintf("%d Basenote",headerId);
        break;
    case 2:
        cap.sprintf("%d Scale",headerId);
        break;
    case 3:
        cap.sprintf("%d Bend horizontal",headerId);
        break;
    case 4:
        cap.sprintf("%d Bend top",headerId);
        break;
    case 5:
        cap.sprintf("%d Bend bottom",headerId);
        break;
    case 6:
        cap.sprintf("%d play area",headerId);
        break;
    case 7:
        cap.sprintf("%d pitch",headerId);
        break;
    case 8:
        cap.sprintf("%d synth ctl",headerId);
        break;
    case 9:
        cap.sprintf("%d presets",headerId);
        break;
    case 10:
        cap.sprintf("%d menu",headerId);
        break;

    }

    painter.drawText(0,0,width(),height(),Qt::AlignTop|Qt::AlignLeft,cap);
}

void MWHeaderSetter::resizeEvent(QResizeEvent *E)
{

}

#include "mwheadersetter.h"
#include <QPainter>
#include <QDesktopServices>
#include <QUrl>

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
        case 11:
            emit toggleBW();
            break;
        case 12:
            overwrite=!overwrite;
            state = overwrite;
            break;
        case 13:
            QString link ="http://scales.misuco.org/";
            link.append(midi2TextUrl(Scale.basenote));
            link.append("-");
            for(int i=0;i<11;i++) {
                if(Scale.bscale[i]) {
                    int currnote=Scale.basenote+i+1;
                    link.append(midi2TextUrl(currnote%12));
                    link.append("-");
                }
            }
            link.chop(1);
            QDesktopServices::openUrl(QUrl(link));
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

void MWHeaderSetter::paintEvent(QPaintEvent *)
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
    case 11:
        cap.sprintf("%d bwmode",headerId);
        break;
    case 12:
        cap.sprintf("overwrite");
        break;
    case 13:
        cap.sprintf("archive");
        break;
    }

    painter.drawText(0,0,width(),height(),Qt::AlignTop|Qt::AlignLeft,cap);
}

void MWHeaderSetter::resizeEvent(QResizeEvent *)
{

}

QString MWHeaderSetter::midi2TextUrl(int midinote) {
    QString t;
    switch(midinote) {
    case 0:
        t="c";
        break;
    case 1:
        t="cs";
        break;
    case 2:
        t="d";
        break;
    case 3:
        t="ds";
        break;
    case 4:
        t="e";
        break;
    case 5:
        t="f";
        break;
    case 6:
        t="fs";
        break;
    case 7:
        t="g";
        break;
    case 8:
        t="gs";
        break;
    case 9:
        t="a";
        break;
    case 10:
        t="as";
        break;
    case 11:
        t="b";
        break;
    default:
        t="";
        break;
    }
    return t;
}

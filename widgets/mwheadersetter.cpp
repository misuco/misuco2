#include "mwheadersetter.h"
#include <QPainter>
#include <QDesktopServices>
#include <QUrl>

MWHeaderSetter::MWHeaderSetter(int headerId, QWidget *parent) : MisuWidget(parent)
{
    this->headerId=headerId;
    pressed=0;
    this->state=0;
}

MWHeaderSetter::MWHeaderSetter(int headerId, int state, QWidget *parent): MisuWidget(parent)
{
    this->headerId=headerId;
    pressed=0;
    this->state=state;
}

void MWHeaderSetter::processTouchEvent(misuTouchEvent e)
{
    QString link;
    switch(e.state) {
    case Qt::TouchPointPressed:
        switch(headerId) {
        case 0:
        case 1:
        case 2:
            emit currentHeader(headerId);
            break;
        case 3:
        case 15:
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
            emit currentMainView(3);
            break;
            //emit togglePresets();
            //break;
        case 10:
            emit toggleMenu();
            break;
        case 11:
            if(state==0) {
                state=1;
            } else {
                state=0;
            }
            emit toggleBW();
            break;
        case 12:
            overwrite=!overwrite;
            state = overwrite;
            break;
        case 13:
            link ="http://scales.misuco.org/";
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
        case 14:
            noteSymbols++;
            if(noteSymbols>3) noteSymbols=0;
            emit scaleUpdate();
            break;
        case 16:
            MisuWidget::sendCC1 = ! MisuWidget::sendCC1;
            state = MisuWidget::sendCC1;
            break;
        case 17:
        case 18:
        case 19:
        case 20:
            emit(toggleSender(headerId-17));
            if(state==0) state = 1;
            else state = 0;
            break;
        case 21:
            holdMode=!holdMode;
            state = holdMode;
            break;
        case 22:
            showFreqs=!showFreqs;
            state = showFreqs;
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
    QFont font(font1);
    font.setPixelSize(font1size);
    painter.setFont(font);
    painter.setPen(fgcolor);
    if(state!=0) painter.setBrush(highlightcolor);
    else painter.setBrush(bgcolor);
    painter.drawRect(0,0,width(),height());
    switch(headerId) {
    case 0:
        cap.sprintf("octaves");
        break;
    case 1:
        cap.sprintf("basenote");
        break;
    case 2:
        cap.sprintf("scale");
        break;
    case 3:
        cap.sprintf("bend horizontal");
        break;
    case 4:
        cap.sprintf("bend top");
        break;
    case 5:
        cap.sprintf("bend bottom");
        break;
    case 6:
        cap.sprintf("play area");
        break;
    case 7:
        cap.sprintf("microtune");
        break;
    case 8:
        cap.sprintf("synth");
        break;
    case 9:
        cap.sprintf("setup");
        break;
        //cap.sprintf("%d presets",headerId);
        //break;
    case 10:
        cap.sprintf("menu");
        break;
    case 11:
        cap.sprintf("bwmode");
        break;
    case 12:
        cap.sprintf("overwrite");
        break;
    case 13:
        cap.sprintf("archive");
        break;
    case 14:
        cap.sprintf("symbols");
        break;
    case 15:
        cap.sprintf("bend\nhorizontal");
        break;
    case 16:
        cap.sprintf("send cc1");
        break;
    case 17:
        cap.sprintf("mobile\nsynth");
        break;
    case 18:
        cap.sprintf("puredata");
        break;
    case 19:
        cap.sprintf("reaktor");
        break;
    case 20:
        cap.sprintf("super\ncollider");
        break;
    case 21:
        cap.sprintf("hold");
        break;
    case 22:
        cap.sprintf("freqs");
        emit toggleShowFreqs();
        break;
    }

    if(width()>height()) {
        painter.drawText(0,0,width(),height(),Qt::AlignTop|Qt::AlignLeft,cap);
    } else {
        painter.rotate(90);
        painter.drawText(0,-width(),height(),width(),Qt::AlignTop|Qt::AlignLeft,cap);
        painter.rotate(-90);
    }
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

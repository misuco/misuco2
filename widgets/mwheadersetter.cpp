/*
 * Copyright (c) 2018 by misuco.org, Claudio Zopfi, Zurich, Switzerland, c@misuco.org
 *
 * This file is part of MISUCO2.
 *
 * MISUCO2 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * MISUCO2 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with MISUCO2.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "mwheadersetter.h"
#include <QPainter>
#include <QDesktopServices>
#include <QUrl>
#include <QDebug>

MWHeaderSetter::MWHeaderSetter(int headerId, QObject *parent) : MisuWidget(parent)
{
    this->_functionId=headerId;
    _pressed=0;
    this->_state=0;
    setText();
}

MWHeaderSetter::MWHeaderSetter(int headerId, int state, QObject *parent): MisuWidget(parent)
{
    this->_functionId=headerId;
    _pressed=0;
    this->_state=state;
    setText();
}

void MWHeaderSetter::setText() {

    _bgColor = bgcolor;
    _fontColor = fgcolor;

    switch(_functionId) {
    case 0:
        _text="root";
        break;
    case 1:
        _text="scale";
        break;
    case 2:
        _text="octave";
        break;
    case 3:
        _text="bend horizontal";
        break;
    case 4:
        _text="bend top";
        break;
    case 5:
        _text="bend bottom";
        break;
    case 6:
        _text="play";
        break;
    case 7:
        _text="tune";
        break;
    case 8:
        _text="synth";
        break;
    case 9:
        _text="setup";
        break;
        //_text="%d presets",headerId;
        //break;
    case 10:
        _text="menu";
        break;
    case 11:
        _text="bwmode";
        break;
    case 12:
        _text="overwrite";
        break;
    case 13:
        _text="archive";
        break;
    case 14:
        _text="symbols";
        break;
    case 15:
        _text="bend\nhorizontal";
        break;
    case 16:
        _text="send cc1";
        break;
    case 17:
        _text="mobile\nsynth";
        break;
    case 18:
        _text="puredata";
        break;
    case 19:
        _text="reaktor";
        break;
    case 20:
        _text="super\ncollider";
        break;
    case 21:
        _text="hold";
        break;
    case 22:
        _text="freqs";
        break;
    }

}

int MWHeaderSetter::getState()
{
    return _state;
}

void MWHeaderSetter::onPressed(int id)
{
    QString link;
    if(_pressed == 0) {
        switch(_functionId) {
        case 0:
        case 1:
        case 2:
            if(_state==0) {
                _state=1;
            } else {
                _state=0;
            }
            emit currentHeader(_functionId);
            break;
        case 3:
        case 15:
            if(_state==0) {
                _state=1;
            } else {
                _state=0;
            }
            emit setBendHori(_state);
            break;
        case 4:
            if(_state==0) {
                _state=2;
            } else {
                _state=0;
            }
            emit setBendVertTop(_state);
            break;
        case 5:
            if(_state==0) {
                _state=-2;
            } else {
                _state=0;
            }
            emit setBendVertBot(_state);
            break;
        case 6:
        case 7:
        case 8:
        case 9:
            if(_state==0) {
                _state=1;
            } else {
                _state=0;
            }
            emit currentMainView(_functionId-6);
            break;
        case 10:
            emit toggleMenu();
            break;
        case 11:
            bwmode=!bwmode;
            _state=bwmode;
            emit toggleBW();
            break;
        case 12:
            overwrite=!overwrite;
            _state = overwrite;
            break;
        case 13:
            link ="http://scales.misuco.org/";
            link.append(midi2TextUrl(Scale.rootNote));
            link.append("-");
            for(int i=0;i<11;i++) {
                if(Scale.bscale[i]) {
                    int currnote=Scale.rootNote+i+1;
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
            emit scaleupdate();
            break;
        case 16:
            MisuWidget::sendCC1 = ! MisuWidget::sendCC1;
            _state = MisuWidget::sendCC1;
            break;
        case 17:
        case 18:
        case 19:
        case 20:
            emit(toggleSender(_functionId-17));
            if(_state==0) _state = 1;
            else _state = 0;
            break;
        case 21:
            holdMode=!holdMode;
            _state = holdMode;
            break;
        case 22:
            showFreqs=!showFreqs;
            _state = showFreqs;
            break;
        }

        _pressed++;
        _pressedTouchId = id;
        _bgColor = highlightcolor;
        emit stateChanged();
    }
}

void MWHeaderSetter::onReleased(int id)
{
    if( id == _pressedTouchId) {
        _pressed--;
        if(_state>0) {
            _bgColor = highlightcolor;
        } else {
            _bgColor = bgcolor;
        }
        emit stateChanged();
    }
}

void MWHeaderSetter::setState(int id, int s)
{
    if(_functionId == id) {
        //qDebug() << "MWHeaderSetter::setState id:" << id << " state: " << s;
        _state = s;
        if(_state>0) {
            _bgColor = highlightcolor;
        } else {
            _bgColor = bgcolor;
        }
        emit stateChanged();
    }
}

/*
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
*/

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

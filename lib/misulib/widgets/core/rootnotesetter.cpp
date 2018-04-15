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

#include "rootnotesetter.h"
#include <QDebug>

RootNoteSetter::RootNoteSetter(int rootNote, MasterSender *ms, QObject *parent) : QObject(parent),
    _out(ms),
    _rootNote(rootNote),
    _noteSymbols(0),
    _showFreqs(false)
{
    //qDebug() << "MWrootNoteSetter::MWrootNoteSetter " << pitch->pitch << " pitch basenote " << pitch->basenote;
    _freq=new FreqTriple(rootNote,this);
    _vId=0;
    _pressed=0;
    _selected=false;
    calcText();
}

RootNoteSetter::~RootNoteSetter()
{
    _freq->deleteLater();
}

void RootNoteSetter::setOctMid(int o)
{
    _freq->setOct(o);
    calcText();
}

void RootNoteSetter::onPitchChange(int rootNote, int pitch)
{
    //qDebug() << "MWrootNoteSetter::pitchChange "  << f->getPitch();
    if(rootNote == _rootNote) {
        _freq->onPitchChange(rootNote,pitch);
        calcText();
    }
}

void RootNoteSetter::onSetRootNote(int rootNote)
{
    if(rootNote == _rootNote) {
        if(!_selected) {
            _selected = true;
            emit selectedChanged();
        }
    } else {
        if(_selected) {
            _selected = false;
            emit selectedChanged();
        }
    }
}

void RootNoteSetter::onSetScale(int rootNote, QList<bool>)
{
    onSetRootNote(rootNote);
}

void RootNoteSetter::onSymbolsChange(int noteSymbols)
{
    _noteSymbols = noteSymbols;
    calcText();
}

void RootNoteSetter::onShowFreqsChange(bool showFreqs)
{
    _showFreqs = showFreqs;
}

void RootNoteSetter::calcText()
{
    _text1 = _freq->getRootNoteString(_noteSymbols);

    if(_showFreqs) {
        _text2.sprintf("%5.1f",_freq->getFreq());
    } else {
        _text2="";
    }

    emit textChanged();
}

void RootNoteSetter::onPressed()
{
    if(_out && _pressed == 0) _vId=_out->noteOn(_freq->getFreq(),_freq->getMidinote(),_freq->getPitch(),127);
    _pressed++;
}

void RootNoteSetter::onReleased()
{
    if(_out && _pressed == 1) {
        _out->noteOff(_vId);
        emit setRootNote(_rootNote);
        emit selectedChanged();
    }
    _pressed--;
}


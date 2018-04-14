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

#include <QDebug>
#include "misuco2.h"
#include "senderoscmidigeneric.h"
#include "sendersupercollider.h"
#include "senderreaktor.h"
#include "senderdebug.h"
#include "sendermobilesynth.h"
#include "pitchcolor.h"

Misuco2::Misuco2(QObject *parent) : QObject(parent),
    _xmlLoader(new XmlLoader(this,this))
{
    _presetsVisible=true;
    _menuVisible=false;
    _rootNoteSetterVisible=true;
    _ScaleSwitchVisible=false;
    _octaveRangerVisible=false;
    _playAreaVisible=true;
    _tuneAreaVisible=false;
    _scalePresetsVisible=false;
    _synthPresetsVisible=false;
    _tunePresetsVisible=false;

    _scalePresets = new PresetCollection(this);
    _synthPresets = new PresetCollection(this);
    _tunePresets = new PresetCollection(this);

    _botOct=6;
    _topOct=7;    

    _heartbeat = new Heartbeat(this);

    _out=new MasterSender();
    _out->addSender(new SenderMobileSynth());
    _out->addSender(new SenderOscMidiGeneric());
    _out->addSender(new SenderReaktor());
    _out->addSender(new SenderSuperCollider());

    _out->onToggleSender(1,false);
    _out->onToggleSender(3,false);

    for(int rootNote=0;rootNote<SCALE_SIZE+1;rootNote++) {
        _pitchColors.append(new PitchColor(rootNote,this));
    }    

    _PlayArea = new MWPlayArea(_out, this);

    _OctaveRanger = new MWOctaveRanger(this);
    connect(_OctaveRanger,SIGNAL(setOctConf(int,int)),_PlayArea,SLOT(setOctConf(int,int)));
    connect(_OctaveRanger,SIGNAL(setOctConf(int,int)),this,SLOT(setOctConf(int,int)));

    for(int rootNote=0;rootNote<SCALE_SIZE+1;rootNote++) {
        MWFaderPitch * fader = new MWFaderPitch(rootNote,_out,this);
        connect(fader,SIGNAL(pitchChange(int,int)),_PlayArea,SLOT(onPitchChange(int,int)));
        for(auto rootNoteSetter:_rootNoteSetter) {
            connect(fader,SIGNAL(pitchChange(int,int)),rootNoteSetter,SLOT(onPitchChange(int,int)));
        }
        for(auto rootNoteSetter:_rootNoteSetter) {
            connect(fader,SIGNAL(pitchChange(int,int)),rootNoteSetter,SLOT(onPitchChange(int,int)));
        }
        for(auto scaleSwitch:_scaleSwitch) {
            connect(fader,SIGNAL(pitchChange(int,int)),scaleSwitch,SLOT(onPitchChange(int,int)));
        }
        for(auto pitchColor:_pitchColors) {
            connect(fader,SIGNAL(pitchChange(int,int)),pitchColor,SLOT(onPitchChange(int,int)));
        }
        connect(_OctaveRanger,SIGNAL(setOctMid(int)),fader,SLOT(setOctMid(int)));
        _faderMicrotune.append(fader);
    }

    for(int i=0;i<10;i++) {
        MWFaderParamCtl * fader = new MWFaderParamCtl(i+102,_out,this);
        fader->setMinValue(0);
        if(i==0) {
            fader->setMaxValue(4);
        } else {
            fader->setMaxValue(1000);
        }
        fader->setInverted(true);

        connect(fader,SIGNAL(controlValueChange(int)),this,SLOT(onSoundChanged(int)));

        _faderParamCtl.append(fader);

    }

    faderPitchTopRange = new MWFaderParamCtl(1,_out,this);
    faderPitchTopRange->setMinValue(-5);
    faderPitchTopRange->setMaxValue(5);
    connect(faderPitchTopRange,SIGNAL(controlValueChange(int)),_PlayArea,SLOT(setBendVertTop(int)));

    faderPitchBottomRange = new MWFaderParamCtl(2,_out,this);
    faderPitchBottomRange->setMinValue(-5);
    faderPitchBottomRange->setMaxValue(5);
    connect(faderPitchBottomRange,SIGNAL(controlValueChange(int)),_PlayArea,SLOT(setBendVertBot(int)));

    pitchHorizontal = new BendHorizontal("horizontal",3,this);
    connect(pitchHorizontal,SIGNAL(setBendHori(bool)),_PlayArea,SLOT(setBendHori(bool)));
    connect(this,SIGNAL(setBendHori(bool)),_PlayArea,SLOT(setBendHori(bool)));

    faderChannel = new MWFaderParamCtl(3,_out,this);
    faderChannel->setMinValue(1);
    faderChannel->setMaxValue(16);
    faderChannel->setInverted(true);
    connect(faderChannel,SIGNAL(controlValueChange(int)),this,SLOT(onChannelChange(int)));

    _sendCc1 = new SendCc1("CC1",0,this);
    connect(_sendCc1,SIGNAL(sendCc1(bool)),_PlayArea,SLOT(onSendCc1(bool)));

    _bwMode = new ToggleBw("BW",0,this);
    connect(_bwMode,SIGNAL(toggleBW(bool)),_PlayArea,SLOT(onBwModeChange(bool)));
    for(auto pitchColor:_pitchColors) {
        connect(_bwMode,SIGNAL(toggleBW(bool)),pitchColor,SLOT(onBwModeChange(bool)));
    }

    _enableMobilesynth = new ToggleSender("mobile\nsynth",0,1,this);
    connect(_enableMobilesynth,SIGNAL(toggleSender(int,bool)),_out,SLOT(onToggleSender(int,bool)));

    _enablePuredata = new ToggleSender("puredata",1,0,this);
    connect(_enablePuredata,SIGNAL(toggleSender(int,bool)),_out,SLOT(onToggleSender(int,bool)));

    _enableReaktor = new ToggleSender("reaktor",2,1,this);
    connect(_enableReaktor,SIGNAL(toggleSender(int,bool)),_out,SLOT(onToggleSender(int,bool)));

    _enableSupercollider = new ToggleSender("super\ncollider",3,0,this);
    connect(_enableSupercollider,SIGNAL(toggleSender(int,bool)),_out,SLOT(onToggleSender(int,bool)));

    showPresets = new MWHeaderSetter(12,this);
    connect(showPresets,SIGNAL(togglePresets()),this,SLOT(togglePresets()));

    showMenu = new MWHeaderSetter(10,this);
    connect(showMenu,SIGNAL(toggleMenu()),this,SLOT(toggleMenu()));

    octUp = new MWHeaderSetter(23,this);
    connect(octUp,SIGNAL(octUp()),_OctaveRanger,SLOT(octUp()));

    octDown = new MWHeaderSetter(24,this);
    connect(octDown,SIGNAL(octDown()),_OctaveRanger,SLOT(octDown()));

    _openArchive = new OpenArchive("archive",0,this);

    for(int rootNote=0;rootNote<SCALE_SIZE+1;rootNote++) {
        MWRootNoteSetter * rootNoteSetter = new MWRootNoteSetter(rootNote,_out,this);
        connect(rootNoteSetter,SIGNAL(setRootNote(int)),_PlayArea,SLOT(onSetRootNote(int)));
        connect(rootNoteSetter,SIGNAL(setRootNote(int)),_heartbeat,SLOT(onSetRootNote(int)));
        connect(rootNoteSetter,SIGNAL(setRootNote(int)),_openArchive,SLOT(onSetRootNote(int)));
        connect(_OctaveRanger,SIGNAL(setOctMid(int)),rootNoteSetter,SLOT(setOctMid(int)));
        _rootNoteSetter.append(rootNoteSetter);
    }

    for(auto rootNoteSetter:_rootNoteSetter) {
        for(auto rootNoteSetter2:_rootNoteSetter) {
            connect(rootNoteSetter,SIGNAL(setRootNote(int)),rootNoteSetter2,SLOT(onSetRootNote(int)));
        }
    }

    for(int i=1;i<SCALE_SIZE+1;i++) {
        ScaleSwitch * bs = new ScaleSwitch(i,_out,this);
        connect(bs,SIGNAL(setScale(int,bool)),_PlayArea,SLOT(setScale(int,bool)));
        connect(bs,SIGNAL(setScale(int,bool)),_heartbeat,SLOT(onSetScale(int,bool)));
        connect(bs,SIGNAL(setScale(int,bool)),_openArchive,SLOT(onSetScale(int,bool)));
        connect(_OctaveRanger,SIGNAL(setOctMid(int)),bs,SLOT(setOctMid(int)));
        for(auto rootNoteSetter:_rootNoteSetter) {
            connect(rootNoteSetter,SIGNAL(setRootNote(int)),bs,SLOT(onSetRootNote(int)));
        }
        _scaleSwitch.append(bs);
    }

    faderSymbols = new MWFaderParamCtl(4,_out,this);
    faderSymbols->setMinValue(0);
    faderSymbols->setMaxValue(4);
    faderSymbols->setInverted(true);
    connect(faderSymbols,SIGNAL(controlValueChange(int)),_PlayArea,SLOT(onSymbolsChange(int)));
    for(auto rootNoteSetter:_rootNoteSetter) {
        connect(faderSymbols,SIGNAL(controlValueChange(int)),rootNoteSetter,SLOT(onSymbolsChange(int)));
    }
    for(auto scaleSwitch:_scaleSwitch) {
        connect(faderSymbols,SIGNAL(controlValueChange(int)),scaleSwitch,SLOT(onSymbolsChange(int)));
    }

    for(int i=0;i<7;i++) {
        int fctId=i;
        if(i>=3) fctId+=3;

        MWHeaderSetter * hs = new MWHeaderSetter(fctId,this);
        // initially lit menu buttons
        if(i==0) hs->setState(0,1);
        if(i==3) hs->setState(6,1);

        connect(this,SIGNAL(setMenuItemState(int,int)),hs,SLOT(setState(int,int)));
        if(i<3) {
            connect(hs,SIGNAL(currentHeader(int)),this,SLOT(currentHeader(int)));
        } else if(i==3) {
            connect(hs,SIGNAL(currentMainView(int)),this,SLOT(currentMainView(int)));
        } else if(i==4) {
            connect(hs,SIGNAL(currentMainView(int)),this,SLOT(currentMainView(int)));
        } else if(i==5) {
            connect(hs,SIGNAL(currentMainView(int)),this,SLOT(currentMainView(int)));
        } else if(i==6) {
            connect(hs,SIGNAL(currentMainView(int)),this,SLOT(currentMainView(int)));
        }
        _menu.append(hs);
    }

    _showFreqs = new ShowFreqs("freq",0,this);
    connect(_showFreqs,SIGNAL(toggleShowFreqs(bool)),_PlayArea,SLOT(onShowFreqsChange(bool)));
    for(auto rootNoteSetter:_rootNoteSetter) {
        connect(_showFreqs,SIGNAL(toggleShowFreqs(bool)),rootNoteSetter,SLOT(onShowFreqsChange(bool)));
    }
    for(auto scaleSwitch:_scaleSwitch) {
        connect(_showFreqs,SIGNAL(toggleShowFreqs(bool)),scaleSwitch,SLOT(onShowFreqsChange(bool)));
    }

    _xmlLoader->readXml("conf.xml");
    _xmlLoader->readXml("scales.xml");
    _xmlLoader->readXml("synth.xml");
    _xmlLoader->readXml("tune.xml");

    for(auto presetButton:_scalePresets->getItems()) {
        connect(presetButton,SIGNAL(setScale(int,QList<bool>)),(MWPlayArea *)_PlayArea,SLOT(onSetScale(int,QList<bool>)));
        connect(presetButton,SIGNAL(setScale(int,QList<bool>)),_heartbeat,SLOT(onSetScale(int,QList<bool>)));
        connect(presetButton,SIGNAL(setScale(int,QList<bool>)),_rootNoteSetter[0],SLOT(onSetScale(int,QList<bool>)));
        for(int j=1;j<SCALE_SIZE+1;j++) {
            connect(presetButton,SIGNAL(setScale(int,QList<bool>)),_rootNoteSetter[j],SLOT(onSetScale(int,QList<bool>)));
            connect(presetButton,SIGNAL(setScale(int,QList<bool>)),_scaleSwitch[j-1],SLOT(onSetScale(int,QList<bool>)));
        }
        for(auto rootNoteSetter:_rootNoteSetter) {
            connect(rootNoteSetter,SIGNAL(setRootNote(int)),presetButton,SLOT(onSetRootNote(int)));
        }
        for(auto scaleSwitch:_scaleSwitch) {
            connect(scaleSwitch,SIGNAL(setScale(int,bool)),presetButton,SLOT(onSetScale(int,bool)));
        }
        for(auto presetButton2:_scalePresets->getItems()) {
            connect(presetButton2,SIGNAL(setScale(int,QList<bool>)),presetButton,SLOT(onSetScale(int,QList<bool>)));
        }
    }

    if(_scalePresets->getItemCount()>0) {
        connect(this,SIGNAL(initialSet()),_scalePresets->getItem(0),SLOT(initialSet()));
    }
    if(_synthPresets->getItemCount()>0) {
        connect(this,SIGNAL(initialSet()),_synthPresets->getItem(0),SLOT(initialSet()));
    }
    if(_tunePresets->getItemCount()>0) {
        connect(this,SIGNAL(initialSet()),_tunePresets->getItem(0),SLOT(initialSet()));
    }
    emit initialSet();

    _game = new MWGame((MWPlayArea *)_PlayArea,this);
    _out->addSender(_game);
    connect(_game,SIGNAL(gameStarted()),this,SLOT(onGameStarted()));
    _game->start();
}

Misuco2::~Misuco2()
{
    _xmlLoader->writeXml("conf.xml");
    _xmlLoader->writeXml("scales.xml");
    _xmlLoader->writeXml("synth.xml");
    _xmlLoader->writeXml("tune.xml");
}

QList<QObject *> Misuco2::confPitchFaders()
{
    QList<QObject*> p;
    p.append(faderPitchTopRange);
    p.append(faderPitchBottomRange);
    return p;
}

void Misuco2::updateMenuButtonState() {

    MWHeaderSetter * playAreaButton = dynamic_cast<MWHeaderSetter *>(_menu[3]);
    MWHeaderSetter * tuneAreaButton = dynamic_cast<MWHeaderSetter *>(_menu[4]);
    MWHeaderSetter * synthAreaButton = dynamic_cast<MWHeaderSetter *>(_menu[5]);
    MWHeaderSetter * confAreaButton = dynamic_cast<MWHeaderSetter *>(_menu[6]);
    MWHeaderSetter * rootButton = dynamic_cast<MWHeaderSetter *>(_menu[0]);
    MWHeaderSetter * scaleButton = dynamic_cast<MWHeaderSetter *>(_menu[1]);
    MWHeaderSetter * octaveButton = dynamic_cast<MWHeaderSetter *>(_menu[2]);

    if(playAreaButton) playAreaButton->setState(6,_playAreaVisible);
    if(tuneAreaButton) tuneAreaButton->setState(7,_tuneAreaVisible);
    if(synthAreaButton) synthAreaButton->setState(8,_synthAreaVisible);
    if(confAreaButton) confAreaButton->setState(9,_confAreaVisible);
    if(rootButton) rootButton->setState(0,_rootNoteSetterVisible);
    if(scaleButton) scaleButton->setState(1,_ScaleSwitchVisible);
    if(octaveButton) octaveButton->setState(2,_octaveRangerVisible);

}

void Misuco2::currentHeader(int id)
{
    switch(id) {
    case 0:
        _rootNoteSetterVisible=!_rootNoteSetterVisible;
        break;
    case 1:
        _ScaleSwitchVisible=!_ScaleSwitchVisible;
        break;
    case 2:
        _octaveRangerVisible=!_octaveRangerVisible;
        break;
    }

    emit layoutChange();
    updateMenuButtonState();
    _xmlLoader->writeXml("conf.xml");

}

void Misuco2::currentMainView(int id)
{    
    if(id==0 && _playAreaVisible) {
        _game->start();
    }

    _synthPresetsVisible=false;
    _scalePresetsVisible=false;
    _tunePresetsVisible=false;
    _playAreaVisible=false;

    switch(id) {
    case 0:
        _synthAreaVisible=false;
        _tuneAreaVisible=false;
        _confAreaVisible=false;
        break;
    case 1:
        _tuneAreaVisible=!_tuneAreaVisible;
        if(_presetsVisible) _tunePresetsVisible=_tuneAreaVisible;
        if(_tuneAreaVisible) {
            _synthAreaVisible=false;
            _confAreaVisible=false;
        }
        break;
    case 2:
        _synthAreaVisible=!_synthAreaVisible;
        _synthPresetsVisible=true;
        if(_synthAreaVisible) {
            _tuneAreaVisible=false;
            _confAreaVisible=false;
        }
        break;
    case 3:
        _confAreaVisible=!_confAreaVisible;
        if(_confAreaVisible) {
            _synthAreaVisible=false;
            _tuneAreaVisible=false;
        }
        break;
    }

    // play area is automatically visible, if no other is visible
    if(!_tuneAreaVisible && !_synthAreaVisible && !_confAreaVisible) {
        _playAreaVisible=true;
        _synthPresetsVisible=false;
        if(_presetsVisible) _scalePresetsVisible=true;
    }

    emit layoutChange();
    updateMenuButtonState();
    _xmlLoader->writeXml("conf.xml");
}

void Misuco2::togglePresets()
{    
    _presetsVisible=!_presetsVisible;
    if(_presetsVisible) {
        if(_playAreaVisible) {
            _scalePresetsVisible = true;
        }
        if(_synthAreaVisible) {
            _synthPresetsVisible = true;
        }
        if(_tuneAreaVisible) {
            _tunePresetsVisible = true;
        }
    } else {
        _scalePresetsVisible = false;
        _synthPresetsVisible = false;
        _tunePresetsVisible = false;
    }
    emit layoutChange();
    _xmlLoader->writeXml("conf.xml");
}

void Misuco2::toggleMenu()
{
    _menuVisible=!_menuVisible;
    emit layoutChange();
    _xmlLoader->writeXml("conf.xml");
}

void Misuco2::setSound(MWSound *s)
{
    int i=0;
    for(auto o:_faderParamCtl) {
        auto p = qobject_cast<MWFaderParamCtl*>(o);
        if(p) {
            switch(i) {
            case 0:
                p->setValue(s->wave_type);
                break;
            case 1:
                p->setValue(s->attack);
                break;
            case 2:
                p->setValue(s->decay);
                break;
            case 3:
                p->setValue(s->sustain);
                break;
            case 4:
                p->setValue(s->release);
                break;
            case 5:
                p->setValue(s->filter_cutoff);
                break;
            case 6:
                p->setValue(s->filter_resonance);
                break;
            case 7:
                p->setValue(s->mod_filter_cutoff);
                break;
            case 8:
                p->setValue(s->mod_filter_resonance);
                break;
            case 9:
                p->setValue(s->volume);
                break;
            }
        }
        i++;
    }

    emit soundChanged();
}

void Misuco2::setMicrotune(MWMicrotune * m)
{
    for(int rootNote=0;rootNote<12;rootNote++) {
        MGlob::Microtune.tuning[rootNote] = m->tuning[rootNote];
        //emit setPitch(rootNote,m->tuning[rootNote]);
        auto p = qobject_cast<MWFaderPitch*>(_faderMicrotune[rootNote]);
        if(p) p->setValue(m->tuning[rootNote]);
    }
}

void Misuco2::onChannelChange(int v)
{
    MGlob::channel = v;
}

/*
void Misuco2::onToggleSender(int v)
{
    if(out->isSenderEnabled(v)) out->setSenderEnabled(v,false);
    else out->setSenderEnabled(v,true);
    _xmlLoader->writeXml("conf.xml");
}
*/

void Misuco2::onSoundChanged(int)
{
    emit soundChanged();
}

void Misuco2::onGameStarted()
{
    _presetsVisible = false;
    _scalePresetsVisible=false;
    _synthPresetsVisible=false;
    _tunePresetsVisible=false;
    _menuVisible=false;
    _rootNoteSetterVisible=false;
    _ScaleSwitchVisible=false;
    _octaveRangerVisible=false;
    _synthAreaVisible=false;
    _confAreaVisible=false;
    _playAreaVisible=true;
    updateMenuButtonState();
    emit layoutChange();
}

void Misuco2::setOctConf(int bot, int top)
{
    _botOct = bot;
    _topOct = top;
    emit octConfChanged();
}

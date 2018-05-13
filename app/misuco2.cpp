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
#include "lib/misulib/comm/sendersupercollider.h"
#include "lib/misulib/comm/senderdebug.h"
#include "lib/misulib/comm/sendermobilesynth.h"
#include "lib/misulib/models/pitchcolor.h"

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
    _senderOscMidiGeneric = new SenderOscMidiGeneric();
    _out->addSender(_senderOscMidiGeneric);
    _senderReaktor = new SenderReaktor();
    _out->addSender(_senderReaktor);
    _out->addSender(new SenderSuperCollider());

    _out->onToggleSender(1,false);
    _out->onToggleSender(3,false);

    for(int rootNote=0;rootNote<SCALE_SIZE+1;rootNote++) {
        _pitchColors.append(new PitchColor(rootNote,this));
    }    

    _PlayArea = new PlayArea(_out, nullptr, this);

    _OctaveRanger = new OctaveRanger(this);
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
    connect(_enableMobilesynth,&ToggleSender::toggleSender,_out,&MasterSender::onToggleSender);

    _enablePuredata = new ToggleSender("puredata",1,0,this);
    connect(_enablePuredata,&ToggleSender::toggleSender,_out,&MasterSender::onToggleSender);

    _enableReaktor = new ToggleSender("reaktor",2,1,this);
    connect(_enableReaktor,&ToggleSender::toggleSender,_out,&MasterSender::onToggleSender);

    _enableSupercollider = new ToggleSender("super\ncollider",3,0,this);
    connect(_enableSupercollider,&ToggleSender::toggleSender,_out,&MasterSender::onToggleSender);

    _togglePresets = new TogglePresets("memo",0,this);
    connect(_togglePresets,SIGNAL(togglePresets(bool)),this,SLOT(togglePresets(bool)));

    _toggleMenu = new ToggleMenu("menu",0,this);
    connect(_toggleMenu,SIGNAL(toggleMenu(bool)),this,SLOT(toggleMenu(bool)));

    _octUp = new OctaveShift("up",true,this);
    connect(_octUp,SIGNAL(octUp()),_OctaveRanger,SLOT(octUp()));

    _octDown = new OctaveShift("down",false,this);
    connect(_octDown,SIGNAL(octDown()),_OctaveRanger,SLOT(octDown()));

    _openArchive = new OpenArchive("archive",0,this);

    for(int rootNote=0;rootNote<SCALE_SIZE+1;rootNote++) {
        RootNoteSetter * rootNoteSetter = new RootNoteSetter(rootNote,_out,this);
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

    _rootButton = new ToggleHeader("root",0,this);
    connect(_rootButton,SIGNAL(toggleHeader(int)),this,SLOT(toggleHeader(int)));
    _menu.append(_rootButton);
    _scaleButton = new ToggleHeader("scale",1,this);
    connect(_scaleButton,SIGNAL(toggleHeader(int)),this,SLOT(toggleHeader(int)));
    _menu.append(_scaleButton);
    _octaveButton = new ToggleHeader("octave",2,this);
    connect(_octaveButton,SIGNAL(toggleHeader(int)),this,SLOT(toggleHeader(int)));
    _menu.append(_octaveButton);

    _playAreaButton = new SetMainView("play",0,this);
    connect(_playAreaButton,SIGNAL(setMainView(int)),this,SLOT(currentMainView(int)));
    _menu.append(_playAreaButton);
    _tuneAreaButton = new SetMainView("tune",1,this);
    connect(_tuneAreaButton,SIGNAL(setMainView(int)),this,SLOT(currentMainView(int)));
    _menu.append(_tuneAreaButton);
    _synthAreaButton = new SetMainView("synth",2,this);
    connect(_synthAreaButton,SIGNAL(setMainView(int)),this,SLOT(currentMainView(int)));
    _menu.append(_synthAreaButton);
    _confAreaButton = new SetMainView("conf",3,this);
    connect(_confAreaButton,SIGNAL(setMainView(int)),this,SLOT(currentMainView(int)));
    _menu.append(_confAreaButton);

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
        connect(presetButton,SIGNAL(setScale(int,QList<bool>)),(PlayArea *)_PlayArea,SLOT(onSetScale(int,QList<bool>)));
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

    _game = new GameControl((PlayArea *)_PlayArea,this);
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

void Misuco2::updateMenuButtonState() {
    _playAreaButton->setState(_playAreaVisible);
    _tuneAreaButton->setState(_tuneAreaVisible);
    _synthAreaButton->setState(_synthAreaVisible);
    _confAreaButton->setState(_confAreaVisible);
    _rootButton->setState(_rootNoteSetterVisible);
    _scaleButton->setState(_ScaleSwitchVisible);
    _octaveButton->setState(_octaveRangerVisible);
}

void Misuco2::toggleHeader(int id)
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

void Misuco2::togglePresets(bool state)
{    
    _presetsVisible = state;
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

void Misuco2::toggleMenu(bool state)
{
    _menuVisible=state;
    emit layoutChange();
    _xmlLoader->writeXml("conf.xml");
}

void Misuco2::setSound(MWSound *s)
{
    for(auto o:_faderParamCtl) {
        auto p = qobject_cast<MWFaderParamCtl*>(o);
        if(p) {
            switch(p->getCc()) {
            case 102:
                p->setValue(s->wave_type);
                break;
            case 103:
                p->setValue(s->attack);
                break;
            case 104:
                p->setValue(s->decay);
                break;
            case 105:
                p->setValue(s->sustain);
                break;
            case 106:
                p->setValue(s->release);
                break;
            case 107:
                p->setValue(s->filter_cutoff);
                break;
            case 108:
                p->setValue(s->filter_resonance);
                break;
            case 109:
                p->setValue(s->mod_filter_cutoff);
                break;
            case 110:
                p->setValue(s->mod_filter_resonance);
                break;
            case 111:
                p->setValue(s->volume);
                break;
            }
        }
    }
    emit soundChanged();
}

void Misuco2::setMicrotune(MWMicrotune * m)
{
    for(int rootNote=0;rootNote<12;rootNote++) {
        auto p = qobject_cast<MWFaderPitch*>(_faderMicrotune[rootNote]);
        if(p) p->setValue(m->tuning[rootNote]);
    }
}

void Misuco2::onChannelChange(int v)
{
    _channel = v;
    _senderReaktor->setChannel(v);
    _senderOscMidiGeneric->setChannel(v);
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

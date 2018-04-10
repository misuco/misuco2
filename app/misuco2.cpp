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

#include "misuco2.h"
#include <QDebug>
#include <QStandardPaths>
#include <QDir>
#include "senderoscmidigeneric.h"
#include "sendersupercollider.h"
#include "senderreaktor.h"
#include "senderdebug.h"
#include "sendermobilesynth.h"

Misuco2::Misuco2(QObject *parent) : QObject(parent)
{
    _presetsVisible=true;
    _menuVisible=false;
    _rootNoteSetterVisible=true;
    _bScaleSwitchVisible=false;
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

    qDebug() << "QSysInfo::productType " << QSysInfo::productType() << " homedir: " << QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    if(QSysInfo::productType() == "ios") {
        _configPath=QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
        //configPath=QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
        //configPath=QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
        //configPath=QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
        //configPath=QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
        //configPath=QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
        //configPath=QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        //_configPath=QStandardPaths::writableLocation(QStandardPaths::DataLocation);
        //_configPath=QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    //} else if(QSysInfo::productType() == "ubuntu") {
    } else if(QSysInfo::productType() == "osx") {
       _configPath=QStandardPaths::writableLocation(QStandardPaths::HomeLocation)+"/.misuco2";
       QDir dir(_configPath);
       if (!dir.exists()) {
           dir.mkpath(".");
       }
    }

    out=new MasterSender();
    out->addSender(new SenderMobileSynth());
    out->addSender(new SenderOscMidiGeneric());
    out->addSender(new SenderReaktor());
    out->addSender(new SenderSuperCollider());

    out->setSenderEnabled(1,false);
    out->setSenderEnabled(3,false);

    for(int i=0;i<BSCALE_SIZE+1;i++) {
        MGlob::MWPitch[i]=new Pitch(i,this);
    }    

    _PlayArea = new MWPlayArea(out, this);

    _OctaveRanger = new MWOctaveRanger(this);
    connect(_OctaveRanger,SIGNAL(setOctConf(int,int)),_PlayArea,SLOT(setOctConf(int,int)));
    connect(_OctaveRanger,SIGNAL(setOctConf(int,int)),this,SLOT(setOctConf(int,int)));

    for(int i=0;i<BSCALE_SIZE+1;i++) {
        MWFaderPitch * fader = new MWFaderPitch(MGlob::MWPitch[i],out,this);
        connect (fader,SIGNAL(valueChange(int)),MGlob::MWPitch[i],SLOT(setPitch(int)));
        connect( MGlob::MWPitch[i], SIGNAL(pitchChanged()), fader, SLOT(pitchChange()));
        connect(_OctaveRanger,SIGNAL(setOctMid(int)),fader,SLOT(setOctMid(int)));
        _faderMicrotune.append(fader);
    }

    for(int i=0;i<10;i++) {
        MWFaderParamCtl * fader = new MWFaderParamCtl(i+102,out,this);
        fader->setMinValue(0);
        if(i==0) {
            fader->setMaxValue(4);
        } else {
            fader->setMaxValue(1000);
        }
        fader->setInverted(true);

        connect(fader,SIGNAL(valueChange(int)),this,SLOT(onSoundChanged(int)));

        _faderParamCtl.append(fader);

    }

    faderPitchTopRange = new MWFaderParamCtl(1,out,this);
    faderPitchTopRange->setMinValue(-5);
    faderPitchTopRange->setMaxValue(5);
    connect(faderPitchTopRange,SIGNAL(valueChange(int)),_PlayArea,SLOT(setBendVertTop(int)));

    faderPitchBottomRange = new MWFaderParamCtl(2,out,this);
    faderPitchBottomRange->setMinValue(-5);
    faderPitchBottomRange->setMaxValue(5);
    connect(faderPitchBottomRange,SIGNAL(valueChange(int)),_PlayArea,SLOT(setBendVertBot(int)));

    pitchHorizontal = new MWHeaderSetter(15,this);
    connect(pitchHorizontal,SIGNAL(setBendHori(bool)),_PlayArea,SLOT(setBendHori(bool)));
    connect(this,SIGNAL(setBendHori(bool)),_PlayArea,SLOT(setBendHori(bool)));

    faderChannel = new MWFaderParamCtl(3,out,this);
    faderChannel->setMinValue(1);
    faderChannel->setMaxValue(16);
    faderChannel->setInverted(true);
    connect(faderChannel,SIGNAL(valueChange(int)),this,SLOT(onChannelChange(int)));

    enableCc1 = new MWHeaderSetter(16,1,this);
    MGlob::sendCC1 = true;

    bwMode = new MWHeaderSetter(11,this);
    connect(bwMode,SIGNAL(toggleBW()),_PlayArea,SLOT(onToggleBW()));
    for(int i=0;i<BSCALE_SIZE+1;i++) {
        connect(bwMode,SIGNAL(toggleBW()),MGlob::MWPitch[i],SLOT(bwModeChanged()));
    }

    enableMobilesynth = new MWHeaderSetter(17,1,this);
    connect(enableMobilesynth,SIGNAL(toggleSender(int)),this,SLOT(onToggleSender(int)));

    enablePuredata = new MWHeaderSetter(18,this);
    connect(enablePuredata,SIGNAL(toggleSender(int)),this,SLOT(onToggleSender(int)));

    enableReaktor = new MWHeaderSetter(19,1,this);
    connect(enableReaktor,SIGNAL(toggleSender(int)),this,SLOT(onToggleSender(int)));

    enableSupercollider = new MWHeaderSetter(20,this);
    connect(enableSupercollider,SIGNAL(toggleSender(int)),this,SLOT(onToggleSender(int)));

    showPresets = new MWHeaderSetter(12,this);
    connect(showPresets,SIGNAL(togglePresets()),this,SLOT(togglePresets()));

    showMenu = new MWHeaderSetter(10,this);
    connect(showMenu,SIGNAL(toggleMenu()),this,SLOT(toggleMenu()));

    octUp = new MWHeaderSetter(23,this);
    connect(octUp,SIGNAL(octUp()),_OctaveRanger,SLOT(octUp()));

    octDown = new MWHeaderSetter(24,this);
    connect(octDown,SIGNAL(octDown()),_OctaveRanger,SLOT(octDown()));

    holdMode = new MWHeaderSetter(21,this);

    for(int i=0;i<BSCALE_SIZE+1;i++) {
        MWRootNoteSetter * rootNoteSetter = new MWRootNoteSetter(MGlob::MWPitch[i],out,this);
        connect(rootNoteSetter,SIGNAL(setRootNote(Pitch *)),_PlayArea,SLOT(setRootNote(Pitch *)));
        connect(rootNoteSetter,SIGNAL(setRootNote(Pitch *)),_heartbeat,SLOT(onSetRootNote(Pitch *)));
        connect(rootNoteSetter,SIGNAL(setRootNote(Pitch *)),this,SLOT(onSetRootNote(Pitch *)));
        connect(this,SIGNAL(setRootNote(Pitch*)),rootNoteSetter,SLOT(onSetRootNote(Pitch*)));
        connect(this,SIGNAL(symbolsChanged()),rootNoteSetter,SLOT(onSymbolsChanged()));
        connect(_OctaveRanger,SIGNAL(setOctMid(int)),rootNoteSetter,SLOT(setOctMid(int)));
        connect(MGlob::MWPitch[i], SIGNAL(pitchChanged()) ,rootNoteSetter, SLOT(pitchChange()));
        _rootNoteSetter.append(rootNoteSetter);
    }

    for(int i=1;i<BSCALE_SIZE+1;i++) {
        MWBScaleSwitch * bs = new MWBScaleSwitch(i,out,this);
        connect(bs,SIGNAL(setBscale(int,bool)),_PlayArea,SLOT(setBscale(int,bool)));
        connect(bs,SIGNAL(setBscale(int,bool)),_heartbeat,SLOT(onSetBscale(int,bool)));
        connect(_OctaveRanger,SIGNAL(setOctMid(int)),bs,SLOT(setOctMid(int)));
        connect(this,SIGNAL(symbolsChanged()),bs,SLOT(onSymbolsChanged()));
        for(int j=0;j<12;j++) {
            connect(_rootNoteSetter[j],SIGNAL(setRootNote(Pitch *)),bs,SLOT(setRootNote(Pitch *)));
        }
        _BScaleSwitch.append(bs);
    }

    openArchive = new MWHeaderSetter(13,1,this);

    faderSymbols = new MWFaderParamCtl(4,out,this);
    faderSymbols->setMinValue(0);
    faderSymbols->setMaxValue(4);
    faderSymbols->setInverted(true);
    connect(faderSymbols,SIGNAL(valueChange(int)),this,SLOT(onSymbolsChange(int)));
    connect(this,SIGNAL(symbolsChanged()),_PlayArea,SLOT(onSymbolsChanged()));

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

    showFreqs = new MWHeaderSetter(22,this);
    connect(showFreqs,SIGNAL(toggleShowFreqs()),this,SLOT(onShowFreqsChange()));

    readXml("conf.xml");
    readXml("scales.xml");
    readXml("synth.xml");
    readXml("tune.xml");

    for(auto presetButton:_scalePresets->getItems()) {
        connect(presetButton,SIGNAL(setScale(MWScale*)),(MWPlayArea *)_PlayArea,SLOT(setScale(MWScale*)));
        connect(presetButton,SIGNAL(setScale(MWScale*)),_rootNoteSetter[0],SLOT(onScaleSet(MWScale*)));
        connect(presetButton,SIGNAL(setScale(MWScale*)),_heartbeat,SLOT(onScaleSet(MWScale*)));
        for(int j=1;j<12;j++) {
            connect(presetButton,SIGNAL(setScale(MWScale*)),_rootNoteSetter[j],SLOT(onScaleSet(MWScale*)));
            connect(presetButton,SIGNAL(setScale(MWScale*)),_BScaleSwitch[j-1],SLOT(onScaleSet(MWScale*)));
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
    out->addSender(_game);
    connect(_game,SIGNAL(gameStarted()),this,SLOT(onGameStarted()));
    _game->start();
}

Misuco2::~Misuco2()
{
    writeXml("conf.xml");
    writeXml("scales.xml");
    writeXml("synth.xml");
    writeXml("tune.xml");
}

QList<QObject *> Misuco2::pitches()
{
    QList<QObject*> p;
    for(int i=0;i<BSCALE_SIZE+1;i++) {
        p.append(MGlob::MWPitch[i]);
    }
    return p;
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
    MWHeaderSetter * bscaleButton = dynamic_cast<MWHeaderSetter *>(_menu[1]);
    MWHeaderSetter * octaveButton = dynamic_cast<MWHeaderSetter *>(_menu[2]);

    if(playAreaButton) playAreaButton->setState(6,_playAreaVisible);
    if(tuneAreaButton) tuneAreaButton->setState(7,_tuneAreaVisible);
    if(synthAreaButton) synthAreaButton->setState(8,_synthAreaVisible);
    if(confAreaButton) confAreaButton->setState(9,_confAreaVisible);
    if(rootButton) rootButton->setState(0,_rootNoteSetterVisible);
    if(bscaleButton) bscaleButton->setState(1,_bScaleSwitchVisible);
    if(octaveButton) octaveButton->setState(2,_octaveRangerVisible);

}

void Misuco2::currentHeader(int id)
{
    switch(id) {
    case 0:
        _rootNoteSetterVisible=!_rootNoteSetterVisible;
        break;
    case 1:
        _bScaleSwitchVisible=!_bScaleSwitchVisible;
        break;
    case 2:
        _octaveRangerVisible=!_octaveRangerVisible;
        break;
    }

    emit layoutChange();
    updateMenuButtonState();
    writeXml("conf.xml");

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
    writeXml("conf.xml");
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
    writeXml("conf.xml");
}

void Misuco2::toggleMenu()
{
    _menuVisible=!_menuVisible;
    emit layoutChange();
    writeXml("conf.xml");
}

void Misuco2::onSetRootNote(Pitch *p)
{
    emit setRootNote(p);
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
    for(int i=0;i<12;i++) {
        MGlob::Microtune.tuning[i] = m->tuning[i];
        MGlob::MWPitch[i]->setPitch(m->tuning[i]);
        auto p = qobject_cast<MWFaderPitch*>(_faderMicrotune[i]);
        if(p) p->setValue(m->tuning[i]);
    }
}

void Misuco2::onChannelChange(int v)
{
    MGlob::channel = v;
}

void Misuco2::onToggleSender(int v)
{
    if(out->isSenderEnabled(v)) out->setSenderEnabled(v,false);
    else out->setSenderEnabled(v,true);
    writeXml("conf.xml");
}

void Misuco2::onSymbolsChange(int v)
{
    MGlob::noteSymbols = v;
    emit symbolsChanged();
    writeXml("conf.xml");
}

void Misuco2::onShowFreqsChange()
{
    emit symbolsChanged();
    writeXml("conf.xml");
}

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
    _bScaleSwitchVisible=false;
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

void Misuco2::readXml(QString filetype)
{
    QString filename = _configPath+"/"+filetype;
    QFile file(filename);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QFile factoryFile(":/xml/"+filetype);
        factoryFile.copy(filename);
        if (!file.open(QFile::ReadOnly | QFile::Text)) {
            //qDebug() << " cannot reopen " << filename;
            return;
        }
        file.setPermissions(QFileDevice::ReadOwner|QFileDevice::WriteOwner);
    }
    //qDebug() << " opened " << filename;
    xmlr.setDevice(&file);
    if (xmlr.readNextStartElement()) {
        //qDebug() << " xmlr.name() " << xmlr.name();
        if (xmlr.name() == "misuco" ) {
            while (xmlr.readNextStartElement()) {
                if(filetype=="conf.xml") {
                    decodeConfigRecord();
                    updateMenuButtonState();
                } else if(filetype=="scales.xml") {
                    decodeScaleRecord();
                } else if(filetype=="synth.xml") {
                    decodeSynthRecord();
                } else if(filetype=="tune.xml") {
                    decodeTuneRecord();
                }
                xmlr.skipCurrentElement();
            }
        } else {
            return;
        }
    }
    file.close();
}

void Misuco2::decodeConfigRecord() {
    if (xmlr.name() == "setup") {
        _presetsVisible=xmlr.attributes().value("presetsVisible").toInt();
        _menuVisible=xmlr.attributes().value("menuVisible").toInt();

        _rootNoteSetterVisible=xmlr.attributes().value("rootNoteSetterVisible").toInt();
        _bScaleSwitchVisible=xmlr.attributes().value("bScaleSwitchVisible").toInt();
        _octaveRangerVisible=xmlr.attributes().value("octaveRangerVisible").toInt();

        _playAreaVisible=xmlr.attributes().value("playAreaVisible").toInt();
        _tuneAreaVisible=xmlr.attributes().value("tuneAreaVisible").toInt();
        _synthAreaVisible=xmlr.attributes().value("synthAreaVisible").toInt();
        _confAreaVisible=xmlr.attributes().value("confAreaVisible").toInt();

        _scalePresetsVisible=xmlr.attributes().value("scalePresetsVisible").toInt();
        _synthPresetsVisible=xmlr.attributes().value("synthPresetsVisible").toInt();
        _tunePresetsVisible=xmlr.attributes().value("tunePresetsVisible").toInt();

        faderPitchTopRange->setValue(xmlr.attributes().value("pitchTopRange").toString().toInt());
        faderPitchBottomRange->setValue(xmlr.attributes().value("pitchBottomRange").toString().toInt());

        pitchHorizontal->setState(15,xmlr.attributes().value("pitchHorizontal").toString().toInt());
        emit setBendHori(xmlr.attributes().value("pitchHorizontal").toString().toInt());

        faderChannel->setValue(xmlr.attributes().value("channel").toString().toInt());

        MGlob::sendCC1 = xmlr.attributes().value("pitchHorizontal").toString().toInt();
        enableCc1->setState(16,xmlr.attributes().value("sendCC1").toString().toInt());

        MGlob::bwmode = xmlr.attributes().value("bwmode").toString().toInt();
        bwMode->setState(11,xmlr.attributes().value("bwmode").toString().toInt());

        out->setSenderEnabled(0,xmlr.attributes().value("mobileSynth").toString().toInt());
        enableMobilesynth->setState(17,xmlr.attributes().value("mobileSynth").toString().toInt());

        out->setSenderEnabled(1,xmlr.attributes().value("pureData").toString().toInt());
        enablePuredata->setState(18,xmlr.attributes().value("pureData").toString().toInt());

        out->setSenderEnabled(2,xmlr.attributes().value("reaktor").toString().toInt());
        enableReaktor->setState(19,xmlr.attributes().value("reaktor").toString().toInt());

        out->setSenderEnabled(3,xmlr.attributes().value("superCollider").toString().toInt());
        enableSupercollider->setState(20,xmlr.attributes().value("superCollider").toString().toInt());

        faderSymbols->setValue(xmlr.attributes().value("noteSymbols").toString().toInt());

        MGlob::showFreqs = xmlr.attributes().value("showFreqs").toString().toInt();
        showFreqs->setState(22,xmlr.attributes().value("showFreqs").toString().toInt());
    }
}

void Misuco2::decodeScaleRecord() {
    if (xmlr.name() == "scale") {
        bool bscaleRead[BSCALE_SIZE];
        for(int i=0;i<BSCALE_SIZE;i++) {
            QString attId;
            attId.sprintf("b%d",i);
            bscaleRead[i] = xmlr.attributes().value(attId).toInt();
        }
        MWScalePreset * p = new MWScalePreset(xmlr.attributes().value("rootNote").toString().toInt(),
                                         bscaleRead,
                                         this);
        connect(((MWPlayArea *)_PlayArea),SIGNAL(playRowsChanged()),p,SLOT(playAreaChanged()));
        connect(p,SIGNAL(editPreset()),_scalePresets,SLOT(onEditPreset()));
        _scalePresets->append(p);
    }
}

void Misuco2::decodeSynthRecord() {
    if (xmlr.name() == "sound") {
        MWSoundPreset * soundPreset = new MWSoundPreset(
                    xmlr.attributes().value("volume").toString().toFloat(),
                    xmlr.attributes().value("wave").toString().toInt(),
                    xmlr.attributes().value("attack").toString().toInt(),
                    xmlr.attributes().value("decay").toString().toInt(),
                    xmlr.attributes().value("sustain").toString().toFloat(),
                    xmlr.attributes().value("release").toString().toInt(),
                    xmlr.attributes().value("cutoff").toString().toFloat(),
                    xmlr.attributes().value("resonance").toString().toFloat(),
                    xmlr.attributes().value("mod_cutoff").toString().toFloat(),
                    xmlr.attributes().value("mod_resonance").toString().toFloat(),
                    this);
        connect(soundPreset,SIGNAL(setSound(MWSound*)),this,SLOT(setSound(MWSound*)));
        connect(soundPreset,SIGNAL(editPreset()),_scalePresets,SLOT(onEditPreset()));
        connect(this, SIGNAL(soundChanged()), soundPreset, SLOT(onSoundChanged()));
        _synthPresets->append(soundPreset);
    }
}

void Misuco2::decodeTuneRecord() {
    if (xmlr.name() == "microtune") {
        int microtune[12];
        for(int i=0;i<12;i++) {
            QString attId;
            attId.sprintf("t%d",i);
            microtune[i]=xmlr.attributes().value(attId).toString().toInt();
        }
        MWMicrotunePreset * microtunePreset = new MWMicrotunePreset(microtune,this);
        connect(microtunePreset,SIGNAL(setMicrotune(MWMicrotune*)),this,SLOT(setMicrotune(MWMicrotune*)));
        connect(microtunePreset,SIGNAL(editPreset()),_scalePresets,SLOT(onEditPreset()));
        _tunePresets->append(microtunePreset);
    }
}

void Misuco2::writeXml(QString filename)
{
    //qDebug() << "wlayout::writeXml";
    QFile file(_configPath+"/"+filename);
    if(file.open(QIODevice::WriteOnly)) {
        xml.setDevice(&file);
        QString att;
        QString attId;

        xml.writeStartDocument();
        xml.writeDTD("<!DOCTYPE misuco>");
        xml.writeStartElement("misuco");
        xml.writeAttribute("version", "2.1");

        if(filename == "scales.xml") {

            for(auto widgetQ:_scalePresets->getItems()) {

                auto widget = qobject_cast<MWScalePreset*>(widgetQ);
                if(widget) {
                    xml.writeStartElement("scale");
                    att.sprintf("%d",widget->PresetScale.rootNote);
                    xml.writeAttribute("rootNote",att);
                    att.sprintf("%d",widget->PresetScale.baseoct);
                    xml.writeAttribute("baseoct",att);
                    att.sprintf("%d",widget->PresetScale.topoct);
                    xml.writeAttribute("topoct",att);
                    for(int i=0;i<BSCALE_SIZE;i++) {
                        att.sprintf("%d",widget->PresetScale.bscale[i]);
                        attId.sprintf("b%d",i);
                        xml.writeAttribute(attId,att);
                    }
                    xml.writeEndElement();
                }
            }
        } else if(filename == "synth.xml") {
            for(auto o:_synthPresets->getItems()) {
                auto widget = qobject_cast<MWSoundPreset *>(o);
                if(widget) {
                    xml.writeStartElement("sound");
                    att.sprintf("%d",widget->wave());
                    xml.writeAttribute("wave",att);
                    att.sprintf("%d",widget->attack());
                    xml.writeAttribute("attack",att);
                    att.sprintf("%d",widget->decay());
                    xml.writeAttribute("decay",att);
                    att.sprintf("%f",widget->sustain());
                    xml.writeAttribute("sustain",att);
                    att.sprintf("%d",widget->release());
                    xml.writeAttribute("release",att);
                    att.sprintf("%f",widget->cutoff());
                    xml.writeAttribute("cutoff",att);
                    att.sprintf("%f",widget->resonance());
                    xml.writeAttribute("resonance",att);
                    att.sprintf("%f",widget->mod_cutoff());
                    xml.writeAttribute("mod_cutoff",att);
                    att.sprintf("%f",widget->mod_resonance());
                    xml.writeAttribute("mod_resonance",att);
                    att.sprintf("%f",widget->volume());
                    xml.writeAttribute("volume",att);
                    xml.writeEndElement();
                }
            }
        } else if(filename == "tune.xml") {

            for(auto o:_tunePresets->getItems()) {
                auto widget = qobject_cast<MWMicrotunePreset *>(o);
                xml.writeStartElement("microtune");
                for(int i=0;i<12;i++) {
                    att.sprintf("%d",widget->PresetMicrotune.tuning[i]);
                    attId.sprintf("t%d",i);
                    xml.writeAttribute(attId,att);
                }
                xml.writeEndElement();
            }
        } else if(filename == "conf.xml") {
            xml.writeStartElement("setup");

            att.sprintf("%d",faderPitchTopRange->getValue());
            xml.writeAttribute("pitchTopRange",att);
            att.sprintf("%d",faderPitchBottomRange->getValue());
            xml.writeAttribute("pitchBottomRange",att);
            att.sprintf("%d",pitchHorizontal->getState());
            xml.writeAttribute("pitchHorizontal",att);
            att.sprintf("%d",MGlob::channel);
            xml.writeAttribute("channel",att);
            att.sprintf("%d",MGlob::sendCC1);
            xml.writeAttribute("sendCC1",att);
            att.sprintf("%d",MGlob::bwmode);
            xml.writeAttribute("bwmode",att);
            att.sprintf("%d",out->isSenderEnabled(0));
            xml.writeAttribute("mobileSynth",att);
            att.sprintf("%d",out->isSenderEnabled(1));
            xml.writeAttribute("pureData",att);
            att.sprintf("%d",out->isSenderEnabled(2));
            xml.writeAttribute("reaktor",att);
            att.sprintf("%d",out->isSenderEnabled(3));
            xml.writeAttribute("superCollider",att);
            att.sprintf("%d",MGlob::noteSymbols);
            xml.writeAttribute("noteSymbols",att);
            att.sprintf("%d",MGlob::showFreqs);
            xml.writeAttribute("showFreqs",att);

            att.sprintf("%d",_presetsVisible);
            xml.writeAttribute("presetsVisible",att);
            att.sprintf("%d",_menuVisible);
            xml.writeAttribute("menuVisible",att);

            att.sprintf("%d",_rootNoteSetterVisible);
            xml.writeAttribute("rootNoteSetterVisible",att);
            att.sprintf("%d",_bScaleSwitchVisible);
            xml.writeAttribute("bScaleSwitchVisible",att);
            att.sprintf("%d",_octaveRangerVisible);
            xml.writeAttribute("octaveRangerVisible",att);

            att.sprintf("%d",_playAreaVisible);
            xml.writeAttribute("playAreaVisible",att);
            att.sprintf("%d",_synthAreaVisible);
            xml.writeAttribute("synthAreaVisible",att);
            att.sprintf("%d",_confAreaVisible);
            xml.writeAttribute("confAreaVisible",att);
            att.sprintf("%d",_tuneAreaVisible);
            xml.writeAttribute("tuneAreaVisible",att);

            att.sprintf("%d",_scalePresetsVisible);
            xml.writeAttribute("scalePresetsVisible",att);
            att.sprintf("%d",_synthPresetsVisible);
            xml.writeAttribute("synthPresetsVisible",att);
            att.sprintf("%d",_tunePresetsVisible);
            xml.writeAttribute("tunePresetsVisible",att);

            xml.writeEndElement();

        }

        xml.writeEndDocument();

        file.close();

    } else {
        qDebug() << "cannot write " << filename;
    }

}

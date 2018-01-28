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

#include "conf/initxml.h"
#include "wlayout.h"
#include <QPushButton>
#include <QDebug>
#include <QStackedWidget>
#include <QStandardPaths>

wlayout::wlayout(QWidget *parent) : QObject(parent)
{
    presetsVisible=false;
    headerVisible=false;
    _rootNoteSetterVisible=true;
    _bScaleSwitchVisible=false;
    _octaveRangerVisible=false;
    _playAreaVisible=true;
    _tuneAreaVisible=false;
    _scalePresetsVisible=true;
    _synthPresetsVisible=false;
    _tunePresetsVisible=false;
    _dialogPresetsVisible=false;

    //qDebug() << QSysInfo::productType();
    if(QSysInfo::productType() == "ios") {
        configPath=QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
        //configPath=QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
        //configPath=QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
        //configPath=QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
        //configPath=QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
        //configPath=QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
        //configPath=QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    } else if(QSysInfo::productType() == "ubuntu") {
        configPath=QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    } else {
        configPath=QStandardPaths::writableLocation(QStandardPaths::DataLocation);
    }
    configPath=configPath.append("/misuco2.xml");

    out=new SenderMulti();
    //out->cc(0,0,105,1,1);

    for(int i=0;i<BSCALE_SIZE+1;i++) {
        MisuWidget::MWPitch[i]=new Pitch(i,this);
    }    

    _PlayArea = new MWPlayArea(this);
    ((MWPlayArea *)_PlayArea)->setOut(out);

    /*
    connect(_PlayArea,SIGNAL(menuTouch()), this, SLOT(toggleMenu()));
    connect(_PlayArea,SIGNAL(presetsTouch()), this, SLOT(togglePresets()));

    for(int i=0;i<BSCALE_SIZE+1;i++) {
        connect( MisuWidget::MWPitch[i], SIGNAL(pitchChanged()), (MWPlayArea *)_PlayArea, SLOT(pitchChange()));
    }
    */

    _OctaveRanger = new MWOctaveRanger(this);
    connect(_OctaveRanger,SIGNAL(setOctConf(int,int)),_PlayArea,SLOT(setOctConf(int,int)));

    for(int i=0;i<BSCALE_SIZE+1;i++) {
        MWFaderPitch * fader = new MWFaderPitch(this,MisuWidget::MWPitch[i]);
        fader->setOut(out);
        connect (fader,SIGNAL(valueChange(int)),MisuWidget::MWPitch[i],SLOT(setPitch(int)));
        connect( MisuWidget::MWPitch[i], SIGNAL(pitchChanged()), fader, SLOT(pitchChange()));
        connect(_OctaveRanger,SIGNAL(setOctMid(int)),fader,SLOT(setOctMid(int)));
        _faderMicrotune.append(fader);
    }

    for(int i=0;i<10;i++) {
        MWFaderParamCtl * fader = new MWFaderParamCtl(this,i+102);
        fader->setOut(out);
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

    faderPitchTopRange = new MWFaderParamCtl(this,1);
    faderPitchTopRange->setOut(out);
    faderPitchTopRange->setMinValue(-5);
    faderPitchTopRange->setMaxValue(5);
    connect(faderPitchTopRange,SIGNAL(valueChange(int)),_PlayArea,SLOT(setBendVertTop(int)));

    faderPitchBottomRange = new MWFaderParamCtl(this,2);
    faderPitchBottomRange->setOut(out);
    faderPitchBottomRange->setMinValue(-5);
    faderPitchBottomRange->setMaxValue(5);
    connect(faderPitchBottomRange,SIGNAL(valueChange(int)),_PlayArea,SLOT(setBendVertBot(int)));

    pitchHorizontal = new MWHeaderSetter(15,this);
    connect(pitchHorizontal,SIGNAL(setBendHori(bool)),_PlayArea,SLOT(setBendHori(bool)));
    connect(this,SIGNAL(setBendHori(bool)),_PlayArea,SLOT(setBendHori(bool)));

    faderChannel = new MWFaderParamCtl(this,3);
    faderChannel->setOut(out);
    faderChannel->setMinValue(1);
    faderChannel->setMaxValue(16);
    faderChannel->setInverted(true);
    connect(faderChannel,SIGNAL(valueChange(int)),this,SLOT(onChannelChange(int)));

    enableCc1 = new MWHeaderSetter(16,1,this);
    MisuWidget::sendCC1 = true;

    bwMode = new MWHeaderSetter(11,this);
    connect(bwMode,SIGNAL(toggleBW()),_PlayArea,SLOT(onToggleBW()));
    for(int i=0;i<BSCALE_SIZE+1;i++) {
        connect(bwMode,SIGNAL(toggleBW()),MisuWidget::MWPitch[i],SLOT(bwModeChanged()));
    }

    enableMobilesynth = new MWHeaderSetter(17,1,this);
    connect(enableMobilesynth,SIGNAL(toggleSender(int)),this,SLOT(onToggleSender(int)));

    enablePuredata = new MWHeaderSetter(18,this);
    connect(enablePuredata,SIGNAL(toggleSender(int)),this,SLOT(onToggleSender(int)));

    enableReaktor = new MWHeaderSetter(19,1,this);
    connect(enableReaktor,SIGNAL(toggleSender(int)),this,SLOT(onToggleSender(int)));

    enableSupercollider = new MWHeaderSetter(20,this);
    connect(enableSupercollider,SIGNAL(toggleSender(int)),this,SLOT(onToggleSender(int)));

    holdMode = new MWHeaderSetter(21,this);

    for(int i=0;i<12;i++) {
        MWRootNoteSetter * rootNoteSetter = new MWRootNoteSetter(MisuWidget::MWPitch[i],this);
        rootNoteSetter->setOut(out);
        connect(rootNoteSetter,SIGNAL(setRootNote(Pitch *)),_PlayArea,SLOT(setRootNote(Pitch *)));
        connect(rootNoteSetter,SIGNAL(setRootNote(Pitch *)),this,SLOT(onSetRootNote(Pitch *)));
        connect(this,SIGNAL(setRootNote(Pitch*)),rootNoteSetter,SLOT(onSetRootNote(Pitch*)));
        connect(this,SIGNAL(symbolsChanged()),rootNoteSetter,SLOT(onSymbolsChanged()));
        connect(_OctaveRanger,SIGNAL(setOctMid(int)),rootNoteSetter,SLOT(setOctMid(int)));
        connect(MisuWidget::MWPitch[i], SIGNAL(pitchChanged()) ,rootNoteSetter, SLOT(pitchChange()));
        _rootNoteSetter.append(rootNoteSetter);
    }

    for(int i=1;i<12;i++) {
        MWBScaleSwitch * bs =new MWBScaleSwitch(i);
        bs->setOut(out);
        connect(bs,SIGNAL(setBscale(int,bool)),_PlayArea,SLOT(setBscale(int,bool)));
        connect(_OctaveRanger,SIGNAL(setOctMid(int)),bs,SLOT(setOctMid(int)));
        connect(this,SIGNAL(symbolsChanged()),bs,SLOT(onSymbolsChanged()));
        for(int j=0;j<12;j++) {
            connect(_rootNoteSetter[j],SIGNAL(setRootNote(Pitch *)),bs,SLOT(setRootNote(Pitch *)));
        }
        _BScaleSwitch.append(bs);
    }

    openScalesArchive = new MWHeaderSetter(13,1,this);

    faderSymbols = new MWFaderParamCtl(this,4);
    faderSymbols->setOut(out);
    faderSymbols->setMinValue(0);
    faderSymbols->setMaxValue(4);
    faderSymbols->setInverted(true);
    connect(faderSymbols,SIGNAL(valueChange(int)),this,SLOT(onSymbolsChange(int)));
    connect(this,SIGNAL(symbolsChanged()),_PlayArea,SLOT(onSymbolsChanged()));

    showFreqs = new MWHeaderSetter(22,this);
    connect(showFreqs,SIGNAL(toggleShowFreqs()),this,SLOT(onShowFreqsChange()));

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

    readXml(configPath);

    for(auto presetButton:_scalePresets) {
        connect(presetButton,SIGNAL(setScale(MWScale*)),(MWPlayArea *)_PlayArea,SLOT(setScale(MWScale*)));
        connect(presetButton,SIGNAL(setScale(MWScale*)),_rootNoteSetter[0],SLOT(onScaleSet(MWScale*)));
        for(int j=1;j<12;j++) {
            connect(presetButton,SIGNAL(setScale(MWScale*)),_rootNoteSetter[j],SLOT(onScaleSet(MWScale*)));
            connect(presetButton,SIGNAL(setScale(MWScale*)),_BScaleSwitch[j-1],SLOT(onScaleSet(MWScale*)));
        }
    }

    if(_scalePresets.size()>0) {
        connect(this,SIGNAL(initialSet()),_scalePresets[0],SLOT(initialSet()));
    }
    if(_synthPresets.size()>0) {
        connect(this,SIGNAL(initialSet()),_synthPresets[0],SLOT(initialSet()));
    }
    if(_tunePresets.size()>0) {
        connect(this,SIGNAL(initialSet()),_tunePresets[0],SLOT(initialSet()));
    }
    emit initialSet();

}

wlayout::~wlayout()
{
    writeXml(configPath);
}

QList<QObject *> wlayout::pitches()
{
    QList<QObject*> p;
    for(int i=0;i<BSCALE_SIZE+1;i++) {
        p.append(MisuWidget::MWPitch[i]);
    }
    return p;
}

QList<QObject *> wlayout::confPitchFaders()
{
    QList<QObject*> p;
    p.append(faderPitchTopRange);
    p.append(faderPitchBottomRange);
    return p;
}

void wlayout::closeDialogPreset()
{
    _dialogPresetsVisible=false;
    emit layoutChange();
}

void wlayout::overwritePreset()
{
    auto soundPreset = qobject_cast<MWSoundPreset *>(MisuWidget::overwritePreset);
    if(soundPreset) {
        soundPreset->overwrite();
        MisuWidget::overwritePreset = nullptr;
    }

    auto scalePreset = qobject_cast<MWPreset *>(MisuWidget::overwritePreset);
    if(scalePreset) {
        scalePreset->overwrite();
        MisuWidget::overwritePreset = nullptr;
    }

    _dialogPresetsVisible=false;
    emit layoutChange();
}

void wlayout::currentHeader(int id)
{
    MWHeaderSetter * rootButton = dynamic_cast<MWHeaderSetter *>(_menu[0]);
    MWHeaderSetter * bscaleButton = dynamic_cast<MWHeaderSetter *>(_menu[1]);
    MWHeaderSetter * octaveButton = dynamic_cast<MWHeaderSetter *>(_menu[2]);

    _rootNoteSetterVisible=false;
    _bScaleSwitchVisible=false;
    _octaveRangerVisible=false;

    if(rootButton) rootButton->setState(0,0);
    if(bscaleButton) bscaleButton->setState(1,0);
    if(octaveButton) octaveButton->setState(2,0);

    switch(id) {
    case 0:
        _rootNoteSetterVisible=true;
        if(rootButton) rootButton->setState(0,1);
        break;
    case 1:
        _bScaleSwitchVisible=true;
        if(bscaleButton) bscaleButton->setState(1,1);
        break;
    case 2:
        _octaveRangerVisible=true;
        if(octaveButton) octaveButton->setState(2,1);
        break;
    }

    emit layoutChange();

}

void wlayout::currentMainView(int id)
{

    MWHeaderSetter * playAreaButton = dynamic_cast<MWHeaderSetter *>(_menu[3]);
    MWHeaderSetter * tuneAreaButton = dynamic_cast<MWHeaderSetter *>(_menu[4]);
    MWHeaderSetter * synthAreaButton = dynamic_cast<MWHeaderSetter *>(_menu[5]);
    MWHeaderSetter * confAreaButton = dynamic_cast<MWHeaderSetter *>(_menu[6]);

    _synthPresetsVisible=false;
    _scalePresetsVisible=false;
    _tunePresetsVisible=false;
    _playAreaVisible=false;
    _tuneAreaVisible=false;
    _synthAreaVisible=false;
    _confAreaVisible=false;

    if(playAreaButton) playAreaButton->setState(6,0);
    if(tuneAreaButton) tuneAreaButton->setState(7,0);
    if(synthAreaButton) synthAreaButton->setState(8,0);
    if(confAreaButton) confAreaButton->setState(9,0);

    switch(id) {
    case 0:
        _playAreaVisible=true;
        _scalePresetsVisible=true;
        if(playAreaButton) playAreaButton->setState(6,1);
        break;
    case 1:
        _tuneAreaVisible=true;
        _tunePresetsVisible=true;
        if(tuneAreaButton) tuneAreaButton->setState(7,1);
        break;
    case 2:
        _synthAreaVisible=true;
        _synthPresetsVisible=true;
        if(synthAreaButton) synthAreaButton->setState(8,1);
        break;
    case 3:
        _confAreaVisible=true;
        if(confAreaButton) confAreaButton->setState(9,1);
        break;
    }
    emit layoutChange();
}

void wlayout::togglePresets()
{
    presetsVisible=!presetsVisible;
    //recalcMainView();
}

void wlayout::toggleMenu()
{
    headerVisible=!headerVisible;
    //recalcMainView();
}

void wlayout::onSetRootNote(Pitch *p)
{
    emit setRootNote(p);
}

void wlayout::setSound(MWSound *s)
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

void wlayout::setMicrotune(MWMicrotune * m)
{
    for(int i=0;i<12;i++) {
        MisuWidget::Microtune.tuning[i] = m->tuning[i];
        MisuWidget::MWPitch[i]->setPitch(m->tuning[i]);
        auto p = qobject_cast<MWFaderPitch*>(_faderMicrotune[i]);
        if(p) p->setValue(m->tuning[i]);
    }
}

void wlayout::onChannelChange(int v)
{
    MisuWidget::channel = v;
}

void wlayout::onToggleSender(int v)
{
    if(out->senderEnabled[v]) out->senderEnabled[v]=false;
    else out->senderEnabled[v]=true;
}

void wlayout::onSymbolsChange(int v)
{
    MisuWidget::noteSymbols = v;
    emit symbolsChanged();
}

void wlayout::onShowFreqsChange()
{
    emit symbolsChanged();
}

void wlayout::onSoundChanged(int)
{
    emit soundChanged();
}

void wlayout::onEditPreset()
{
    _dialogPresetsVisible = true;
    emit layoutChange();
}

void wlayout::readXml(QString filename)
{
    QFile file(filename);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        if (file.open(QFile::ReadWrite | QFile::Text)) {
            file.write(initXml);
            file.flush();
            file.close();
            if (!file.open(QFile::ReadOnly | QFile::Text)) {
                //qDebug() << " cannot reopen " << filename;
                return;
            }
        } else {
            //qDebug() << " cannot write " << filename;
            return;
        }
    }
    //qDebug() << " opened " << filename;
    xmlr.setDevice(&file);
    if (xmlr.readNextStartElement()) {
        //qDebug() << " xmlr.name() " << xmlr.name();
        if (xmlr.name() == "misuco" && xmlr.attributes().value("version") == "2.0")
            readLayout();
        else
            xmlr.raiseError(QObject::tr("The file is not a MISUCO version 1.0 file."));
    }
    file.close();
}

void wlayout::writeXml(QString filename)
{
    QFile file(filename);
    file.open(QIODevice::WriteOnly);
    xml.setDevice(&file);
    QString att;
    QString attId;

    xml.writeStartDocument();
    xml.writeDTD("<!DOCTYPE misuco>");
    xml.writeStartElement("misuco");
    xml.writeAttribute("version", "2.0");

    for(auto widgetQ:_scalePresets) {

        auto widget = qobject_cast<MWPreset*>(widgetQ);
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

    for(auto o:_synthPresets) {
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

    for(auto o:_tunePresets) {
        auto widget = qobject_cast<MWMicrotunePreset *>(o);
        xml.writeStartElement("microtune");
        for(int i=0;i<12;i++) {
            att.sprintf("%d",widget->PresetMicrotune.tuning[i]);
            attId.sprintf("t%d",i);
            xml.writeAttribute(attId,att);
        }
        xml.writeEndElement();
    }

    xml.writeStartElement("setup");

    att.sprintf("%d",faderPitchTopRange->getValue());
    xml.writeAttribute("pitchTopRange",att);
    att.sprintf("%d",faderPitchBottomRange->getValue());
    xml.writeAttribute("pitchBottomRange",att);
    att.sprintf("%d",pitchHorizontal->getState());
    xml.writeAttribute("pitchHorizontal",att);
    att.sprintf("%d",MisuWidget::channel);
    xml.writeAttribute("channel",att);
    att.sprintf("%d",MisuWidget::sendCC1);
    xml.writeAttribute("sendCC1",att);
    att.sprintf("%d",MisuWidget::bwmode);
    xml.writeAttribute("bwmode",att);
    att.sprintf("%d",out->senderEnabled[0]);
    xml.writeAttribute("mobileSynth",att);
    att.sprintf("%d",out->senderEnabled[1]);
    xml.writeAttribute("pureData",att);
    att.sprintf("%d",out->senderEnabled[2]);
    xml.writeAttribute("reaktor",att);
    att.sprintf("%d",out->senderEnabled[3]);
    xml.writeAttribute("superCollider",att);
    att.sprintf("%d",MisuWidget::holdMode);
    xml.writeAttribute("holdMode",att);
    att.sprintf("%d",MisuWidget::noteSymbols);
    xml.writeAttribute("noteSymbols",att);
    att.sprintf("%d",MisuWidget::showFreqs);
    xml.writeAttribute("showFreqs",att);

    for(int i=0;i<3;i++) {
        QString attId;
        attId.sprintf("showH%d",i);
        /*
        if(H[i]->isHidden()){
            att.sprintf("0");
        } else {
            att.sprintf("1");
        }
        */
        xml.writeAttribute(attId,att);
    }

    for(int i=0;i<4;i++) {
        QString attId;
        attId.sprintf("showM%d",i);
        /*
        if(M[i]->isHidden()){
            att.sprintf("0");
        } else {
            att.sprintf("1");
        }
        */
        xml.writeAttribute(attId,att);
    }

    xml.writeEndElement();

    xml.writeEndDocument();

    file.close();

}

void wlayout::readLayout() {

    while (xmlr.readNextStartElement()) {
        //qDebug() << "xmlr row name " << xmlr.name();
        if (xmlr.name() == "scale") {
            bool bscaleRead[BSCALE_SIZE];
            for(int i=0;i<BSCALE_SIZE;i++) {
                QString attId;
                attId.sprintf("b%d",i);
                bscaleRead[i] = xmlr.attributes().value(attId).toInt();
            }
            MWPreset * p = new MWPreset(xmlr.attributes().value("rootNote").toString().toInt(),
                                             xmlr.attributes().value("baseoct").toString().toInt(),
                                             xmlr.attributes().value("topoct").toString().toInt(),
                                             bscaleRead,
                                             this);
            connect(((MWPlayArea *)_PlayArea),SIGNAL(playRowsChanged()),p,SLOT(playAreaChanged()));
            connect(p,SIGNAL(editPreset()),this,SLOT(onEditPreset()));
            _scalePresets.append(p);
        } else if (xmlr.name() == "sound") {
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
            connect(soundPreset,SIGNAL(editPreset()),this,SLOT(onEditPreset()));
            connect(this, SIGNAL(soundChanged()), soundPreset, SLOT(onSoundChanged()));
            _synthPresets.append(soundPreset);
        } else if (xmlr.name() == "microtune") {
            int microtune[12];
            for(int i=0;i<12;i++) {
                QString attId;
                attId.sprintf("t%d",i);
                microtune[i]=xmlr.attributes().value(attId).toString().toInt();
            }
            MWMicrotunePreset * microtunePreset = new MWMicrotunePreset(microtune,this);
            connect(microtunePreset,SIGNAL(setMicrotune(MWMicrotune*)),this,SLOT(setMicrotune(MWMicrotune*)));
            connect(microtunePreset,SIGNAL(editPreset()),this,SLOT(onEditPreset()));
            _tunePresets.append(microtunePreset);
        } else if (xmlr.name() == "setup") {
            for(int i=0;i<3;i++) {
                QString attId;
                attId.sprintf("showH%d",i);
                /*
                if(xmlr.attributes().value(attId).toString()=="1"){
                    H[i]->show();
                    emit setMenuItemState(i,1);
                } else {
                    H[i]->hide();
                    emit setMenuItemState(i,0);
                }
                */
            }

            for(int i=0;i<4;i++) {
                QString attId;
                attId.sprintf("showM%d",i);
                /*
                if(xmlr.attributes().value(attId).toString()=="1"){
                    M[i]->show();
                    emit setMenuItemState(i+6,1);
                } else {
                    M[i]->hide();
                    emit setMenuItemState(i+6,0);
                }
                */
            }

            faderPitchTopRange->setValue(xmlr.attributes().value("pitchTopRange").toString().toInt());
            faderPitchBottomRange->setValue(xmlr.attributes().value("pitchBottomRange").toString().toInt());

            pitchHorizontal->setState(15,xmlr.attributes().value("pitchHorizontal").toString().toInt());
            emit setBendHori(xmlr.attributes().value("pitchHorizontal").toString().toInt());

            faderChannel->setValue(xmlr.attributes().value("channel").toString().toInt());

            MisuWidget::sendCC1 = xmlr.attributes().value("pitchHorizontal").toString().toInt();
            enableCc1->setState(16,xmlr.attributes().value("sendCC1").toString().toInt());

            MisuWidget::bwmode = xmlr.attributes().value("bwmode").toString().toInt();
            bwMode->setState(11,xmlr.attributes().value("bwmode").toString().toInt());

            out->senderEnabled[0] = xmlr.attributes().value("mobileSynth").toString().toInt();
            enableMobilesynth->setState(17,xmlr.attributes().value("mobileSynth").toString().toInt());

            out->senderEnabled[1] = xmlr.attributes().value("pureData").toString().toInt();
            enablePuredata->setState(18,xmlr.attributes().value("pureData").toString().toInt());

            out->senderEnabled[2] = xmlr.attributes().value("reaktor").toString().toInt();
            enableReaktor->setState(19,xmlr.attributes().value("reaktor").toString().toInt());

            out->senderEnabled[3] = xmlr.attributes().value("superCollider").toString().toInt();
            enableSupercollider->setState(20,xmlr.attributes().value("superCollider").toString().toInt());

            MisuWidget::holdMode = xmlr.attributes().value("holdMode").toString().toInt();
            holdMode->setState(21,xmlr.attributes().value("holdMode").toString().toInt());

            faderSymbols->setValue(xmlr.attributes().value("noteSymbols").toString().toInt());

            MisuWidget::showFreqs = xmlr.attributes().value("showFreqs").toString().toInt();
            showFreqs->setState(22,xmlr.attributes().value("showFreqs").toString().toInt());
        }
        xmlr.skipCurrentElement();
    }
}

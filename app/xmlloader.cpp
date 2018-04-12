#include "xmlloader.h"

XmlLoader::XmlLoader(Misuco2 *misuco2, QObject *parent) : QObject(parent),
    _app(misuco2)
{    
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
}


void XmlLoader::readXml(QString filetype)
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

void XmlLoader::decodeConfigRecord() {
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

        //MGlob::bwmode = xmlr.attributes().value("bwmode").toString().toInt();
        //bwMode->setState(11,xmlr.attributes().value("bwmode").toString().toInt());

        out->setSenderEnabled(0,xmlr.attributes().value("mobileSynth").toString().toInt());
        enableMobilesynth->setState(17,xmlr.attributes().value("mobileSynth").toString().toInt());

        out->setSenderEnabled(1,xmlr.attributes().value("pureData").toString().toInt());
        enablePuredata->setState(18,xmlr.attributes().value("pureData").toString().toInt());

        out->setSenderEnabled(2,xmlr.attributes().value("reaktor").toString().toInt());
        enableReaktor->setState(19,xmlr.attributes().value("reaktor").toString().toInt());

        out->setSenderEnabled(3,xmlr.attributes().value("superCollider").toString().toInt());
        enableSupercollider->setState(20,xmlr.attributes().value("superCollider").toString().toInt());

        faderSymbols->setValue(xmlr.attributes().value("noteSymbols").toString().toInt());

        //MGlob::showFreqs = xmlr.attributes().value("showFreqs").toString().toInt();
        //showFreqs->setState(22,xmlr.attributes().value("showFreqs").toString().toInt());
    }
}

void XmlLoader::decodeScaleRecord() {
    if (xmlr.name() == "scale") {
        QList<bool> bscaleRead;
        for(int i=0;i<BSCALE_SIZE;i++) {
            QString attId;
            attId.sprintf("b%d",i);
            bscaleRead.append(xmlr.attributes().value(attId).toInt());
        }
        MWScalePreset * p = new MWScalePreset(xmlr.attributes().value("rootNote").toString().toInt(),
                                         bscaleRead,
                                         this);
        connect(p,SIGNAL(editPreset()),_scalePresets,SLOT(onEditPreset()));
        _scalePresets->append(p);
    }
}

void XmlLoader::decodeSynthRecord() {
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

void XmlLoader::decodeTuneRecord() {
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

void XmlLoader::writeXml(QString filename)
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
                    att.sprintf("%d",widget->getRootNote());
                    xml.writeAttribute("rootNote",att);
                    for(int i=0;i<BSCALE_SIZE;i++) {
                        att.sprintf("%d",widget->getScale(i));
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
            //att.sprintf("%d",);
            //xml.writeAttribute("bwmode",att);
            att.sprintf("%d",out->isSenderEnabled(0));
            xml.writeAttribute("mobileSynth",att);
            att.sprintf("%d",out->isSenderEnabled(1));
            xml.writeAttribute("pureData",att);
            att.sprintf("%d",out->isSenderEnabled(2));
            xml.writeAttribute("reaktor",att);
            att.sprintf("%d",out->isSenderEnabled(3));
            xml.writeAttribute("superCollider",att);
            //att.sprintf("%d",MGlob::noteSymbols);
            //xml.writeAttribute("noteSymbols",att);
            //att.sprintf("%d",MGlob::showFreqs);
            //xml.writeAttribute("showFreqs",att);

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

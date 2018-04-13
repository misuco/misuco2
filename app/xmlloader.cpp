#include "xmlloader.h"
#include <QStandardPaths>
#include <QDir>

XmlLoader::XmlLoader(Misuco2 *misuco2, QObject *parent) : QObject(parent),
    _app(misuco2)
{    
    //qDebug() << "QSysInfo::productType " << QSysInfo::productType() << " homedir: " << QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
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
    _xmlReader.setDevice(&file);
    if (_xmlReader.readNextStartElement()) {
        //qDebug() << " xmlr.name() " << xmlr.name();
        if (_xmlReader.name() == "misuco" ) {
            while (_xmlReader.readNextStartElement()) {
                if(filetype=="conf.xml") {
                    decodeConfigRecord();
                   _app->updateMenuButtonState();
                } else if(filetype=="scales.xml") {
                    decodeScaleRecord();
                } else if(filetype=="synth.xml") {
                    decodeSynthRecord();
                } else if(filetype=="tune.xml") {
                    decodeTuneRecord();
                }
                _xmlReader.skipCurrentElement();
            }
        } else {
            return;
        }
    }
    file.close();
}

void XmlLoader::decodeConfigRecord() {
    if (_xmlReader.name() == "setup") {
        _app->_presetsVisible=_xmlReader.attributes().value("presetsVisible").toInt();
        _app->_menuVisible=_xmlReader.attributes().value("menuVisible").toInt();

        _app->_rootNoteSetterVisible=_xmlReader.attributes().value("rootNoteSetterVisible").toInt();
        _app->_bScaleSwitchVisible=_xmlReader.attributes().value("bScaleSwitchVisible").toInt();
        _app->_octaveRangerVisible=_xmlReader.attributes().value("octaveRangerVisible").toInt();

        _app->_playAreaVisible=_xmlReader.attributes().value("playAreaVisible").toInt();
        _app->_tuneAreaVisible=_xmlReader.attributes().value("tuneAreaVisible").toInt();
        _app->_synthAreaVisible=_xmlReader.attributes().value("synthAreaVisible").toInt();
        _app->_confAreaVisible=_xmlReader.attributes().value("confAreaVisible").toInt();

        _app->_scalePresetsVisible=_xmlReader.attributes().value("scalePresetsVisible").toInt();
        _app->_synthPresetsVisible=_xmlReader.attributes().value("synthPresetsVisible").toInt();
        _app->_tunePresetsVisible=_xmlReader.attributes().value("tunePresetsVisible").toInt();

        _app->faderPitchTopRange->setValue(_xmlReader.attributes().value("pitchTopRange").toString().toInt());
        _app->faderPitchBottomRange->setValue(_xmlReader.attributes().value("pitchBottomRange").toString().toInt());
        _app->pitchHorizontal->setState(_xmlReader.attributes().value("pitchHorizontal").toString().toInt());

        _app->faderChannel->setValue(_xmlReader.attributes().value("channel").toString().toInt());

        _app->enableCc1->setState(_xmlReader.attributes().value("pitchHorizontal").toString().toInt());

        //MGlob::bwmode = xmlr.attributes().value("bwmode").toString().toInt();
        //bwMode->setState(11,xmlr.attributes().value("bwmode").toString().toInt());

        out->setSenderEnabled(0,_xmlReader.attributes().value("mobileSynth").toString().toInt());
        enableMobilesynth->setState(17,_xmlReader.attributes().value("mobileSynth").toString().toInt());

        out->setSenderEnabled(1,_xmlReader.attributes().value("pureData").toString().toInt());
        enablePuredata->setState(18,_xmlReader.attributes().value("pureData").toString().toInt());

        out->setSenderEnabled(2,_xmlReader.attributes().value("reaktor").toString().toInt());
        enableReaktor->setState(19,_xmlReader.attributes().value("reaktor").toString().toInt());

        out->setSenderEnabled(3,_xmlReader.attributes().value("superCollider").toString().toInt());
        enableSupercollider->setState(20,_xmlReader.attributes().value("superCollider").toString().toInt());

        faderSymbols->setValue(_xmlReader.attributes().value("noteSymbols").toString().toInt());

        //MGlob::showFreqs = xmlr.attributes().value("showFreqs").toString().toInt();
        //showFreqs->setState(22,xmlr.attributes().value("showFreqs").toString().toInt());
    }
}

void XmlLoader::decodeScaleRecord() {
    if (_xmlReader.name() == "scale") {
        QList<bool> bscaleRead;
        for(int i=0;i<BSCALE_SIZE;i++) {
            QString attId;
            attId.sprintf("b%d",i);
            bscaleRead.append(_xmlReader.attributes().value(attId).toInt());
        }
        MWScalePreset * p = new MWScalePreset(_xmlReader.attributes().value("rootNote").toString().toInt(),
                                         bscaleRead,
                                         this);
        connect(p,SIGNAL(editPreset()),_scalePresets,SLOT(onEditPreset()));
        _scalePresets->append(p);
    }
}

void XmlLoader::decodeSynthRecord() {
    if (_xmlReader.name() == "sound") {
        MWSoundPreset * soundPreset = new MWSoundPreset(
                    _xmlReader.attributes().value("volume").toString().toFloat(),
                    _xmlReader.attributes().value("wave").toString().toInt(),
                    _xmlReader.attributes().value("attack").toString().toInt(),
                    _xmlReader.attributes().value("decay").toString().toInt(),
                    _xmlReader.attributes().value("sustain").toString().toFloat(),
                    _xmlReader.attributes().value("release").toString().toInt(),
                    _xmlReader.attributes().value("cutoff").toString().toFloat(),
                    _xmlReader.attributes().value("resonance").toString().toFloat(),
                    _xmlReader.attributes().value("mod_cutoff").toString().toFloat(),
                    _xmlReader.attributes().value("mod_resonance").toString().toFloat(),
                    this);
        connect(soundPreset,SIGNAL(setSound(MWSound*)),this,SLOT(setSound(MWSound*)));
        connect(soundPreset,SIGNAL(editPreset()),_scalePresets,SLOT(onEditPreset()));
        connect(this, SIGNAL(soundChanged()), soundPreset, SLOT(onSoundChanged()));
        _synthPresets->append(soundPreset);
    }
}

void XmlLoader::decodeTuneRecord() {
    if (_xmlReader.name() == "microtune") {
        int microtune[12];
        for(int i=0;i<12;i++) {
            QString attId;
            attId.sprintf("t%d",i);
            microtune[i]=_xmlReader.attributes().value(attId).toString().toInt();
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
        _xmlWriter.setDevice(&file);
        QString att;
        QString attId;

        _xmlWriter.writeStartDocument();
        _xmlWriter.writeDTD("<!DOCTYPE misuco>");
        _xmlWriter.writeStartElement("misuco");
        _xmlWriter.writeAttribute("version", "2.1");

        if(filename == "scales.xml") {

            for(auto widgetQ:_scalePresets->getItems()) {

                auto widget = qobject_cast<MWScalePreset*>(widgetQ);
                if(widget) {
                    _xmlWriter.writeStartElement("scale");
                    att.sprintf("%d",widget->getRootNote());
                    _xmlWriter.writeAttribute("rootNote",att);
                    for(int i=0;i<BSCALE_SIZE;i++) {
                        att.sprintf("%d",widget->getScale(i));
                        attId.sprintf("b%d",i);
                        _xmlWriter.writeAttribute(attId,att);
                    }
                    _xmlWriter.writeEndElement();
                }
            }
        } else if(filename == "synth.xml") {
            for(auto o:_synthPresets->getItems()) {
                auto widget = qobject_cast<MWSoundPreset *>(o);
                if(widget) {
                    _xmlWriter.writeStartElement("sound");
                    att.sprintf("%d",widget->wave());
                    _xmlWriter.writeAttribute("wave",att);
                    att.sprintf("%d",widget->attack());
                    _xmlWriter.writeAttribute("attack",att);
                    att.sprintf("%d",widget->decay());
                    _xmlWriter.writeAttribute("decay",att);
                    att.sprintf("%f",widget->sustain());
                    _xmlWriter.writeAttribute("sustain",att);
                    att.sprintf("%d",widget->release());
                    _xmlWriter.writeAttribute("release",att);
                    att.sprintf("%f",widget->cutoff());
                    _xmlWriter.writeAttribute("cutoff",att);
                    att.sprintf("%f",widget->resonance());
                    _xmlWriter.writeAttribute("resonance",att);
                    att.sprintf("%f",widget->mod_cutoff());
                    _xmlWriter.writeAttribute("mod_cutoff",att);
                    att.sprintf("%f",widget->mod_resonance());
                    _xmlWriter.writeAttribute("mod_resonance",att);
                    att.sprintf("%f",widget->volume());
                    _xmlWriter.writeAttribute("volume",att);
                    _xmlWriter.writeEndElement();
                }
            }
        } else if(filename == "tune.xml") {
            for(auto o:_tunePresets->getItems()) {
                auto widget = qobject_cast<MWMicrotunePreset *>(o);
                _xmlWriter.writeStartElement("microtune");
                for(int i=0;i<12;i++) {
                    att.sprintf("%d",widget->PresetMicrotune.tuning[i]);
                    attId.sprintf("t%d",i);
                    _xmlWriter.writeAttribute(attId,att);
                }
                _xmlWriter.writeEndElement();
            }
        } else if(filename == "conf.xml") {
            _xmlWriter.writeStartElement("setup");

            att.sprintf("%d",faderPitchTopRange->getValue());
            _xmlWriter.writeAttribute("pitchTopRange",att);
            att.sprintf("%d",faderPitchBottomRange->getValue());
            _xmlWriter.writeAttribute("pitchBottomRange",att);
            att.sprintf("%d",pitchHorizontal->getState());
            _xmlWriter.writeAttribute("pitchHorizontal",att);
            att.sprintf("%d",MGlob::channel);
            _xmlWriter.writeAttribute("channel",att);
            att.sprintf("%d",_app->enableCc1->getState());
            _xmlWriter.writeAttribute("sendCC1",att);
            //att.sprintf("%d",);
            //xml.writeAttribute("bwmode",att);
            att.sprintf("%d",out->isSenderEnabled(0));
            _xmlWriter.writeAttribute("mobileSynth",att);
            att.sprintf("%d",out->isSenderEnabled(1));
            _xmlWriter.writeAttribute("pureData",att);
            att.sprintf("%d",out->isSenderEnabled(2));
            _xmlWriter.writeAttribute("reaktor",att);
            att.sprintf("%d",out->isSenderEnabled(3));
            _xmlWriter.writeAttribute("superCollider",att);
            //att.sprintf("%d",MGlob::noteSymbols);
            //xml.writeAttribute("noteSymbols",att);
            //att.sprintf("%d",MGlob::showFreqs);
            //xml.writeAttribute("showFreqs",att);

            att.sprintf("%d",_presetsVisible);
            _xmlWriter.writeAttribute("presetsVisible",att);
            att.sprintf("%d",_menuVisible);
            _xmlWriter.writeAttribute("menuVisible",att);

            att.sprintf("%d",_rootNoteSetterVisible);
            _xmlWriter.writeAttribute("rootNoteSetterVisible",att);
            att.sprintf("%d",_bScaleSwitchVisible);
            _xmlWriter.writeAttribute("bScaleSwitchVisible",att);
            att.sprintf("%d",_octaveRangerVisible);
            _xmlWriter.writeAttribute("octaveRangerVisible",att);

            att.sprintf("%d",_playAreaVisible);
            _xmlWriter.writeAttribute("playAreaVisible",att);
            att.sprintf("%d",_synthAreaVisible);
            _xmlWriter.writeAttribute("synthAreaVisible",att);
            att.sprintf("%d",_confAreaVisible);
            _xmlWriter.writeAttribute("confAreaVisible",att);
            att.sprintf("%d",_tuneAreaVisible);
            _xmlWriter.writeAttribute("tuneAreaVisible",att);

            att.sprintf("%d",_scalePresetsVisible);
            _xmlWriter.writeAttribute("scalePresetsVisible",att);
            att.sprintf("%d",_synthPresetsVisible);
            _xmlWriter.writeAttribute("synthPresetsVisible",att);
            att.sprintf("%d",_tunePresetsVisible);
            _xmlWriter.writeAttribute("tunePresetsVisible",att);

            _xmlWriter.writeEndElement();

        }

        _xmlWriter.writeEndDocument();

        file.close();

    } else {
        qDebug() << "cannot write " << filename;
    }

}

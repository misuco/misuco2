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

#include <conf/color.h>

wlayout::wlayout(QWidget *parent) : QObject(parent)
{
    presetsVisible=false;
    headerVisible=false;

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

    M[0] = new MWPlayArea(MisuWidget::MWPitch,this);
    ((MWPlayArea *)M[0])->setOut(out);
    connect(M[0],SIGNAL(menuTouch()), this, SLOT(toggleMenu()));
    connect(M[0],SIGNAL(presetsTouch()), this, SLOT(togglePresets()));

    for(int i=0;i<BSCALE_SIZE+1;i++) {
        connect( MisuWidget::MWPitch[i], SIGNAL(pitchChanged()), (MWPlayArea *)M[0], SLOT(pitchChange()));
    }

    H[0] = new MWOctaveRanger(this);
    connect(H[0],SIGNAL(setOctConf(int,int)),M[0],SLOT(setOctConf(int,int)));

    for(int i=0;i<BSCALE_SIZE+1;i++) {
        faderMicrotune[i] = new MWFaderPitch(this,MisuWidget::MWPitch[i]);
        faderMicrotune[i]->setOut(out);
        connect (faderMicrotune[i],SIGNAL(valueChange(int)),MisuWidget::MWPitch[i],SLOT(setPitch(int)));
        connect( MisuWidget::MWPitch[i], SIGNAL(pitchChanged()), faderMicrotune[i], SLOT(pitchChange()));
        connect(H[0],SIGNAL(setOctMid(int)),faderMicrotune[i],SLOT(setOctMid(int)));
    }

    Color * synthCtlColor=new Pitch(1,this);

    for(int i=0;i<10;i++) {
        faderParamCtl[i] = new MWFaderParamCtl(this,synthCtlColor,i+102);
        faderParamCtl[i]->setOut(out);
        faderParamCtl[i]->setMinValue(0);
        if(i==0) {
            faderParamCtl[i]->setMaxValue(4);
        } else {
            faderParamCtl[i]->setMaxValue(1000);
        }
        faderParamCtl[i]->setInverted(true);

        // update fadders on sustain update
        if(i==3) {
            connect(faderParamCtl[i],SIGNAL(valueChange(int)),this,SLOT(onSoundSustainUpdate(int)));
        }
        // update fadders on filter res update
        if(i==6) {
            connect(faderParamCtl[i],SIGNAL(valueChange(int)),this,SLOT(onSoundSustainUpdate(int)));
        }
    }

    faderPitchTopRange = new MWFaderParamCtl(this,synthCtlColor,1);
    faderPitchTopRange->setOut(out);
    faderPitchTopRange->setMinValue(-5);
    faderPitchTopRange->setMaxValue(5);
    connect(faderPitchTopRange,SIGNAL(valueChange(int)),M[0],SLOT(setBendVertTop(int)));

    faderPitchBottomRange = new MWFaderParamCtl(this,synthCtlColor,2);
    faderPitchBottomRange->setOut(out);
    faderPitchBottomRange->setMinValue(-5);
    faderPitchBottomRange->setMaxValue(5);
    connect(faderPitchBottomRange,SIGNAL(valueChange(int)),M[0],SLOT(setBendVertBot(int)));

    pitchHorizontal = new MWHeaderSetter(15,this);
    connect(pitchHorizontal,SIGNAL(setBendHori(bool)),M[0],SLOT(setBendHori(bool)));
    connect(this,SIGNAL(setBendHori(bool)),M[0],SLOT(setBendHori(bool)));

    faderChannel = new MWFaderParamCtl(this,synthCtlColor,3);
    faderChannel->setOut(out);
    faderChannel->setMinValue(1);
    faderChannel->setMaxValue(16);
    faderChannel->setInverted(true);
    connect(faderChannel,SIGNAL(valueChange(int)),this,SLOT(onChannelChange(int)));

    enableCc1 = new MWHeaderSetter(16,1,this);
    MisuWidget::sendCC1 = true;

    bwMode = new MWHeaderSetter(11,this);
    connect(bwMode,SIGNAL(toggleBW()),this,SLOT(toggleBW()));

    enableMobilesynth = new MWHeaderSetter(17,1,this);
    connect(enableMobilesynth,SIGNAL(toggleSender(int)),this,SLOT(onToggleSender(int)));

    enablePuredata = new MWHeaderSetter(18,this);
    connect(enablePuredata,SIGNAL(toggleSender(int)),this,SLOT(onToggleSender(int)));

    enableReaktor = new MWHeaderSetter(19,1,this);
    connect(enableReaktor,SIGNAL(toggleSender(int)),this,SLOT(onToggleSender(int)));

    enableSupercollider = new MWHeaderSetter(20,this);
    connect(enableSupercollider,SIGNAL(toggleSender(int)),this,SLOT(onToggleSender(int)));

    holdMode = new MWHeaderSetter(21,this);

    overwritePreset = new MWHeaderSetter(12,this);

    for(int i=0;i<12;i++) {
        MWBaseNoteSetter * baseNoteSetter = new MWBaseNoteSetter(MisuWidget::MWPitch[i],this);
        baseNoteSetter->setOut(out);
        connect(baseNoteSetter,SIGNAL(setBaseNote(Pitch *)),M[0],SLOT(setBaseNote(Pitch *)));
        connect(baseNoteSetter,SIGNAL(setBaseNote(Pitch *)),this,SLOT(onSetBaseNote(Pitch *)));
        connect(baseNoteSetter,SIGNAL(scaleupdate()),this,SLOT(onscaleupdate()));
        connect(this,SIGNAL(setBaseNote(Pitch*)),baseNoteSetter,SLOT(onSetBaseNote(Pitch*)));
        connect(H[0],SIGNAL(setOctMid(int)),baseNoteSetter,SLOT(setOctMid(int)));
        connect(MisuWidget::MWPitch[i], SIGNAL(pitchChanged()) ,baseNoteSetter, SLOT(pitchChange()));
        _BaseNoteSetter.append(baseNoteSetter);
    }

    for(int i=1;i<12;i++) {
        MWBScaleSwitch * bs =new MWBScaleSwitch(i);
        bs->setOut(out);
        connect(bs,SIGNAL(setBscale(int,bool)),M[0],SLOT(setBscale(int,bool)));
        connect(bs,SIGNAL(scaleupdate()),this,SLOT(onscaleupdate()));
        connect(H[0],SIGNAL(setOctMid(int)),bs,SLOT(setOctMid(int)));
        for(int j=0;j<12;j++) {
            connect(_BaseNoteSetter[j],SIGNAL(setBaseNote(Pitch *)),bs,SLOT(setBaseNote(Pitch *)));
        }
        _BScaleSwitch.append(bs);
    }

    openScalesArchive = new MWHeaderSetter(13,1,this);

    faderSymbols = new MWFaderParamCtl(this,synthCtlColor,4);
    faderSymbols->setOut(out);
    faderSymbols->setMinValue(0);
    faderSymbols->setMaxValue(4);
    faderSymbols->setInverted(true);
    connect(faderSymbols,SIGNAL(valueChange(int)),this,SLOT(onSymbolsChange(int)));
    connect(this,SIGNAL(scaleupdate()),M[0],SLOT(onscaleupdate()));
    connect(this,SIGNAL(scaleupdate()),_BaseNoteSetter[0],SLOT(onscaleupdate()));
    for(int j=1;j<12;j++) {
        connect(this,SIGNAL(scaleupdate()),_BaseNoteSetter[j],SLOT(onscaleupdate()));
        connect(this,SIGNAL(scaleupdate()),_BScaleSwitch[j-1],SLOT(onscaleupdate()));
    }

    showFreqs = new MWHeaderSetter(22,this);
    connect(showFreqs,SIGNAL(toggleShowFreqs()),this,SLOT(onShowFreqsChange()));

    for(int i=0;i<7;i++) {
        int fctId=i;
        if(i>=3) fctId+=3;
        HS[i] = new MWHeaderSetter(fctId,this);
        connect(this,SIGNAL(setMenuItemState(int,int)),HS[i],SLOT(setState(int,int)));
        if(i<3) {
            connect(HS[i],SIGNAL(currentHeader(int)),this,SLOT(currentHeader(int)));
        } else if(i==3) {
            connect(HS[i],SIGNAL(currentMainView(int)),this,SLOT(currentMainView(int)));
        } else if(i==4) {
            connect(HS[i],SIGNAL(currentMainView(int)),this,SLOT(currentMainView(int)));
        } else if(i==5) {
            connect(HS[i],SIGNAL(currentMainView(int)),this,SLOT(currentMainView(int)));
        } else if(i==6) {
            connect(HS[i],SIGNAL(currentMainView(int)),this,SLOT(currentMainView(int)));
        }
    }

    readXml(configPath);

    for(auto presetButton:scalePresets) {
        connect(presetButton,SIGNAL(setScale(MWScale*)),(MWPlayArea *)M[0],SLOT(setScale(MWScale*)));
        connect(presetButton,SIGNAL(scaleupdate()),this,SLOT(onscaleupdate()));
        connect(presetButton,SIGNAL(setScale(MWScale*)),_BaseNoteSetter[0],SLOT(onScaleSet(MWScale*)));
        for(int j=1;j<12;j++) {
            connect(presetButton,SIGNAL(setScale(MWScale*)),_BaseNoteSetter[j],SLOT(onScaleSet(MWScale*)));
            connect(presetButton,SIGNAL(setScale(MWScale*)),_BScaleSwitch[j-1],SLOT(onScaleSet(MWScale*)));
        }
    }

    if(scalePresets.size()>0) {
        connect(this,SIGNAL(initialSet()),scalePresets[0],SLOT(initialSet()));
    }
    if(soundPresets.size()>0) {
        connect(this,SIGNAL(initialSet()),soundPresets[0],SLOT(initialSet()));
    }
    if(microtunePresets.size()>0) {
        connect(this,SIGNAL(initialSet()),microtunePresets[0],SLOT(initialSet()));
    }
    emit initialSet();

}

wlayout::~wlayout()
{
    writeXml(configPath);
}

void wlayout::resizeEvent(QResizeEvent *)
{
    /*
    //qDebug() << "wlayout::resizeEvent " << width() << " " << height();
    if(width()>height()) {
        MisuWidget::font1size=width()/40;
    } else {
        MisuWidget::font1size=height()/40;

    }
    recalcMainView();
    */
}

void wlayout::currentHeader(int)
{

    /*
    for(int j=0;j<3;j++) {
        if(i!=j) {
            H[j]->hide();
            emit setMenuItemState(j,0);
        }
    }

    if(H[i]->isHidden()) {
        H[i]->show();
    } else {
        H[i]->hide();
    }
    recalcMainView();
    */
}

void wlayout::currentMainView(int)
{
    /*
    if(i>0) {
        for(int j=1;j<4;j++) {
            if(i!=j) {
                M[j]->hide();
                emit setMenuItemState(j+6,0);
            }
        }
    }


    if(M[i]->isHidden()) {
        M[i]->show();
        emit setMenuItemState(i+6,1);
    } else {
        M[i]->hide();
        emit setMenuItemState(i+6,0);
    }
    recalcMainView();
    */
}

/*
void wlayout::recalcMainView()
{    
    int mainCnt=0;
    int headerCnt=0;

    for(int i=0;i<4;i++) {
        layout->removeWidget(M[i]);
        if(!M[i]->isHidden()) {mainCnt++;}
    }

    for(int i=0;i<3;i++) {
        layout->removeWidget(H[i]);
        if(!H[i]->isHidden()) {headerCnt+=2;}
    }

    for(int i=0;i<7;i++) {
        layout->removeWidget(HS[i]);
        HS[i]->hide();
    }

    layout->removeWidget(overwritePreset);
    overwritePreset->hide();

    for(auto widget:scalePresets) {
        layout->removeWidget(widget);
        widget->hide();
    }
    for(auto widget:soundPresets) {
        layout->removeWidget(widget);
        widget->hide();
    }
    for(auto widget:microtunePresets) {
        layout->removeWidget(widget);
        widget->hide();
    }

    if(mainCnt==0) {
        M[0]->show();
        mainCnt=1;
        emit setMenuItemState(6,1);
    }


    if(width()>height()) {

        int height=(15-headerCnt)/mainCnt;
        int roundDiff=15-headerCnt-mainCnt*height;
        int top=0;
        int xpos = 0;
        int width = 14;

        if(presetsVisible) {
            layout->addWidget(overwritePreset,0,0,1,2);
            overwritePreset->show();

            int presetCount1 = 6;
            int presetCount2 = 0;
            if(mainCnt>1) {
                presetCount1 = 3;
                presetCount2 = 3;
                for(int i=0;i<presetCount1;i++) {
                    if(i<scalePresets.size()) {
                        layout->addWidget(scalePresets[i],(i+1)*2,0,1,2);
                        scalePresets[i]->show();
                    }
                }

                if(M[1]->isHidden()) {
                    for(int i=0;i<presetCount2;i++) {
                        if(i<soundPresets.size()) {
                            layout->addWidget(soundPresets[i],(i+1)*2+presetCount1*2,0,1,2);
                            soundPresets[i]->show();
                        }
                    }
                } else {
                    for(int i=0;i<presetCount2;i++) {
                        if(i<microtunePresets.size()) {
                            layout->addWidget(microtunePresets[i],(i+1)*2+presetCount1*2,0,1,2);
                            microtunePresets[i]->show();
                        }
                    }
                }

            } else {
                for(int i=0;i<presetCount1;i++) {
                    if(M[0]->isHidden()) {
                        if(M[1]->isHidden()) {
                            if(i<soundPresets.size()) {
                                layout->addWidget(soundPresets[i],(i+1)*2,0,1,2);
                                soundPresets[i]->show();
                            }
                        } else {
                            if(i<microtunePresets.size()) {
                                layout->addWidget(microtunePresets[i],(i+1)*2,0,1,2);
                                microtunePresets[i]->show();
                            }
                        }
                    } else {
                        if(i<scalePresets.size()) {
                            layout->addWidget(scalePresets[i],(i+1)*2,0,1,2);
                            scalePresets[i]->show();
                        }
                    }
                }
            }
        }

        if(presetsVisible) {
            width-=2;
            xpos+=2;
        }

        if(headerVisible) {
            width-=2;
        }

        for(int i=0;i<3;i++) {
            if(!H[i]->isHidden()) {
                //qDebug() << "layout->addWidget "  << i << " top " << top << " xpos " << xpos << " h " << height << " w " << width;
                layout->addWidget(H[i],top,xpos,2,width);
                top+=2;
            }
        }

        for(int i=0;i<4;i++) {
            if(!M[i]->isHidden()) {
                //qDebug() << "layout->addWidget "  << i << " top " << top << " xpos " << xpos << " h " << height << " w " << width;
                layout->addWidget(M[i],top,xpos,height+roundDiff,width);
                top+=height+roundDiff;
                roundDiff=0;
            }
        }

        if(headerVisible) {
            for(int i=0;i<7;i++) {
                layout->addWidget(HS[i],i*2,xpos+width,2,2);
                HS[i]->show();
            }
        }

    } else {



        int height = 30;
        int roundDiff = 0;

        int top = 0;
        int xpos = 0;
        int width = 7;

        if(headerVisible) {
            for(int i=0;i<7;i++) {
                //qDebug() << "layout->addWidget HS "  << i << " top " << top << " xpos " << xpos << " h " << height << " w " << width;
                layout->addWidget(HS[i],top,i,10,1);
                HS[i]->show();
            }
            top+=10;
        }

        if(presetsVisible) {
            layout->addWidget(overwritePreset,top,xpos,10,1);
            overwritePreset->show();

            int presetCount1 = 6;
            int presetCount2 = 0;
            if(mainCnt>1) {
                presetCount1 = 3;
                presetCount2 = 3;
                for(int i=0;i<presetCount1;i++) {
                    if(i<scalePresets.size()) {
                        //qDebug() << "layout->addWidget scalePreset "  << i << " top " << top << " xpos " << xpos << " h " << height << " w " << width;
                        layout->addWidget(scalePresets[i],top,i+1,10,1);
                        scalePresets[i]->show();
                    }
                }

                if(M[1]->isHidden()) {
                    for(int i=0;i<presetCount2;i++) {
                        if(i<soundPresets.size()) {
                            //qDebug() << "layout->addWidget soundPreset "  << i << " top " << top << " xpos " << xpos << " h " << height << " w " << width;
                            layout->addWidget(soundPresets[i],top,i+1+presetCount1,10,1);
                            soundPresets[i]->show();
                        }
                    }
                } else {
                    for(int i=0;i<presetCount2;i++) {
                        if(i<microtunePresets.size()) {
                            //qDebug() << "layout->addWidget microtunePreset "  << i << " top " << top << " xpos " << xpos << " h " << height << " w " << width;
                            layout->addWidget(microtunePresets[i],top,(i+1)+presetCount1,10,1);
                            microtunePresets[i]->show();
                        }
                    }
                }
            } else {
                for(int i=0;i<presetCount1;i++) {
                    if(M[0]->isHidden()) {
                        if(M[1]->isHidden()) {
                            if(i<soundPresets.size()) {
                                //qDebug() << "layout->addWidget soundPreset "  << i << " top " << top << " xpos " << xpos << " h " << height << " w " << width;
                                layout->addWidget(soundPresets[i],top,i+1,10,1);
                                soundPresets[i]->show();
                            }
                        } else {
                            if(i<microtunePresets.size()) {
                                //qDebug() << "layout->addWidget microtunePreset "  << i << " top " << top << " xpos " << xpos << " h " << height << " w " << width;
                                layout->addWidget(microtunePresets[i],top,i+1,10,1);
                                microtunePresets[i]->show();
                            }
                        }
                    } else {
                        if(i<scalePresets.size()) {
                            //qDebug() << "layout->addWidget scalePreset "  << i << " top " << top << " xpos " << xpos << " h " << height << " w " << width;
                            layout->addWidget(scalePresets[i],top,i+1,10,1);
                            scalePresets[i]->show();
                        }
                    }
                }
            }
            top+=10;
        }

        for(int i=0;i<3;i++) {
            if(!H[i]->isHidden()) {
                //qDebug() << "layout->addWidget H "  << i << " top " << top << " xpos " << xpos << " h " << height << " w " << width;
                layout->addWidget(H[i],top,xpos,10,width);
                top+=10;
            }
        }

        for(int i=0;i<4;i++) {
            if(!M[i]->isHidden()) {
                //qDebug() << "layout->addWidget M "  << i << " top " << top << " xpos " << xpos << " h " << height << " w " << width;
                layout->addWidget(M[i],top,xpos,height,width);
                top+=height+roundDiff;
                roundDiff=0;
            }
        }

    }

}
*/

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

void wlayout::toggleBW()
{
    /*
    for(auto widget:scalePresets) {
        widget->//update();
    }

    for(int i=0;i<3;i++) {
        H[i]->//update();
        M[i]->//update();
    }
    */
}

void wlayout::onSetBaseNote(Pitch *p)
{
    emit setBaseNote(p);
}

void wlayout::setSound(MWSound *s)
{
    faderParamCtl[0]->setValue(s->wave_type);
    faderParamCtl[1]->setValue(s->attack);
    faderParamCtl[2]->setValue(s->decay);
    faderParamCtl[3]->setValue(s->sustain);
    faderParamCtl[4]->setValue(s->release);
    faderParamCtl[5]->setValue(s->filter_cutoff);
    faderParamCtl[6]->setValue(s->filter_resonance);
    faderParamCtl[7]->setValue(s->mod_filter_cutoff);
    faderParamCtl[8]->setValue(s->mod_filter_resonance);
    faderParamCtl[9]->setValue(s->volume);
    /*
    for(auto widget:soundPresets) {
        widget->//update();
    }
    */
}

void wlayout::setMicrotune(MWMicrotune * m)
{
    for(int i=0;i<12;i++) {
        MisuWidget::Microtune.tuning[i] = m->tuning[i];
        MisuWidget::MWPitch[i]->setPitch(m->tuning[i]);
        faderMicrotune[i]->setValue(m->tuning[i]);
    }
    /*
    M[0]->//update();
    for(auto widget:microtunePresets) {
        widget->//update();
    }
    */
}

void wlayout::onscaleupdate()
{
    /*
    for(auto widget:scalePresets) {
        widget->//update();
    }
    */
}

void wlayout::onSoundSustainUpdate(int)
{
    /*
    faderParamCtl[2]->//update();
    faderParamCtl[4]->//update();
    faderParamCtl[5]->//update();
    */
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
    emit scaleupdate();
}

void wlayout::onShowFreqsChange()
{
    emit scaleupdate();
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

    for(auto widget:scalePresets) {
        xml.writeStartElement("scale");
        att.sprintf("%d",widget->PresetScale.basenote);
        xml.writeAttribute("basenote",att);
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

    for(auto widget:soundPresets) {
        xml.writeStartElement("sound");
        att.sprintf("%d",widget->PresetSound.wave_type);
        xml.writeAttribute("wave",att);
        att.sprintf("%d",widget->PresetSound.attack);
        xml.writeAttribute("attack",att);
        att.sprintf("%d",widget->PresetSound.decay);
        xml.writeAttribute("decay",att);
        att.sprintf("%f",widget->PresetSound.sustain);
        xml.writeAttribute("sustain",att);
        att.sprintf("%d",widget->PresetSound.release);
        xml.writeAttribute("release",att);
        att.sprintf("%f",widget->PresetSound.filter_cutoff);
        xml.writeAttribute("cutoff",att);
        att.sprintf("%f",widget->PresetSound.filter_resonance);
        xml.writeAttribute("resonance",att);
        att.sprintf("%f",widget->PresetSound.mod_filter_cutoff);
        xml.writeAttribute("mod_cutoff",att);
        att.sprintf("%f",widget->PresetSound.mod_filter_resonance);
        xml.writeAttribute("mod_resonance",att);
        att.sprintf("%f",widget->PresetSound.volume);
        xml.writeAttribute("volume",att);
        xml.writeEndElement();
    }

    for(auto widget:microtunePresets) {
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
            scalePresets.append(new MWPreset(MisuWidget::MWPitch,
                                             xmlr.attributes().value("basenote").toString().toInt(),
                                             xmlr.attributes().value("baseoct").toString().toInt(),
                                             xmlr.attributes().value("topoct").toString().toInt(),
                                             bscaleRead,
                                             this));
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
            soundPresets.append(soundPreset);
        } else if (xmlr.name() == "microtune") {
            int microtune[12];
            for(int i=0;i<12;i++) {
                QString attId;
                attId.sprintf("t%d",i);
                microtune[i]=xmlr.attributes().value(attId).toString().toInt();
            }
            MWMicrotunePreset * microtunePreset = new MWMicrotunePreset(microtune,this);
            connect(microtunePreset,SIGNAL(setMicrotune(MWMicrotune*)),this,SLOT(setMicrotune(MWMicrotune*)));
            microtunePresets.append(microtunePreset);
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

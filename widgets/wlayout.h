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

#ifndef WLAYOUT_H
#define WLAYOUT_H

#include <QWidget>
#include <QGridLayout>
#include <QFile>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include "widgets/mwplayarea.h"
#include "widgets/mwbasenotesetter.h"
#include "comm/sendermobilesynth.h"
#include "comm/senderdebug.h"
#include "widgets/mwoctaveranger.h"
#include "conf/types.h"
#include <QStackedWidget>
#include "mwfaderparamctl.h"
#include "mwbscaleswitch.h"
#include "mwheadersetter.h"
#include "comm/sendermulti.h"
#include "mwfaderpitch.h"
#include "mwpreset.h"
#include "mwsoundpreset.h"
#include "mwmicrotunepreset.h"

class wlayout : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QList<QObject*> baseNoteSetter MEMBER _BaseNoteSetter CONSTANT)
    Q_PROPERTY(QList<QObject*> bScaleSwitch MEMBER _BScaleSwitch CONSTANT)

public:
    explicit wlayout(QWidget *parent = 0);
    ~wlayout();

protected:
    void resizeEvent(QResizeEvent *);

signals:
    void setBaseNote(Pitch * p);
    void initialSet();
    void scaleupdate();
    void setMenuItemState(int id, int s);
    void setBendHori(bool);

private slots:    
    void currentHeader(int i);
    void currentMainView(int i);
    void togglePresets();
    void toggleMenu();
    void toggleBW();
    void onSetBaseNote(Pitch * p);
    void setSound(MWSound * s);
    void setMicrotune(MWMicrotune*m);
    void onscaleupdate();
    void onSoundSustainUpdate(int);
    void onChannelChange(int v);
    void onToggleSender(int v);
    void onSymbolsChange(int v);
    void onShowFreqsChange();

private:
    QGridLayout * layout;
    SenderMulti * out;

    QXmlStreamWriter xml;
    QXmlStreamReader xmlr;

    QList<QObject*> _BaseNoteSetter;
    QList<QObject*> _BScaleSwitch;
    MWOctaveRanger * OctaveRanger;

    // synth ctl faders
    MWFaderParamCtl * faderParamCtl[10];
    MWFaderPitch * faderMicrotune[12];

    // header widgets
    QObject * H[3];

    // main area widgets
    QObject * M[4];

    // right menu
    QObject * HS[15];

    // preset buttons
    QList<MWPreset * > scalePresets;
    QList<MWSoundPreset * > soundPresets;
    QList<MWMicrotunePreset * > microtunePresets;

    bool presetsVisible;
    bool headerVisible;

    MWFaderParamCtl * faderPitchTopRange;
    MWFaderParamCtl * faderPitchBottomRange;
    MWFaderParamCtl * faderChannel;
    MWHeaderSetter * pitchHorizontal;
    MWHeaderSetter * bwMode;
    MWHeaderSetter * enableCc1;
    MWHeaderSetter * enableMobilesynth;
    MWHeaderSetter * enableReaktor;
    MWHeaderSetter * enablePuredata;
    MWHeaderSetter * enableSupercollider;
    MWFaderParamCtl * faderSymbols;
    MWHeaderSetter * openScalesArchive;
    MWHeaderSetter * holdMode;
    MWHeaderSetter * showFreqs;
    MWHeaderSetter * overwritePreset;

    QString configPath;

    void recalcMainView();
    void readXml(QString filename);
    void writeXml(QString filename);
    void readLayout();
};

#endif // WLAYOUT_H

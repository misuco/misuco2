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

#include <QFile>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include "widgets/mwplayarea.h"
#include "widgets/mwrootnotesetter.h"
#include "comm/sendermobilesynth.h"
#include "comm/senderdebug.h"
#include "widgets/mwoctaveranger.h"
#include "conf/types.h"
#include "mwfaderparamctl.h"
#include "mwbscaleswitch.h"
#include "mwheadersetter.h"
#include "comm/sendermulti.h"
#include "mwfaderpitch.h"
#include "mwpreset.h"
#include "mwsoundpreset.h"
#include "mwmicrotunepreset.h"
#include "mwgame.h"

class wlayout : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QList<QObject*> rootNoteSetter MEMBER _rootNoteSetter CONSTANT)
    Q_PROPERTY(bool rootNoteSetterVisible MEMBER _rootNoteSetterVisible NOTIFY layoutChange)

    Q_PROPERTY(QList<QObject*> bScaleSwitch MEMBER _BScaleSwitch CONSTANT)
    Q_PROPERTY(bool bScaleSwitchVisible MEMBER _bScaleSwitchVisible NOTIFY layoutChange)

    Q_PROPERTY(QObject* octaveRanger MEMBER _OctaveRanger CONSTANT)
    Q_PROPERTY(bool octaveRangerVisible MEMBER _octaveRangerVisible NOTIFY layoutChange)

    Q_PROPERTY(QObject* playArea MEMBER _PlayArea CONSTANT)
    Q_PROPERTY(bool playAreaVisible MEMBER _playAreaVisible NOTIFY layoutChange)

    Q_PROPERTY(QList<QObject*> menu MEMBER _menu CONSTANT)

    Q_PROPERTY(QList<QObject*> tuneArea MEMBER _faderMicrotune CONSTANT)
    Q_PROPERTY(bool tuneAreaVisible MEMBER _tuneAreaVisible NOTIFY layoutChange)

    Q_PROPERTY(QList<QObject*> scalePresets MEMBER _scalePresets CONSTANT)
    Q_PROPERTY(bool scalePresetsVisible MEMBER _scalePresetsVisible NOTIFY layoutChange)

    Q_PROPERTY(QList<QObject*> synthPresets MEMBER _synthPresets CONSTANT)
    Q_PROPERTY(bool synthPresetsVisible MEMBER _synthPresetsVisible NOTIFY layoutChange)

    Q_PROPERTY(QList<QObject*> tunePresets MEMBER _tunePresets CONSTANT)
    Q_PROPERTY(bool tunePresetsVisible MEMBER _tunePresetsVisible NOTIFY layoutChange)

    Q_PROPERTY(QList<QObject*> synthArea MEMBER _faderParamCtl CONSTANT)
    Q_PROPERTY(bool synthAreaVisible MEMBER _synthAreaVisible NOTIFY layoutChange)

    Q_PROPERTY(QList<QObject*> confPitchFaders READ confPitchFaders CONSTANT)
    Q_PROPERTY(QObject* confPitchHorizButton MEMBER pitchHorizontal CONSTANT)

    Q_PROPERTY(QObject* confSymbolFader MEMBER faderSymbols CONSTANT)
    Q_PROPERTY(QObject* confBWModeButton MEMBER bwMode CONSTANT)
    Q_PROPERTY(QObject* confShowFreqsButton MEMBER showFreqs CONSTANT)
    Q_PROPERTY(QObject* confCC1Button MEMBER enableCc1 CONSTANT)
    Q_PROPERTY(QObject* confSendMobile MEMBER enableMobilesynth CONSTANT)
    Q_PROPERTY(QObject* confSendPuredata MEMBER enablePuredata CONSTANT)
    Q_PROPERTY(QObject* confSendReaktor MEMBER enableReaktor CONSTANT)
    Q_PROPERTY(QObject* confSendSupercollider MEMBER enableSupercollider CONSTANT)

    Q_PROPERTY(QObject* confChannelFader MEMBER faderChannel CONSTANT)

    Q_PROPERTY(QObject* openArchive MEMBER openArchive  CONSTANT)

    Q_PROPERTY(QObject* showPresets MEMBER showPresets CONSTANT)
    Q_PROPERTY(QObject* showMenu MEMBER showMenu CONSTANT)
    Q_PROPERTY(bool menuVisible MEMBER _menuVisible NOTIFY layoutChange)

    Q_PROPERTY(QObject* octUp MEMBER octUp CONSTANT)
    Q_PROPERTY(QObject* octDown MEMBER octDown CONSTANT)
    Q_PROPERTY(int topOct MEMBER _topOct NOTIFY octConfChanged)
    Q_PROPERTY(int botOct MEMBER _botOct NOTIFY octConfChanged)

    Q_PROPERTY(bool confAreaVisible MEMBER _confAreaVisible NOTIFY layoutChange)
    Q_PROPERTY(bool dialogPresetsVisible MEMBER _dialogPresetsVisible NOTIFY layoutChange)

    Q_PROPERTY(QList<QObject*> pitches READ pitches CONSTANT)

    Q_PROPERTY(QObject* game MEMBER _game CONSTANT)

public:
    explicit wlayout(QObject *parent = 0);
    ~wlayout();

    QList<QObject*> pitches();
    QList<QObject*> confPitchFaders();

    Q_INVOKABLE void closeDialogPreset();
    Q_INVOKABLE void overwritePreset();

signals:
    void setRootNote(Pitch * p);
    void initialSet();
    void scaleupdate();
    void setMenuItemState(int id, int s);
    void setBendHori(bool);
    void symbolsChanged();
    void soundChanged();

    // QML
    void layoutChange();
    void octConfChanged();

private slots:    
    void currentHeader(int id);
    void currentMainView(int id);
    void togglePresets();
    void toggleMenu();
    void onSetRootNote(Pitch * p);
    void setSound(MWSound * s);
    void setMicrotune(MWMicrotune*m);
    void onChannelChange(int v);
    void onToggleSender(int v);
    void onSymbolsChange(int v);
    void onShowFreqsChange();
    void onSoundChanged(int);
    void onGameStarted();

    void onEditPreset();
    void setOctConf(int bot, int top);

private:
    //QGridLayout * layout;
    SenderMulti * out;

    QXmlStreamWriter xml;
    QXmlStreamReader xmlr;

    QList<QObject*> _rootNoteSetter;
    QList<QObject*> _BScaleSwitch;
    MWOctaveRanger * _OctaveRanger;
    QObject *       _PlayArea;

    // synth ctl faders
    QList<QObject*> _faderParamCtl;
    QList<QObject*> _faderMicrotune;

    // main area widgets
    QObject * M[4];

    // right menu
    QList<QObject *> _menu;

    // preset buttons
    QList<QObject * > _scalePresets;
    QList<QObject * > _synthPresets;
    QList<QObject * > _tunePresets;

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
    MWHeaderSetter * openArchive;
    MWHeaderSetter * holdMode;
    MWHeaderSetter * showFreqs;
    MWHeaderSetter * showPresets;
    MWHeaderSetter * showMenu;
    MWHeaderSetter * octUp;
    MWHeaderSetter * octDown;

    MWGame * _game;

    // layout
    bool _menuVisible;
    bool _rootNoteSetterVisible;
    bool _bScaleSwitchVisible;
    bool _octaveRangerVisible;
    bool _playAreaVisible;
    bool _tuneAreaVisible;
    bool _synthAreaVisible;
    bool _confAreaVisible;
    bool _presetsVisible;
    bool _scalePresetsVisible;
    bool _synthPresetsVisible;
    bool _tunePresetsVisible;

    bool _dialogPresetsVisible;

    int _botOct;
    int _topOct;

    QString _configPath;

    void recalcMainView();
    void readXml(QString filetype);
    void writeXml(QString filetype);
    void updateMenuButtonState();
    void decodeConfigRecord();
    void decodeScaleRecord();
    void decodeSynthRecord();
    void decodeTuneRecord();
};

#endif // WLAYOUT_H

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

#include "widgets/core/playarea.h"
#include "widgets/core/rootnotesetter.h"
#include "widgets/core/octaveranger.h"
#include "conf/types.h"
#include "widgets/faders/mwfaderparamctl.h"
#include "widgets/core/scaleswitch.h"
#include "widgets/core/gamecontrol.h"
#include "comm/mastersender.h"
#include "comm/senderoscmidigeneric.h"
#include "comm/senderreaktor.h"
#include "widgets/faders/mwfaderpitch.h"
#include "widgets/presets/mwscalepreset.h"
#include "widgets/presets/mwsoundpreset.h"
#include "widgets/presets/mwmicrotunepreset.h"
#include "widgets/presets/presetcollection.h"
#include "widgets/buttons/openarchive.h"
#include "widgets/buttons/bendhorizontal.h"
#include "widgets/buttons/sendcc1.h"
#include "widgets/buttons/togglesender.h"
#include "widgets/buttons/togglebw.h"
#include "widgets/buttons/showfreqs.h"
#include "widgets/buttons/toggleheader.h"
#include "widgets/buttons/setmainview.h"
#include "widgets/buttons/togglemenu.h"
#include "widgets/buttons/togglepresets.h"
#include "widgets/buttons/octaveshift.h"
#include "conf/purchases.h"
#include "heartbeat.h"
#include "xmlloader.h"

class XmlLoader;

class Misuco2 : public QObject
{
    friend class XmlLoader;

    Q_OBJECT

    Q_PROPERTY(QList<QObject*> rootNoteSetter MEMBER _rootNoteSetter CONSTANT)
    Q_PROPERTY(bool rootNoteSetterVisible MEMBER _rootNoteSetterVisible NOTIFY layoutChange)

    Q_PROPERTY(QList<QObject*> ScaleSwitch MEMBER _scaleSwitch CONSTANT)
    Q_PROPERTY(bool ScaleSwitchVisible MEMBER _ScaleSwitchVisible NOTIFY layoutChange)

    Q_PROPERTY(QObject* octaveRanger MEMBER _OctaveRanger CONSTANT)
    Q_PROPERTY(bool octaveRangerVisible MEMBER _octaveRangerVisible NOTIFY layoutChange)

    Q_PROPERTY(QObject* playArea MEMBER _PlayArea CONSTANT)
    Q_PROPERTY(bool playAreaVisible MEMBER _playAreaVisible NOTIFY layoutChange)

    Q_PROPERTY(QList<QObject*> menu MEMBER _menu CONSTANT)

    Q_PROPERTY(QList<QObject*> tuneArea MEMBER _faderMicrotune CONSTANT)
    Q_PROPERTY(bool tuneAreaVisible MEMBER _tuneAreaVisible NOTIFY layoutChange)

    Q_PROPERTY(QObject* scalePresets MEMBER _scalePresets CONSTANT)
    Q_PROPERTY(bool scalePresetsVisible MEMBER _scalePresetsVisible NOTIFY layoutChange)

    Q_PROPERTY(QObject* synthPresets MEMBER _synthPresets CONSTANT)
    Q_PROPERTY(bool synthPresetsVisible MEMBER _synthPresetsVisible NOTIFY layoutChange)

    Q_PROPERTY(QObject* tunePresets MEMBER _tunePresets CONSTANT)
    Q_PROPERTY(bool tunePresetsVisible MEMBER _tunePresetsVisible NOTIFY layoutChange)

    Q_PROPERTY(QList<QObject*> synthArea MEMBER _faderParamCtl CONSTANT)
    Q_PROPERTY(bool synthAreaVisible MEMBER _synthAreaVisible NOTIFY layoutChange)

    Q_PROPERTY(QObject* confPitchTopFader MEMBER faderPitchTopRange CONSTANT)
    Q_PROPERTY(QObject* confPitchBottomFader MEMBER faderPitchBottomRange CONSTANT)
    Q_PROPERTY(QObject* confPitchHorizButton MEMBER pitchHorizontal CONSTANT)

    Q_PROPERTY(QObject* confSymbolFader MEMBER faderSymbols CONSTANT)
    Q_PROPERTY(QObject* confBWModeButton MEMBER _bwMode CONSTANT)
    Q_PROPERTY(QObject* confShowFreqsButton MEMBER _showFreqs CONSTANT)
    Q_PROPERTY(QObject* confCC1Button MEMBER _sendCc1 CONSTANT)
    Q_PROPERTY(QObject* confSendMobile MEMBER _enableMobilesynth CONSTANT)
    Q_PROPERTY(QObject* confSendPuredata MEMBER _enablePuredata CONSTANT)
    Q_PROPERTY(QObject* confSendReaktor MEMBER _enableReaktor CONSTANT)
    Q_PROPERTY(QObject* confSendSupercollider MEMBER _enableSupercollider CONSTANT)

    Q_PROPERTY(QObject* confChannelFader MEMBER faderChannel CONSTANT)

    Q_PROPERTY(QObject* openArchive MEMBER _openArchive  CONSTANT)

    Q_PROPERTY(QObject* togglePresets MEMBER _togglePresets CONSTANT)
    Q_PROPERTY(QObject* toggleMenu MEMBER _toggleMenu CONSTANT)
    Q_PROPERTY(bool menuVisible MEMBER _menuVisible NOTIFY layoutChange)

    Q_PROPERTY(QObject* octUp MEMBER _octUp CONSTANT)
    Q_PROPERTY(QObject* octDown MEMBER _octDown CONSTANT)
    Q_PROPERTY(int topOct MEMBER _topOct NOTIFY octConfChanged)
    Q_PROPERTY(int botOct MEMBER _botOct NOTIFY octConfChanged)

    Q_PROPERTY(bool confAreaVisible MEMBER _confAreaVisible NOTIFY layoutChange)

    Q_PROPERTY(QList<QObject*> pitchColors MEMBER _pitchColors CONSTANT)

    Q_PROPERTY(QObject* game MEMBER _game CONSTANT)

public:
    explicit Misuco2(QObject *parent = 0);
    ~Misuco2();

    void updateMenuButtonState();

signals:
    void initialSet();
    void scaleupdate();
    void setMenuItemState(int id, int s);
    void setBendHori(bool);

    // notify other presets on sound change
    void soundChanged();

    // QML
    void layoutChange();
    void octConfChanged();

private slots:    
    void toggleHeader(int id);
    void currentMainView(int id);
    void togglePresets(bool);
    void toggleMenu(bool);
    void setSound(MWSound* s);
    void setMicrotune(MWMicrotune* m);
    void onChannelChange(int v);
    void onGameStarted();

    void setOctConf(int bot, int top);

protected:
    // widget visibility states
    bool _menuVisible;
    bool _rootNoteSetterVisible;
    bool _ScaleSwitchVisible;
    bool _octaveRangerVisible;
    bool _playAreaVisible;
    bool _tuneAreaVisible;
    bool _synthAreaVisible;
    bool _confAreaVisible;
    bool _presetsVisible;
    bool _scalePresetsVisible;
    bool _synthPresetsVisible;
    bool _tunePresetsVisible;

    // menu
    ToggleHeader    * _rootButton;
    ToggleHeader    * _scaleButton;
    ToggleHeader    * _octaveButton;
    SetMainView     * _playAreaButton;
    SetMainView     * _tuneAreaButton;
    SetMainView     * _synthAreaButton;
    SetMainView     * _confAreaButton;

    // synth ctl faders
    QList<QObject*> _faderParamCtl;

    // microtuning faders
    QList<QObject*> _faderMicrotune;

    // faders
    MWFaderParamCtl * faderPitchTopRange;
    MWFaderParamCtl * faderPitchBottomRange;
    BendHorizontal  * pitchHorizontal;
    MWFaderParamCtl * faderChannel;
    MWFaderParamCtl * faderSymbols;

    // config buttons
    SendCc1         * _sendCc1;
    ToggleSender    * _enableMobilesynth;
    ToggleSender    * _enableReaktor;
    ToggleSender    * _enablePuredata;
    ToggleSender    * _enableSupercollider;
    ToggleBw        * _bwMode;
    ShowFreqs       * _showFreqs;

    // preset buttons
    PresetCollection * _scalePresets;
    PresetCollection * _synthPresets;
    PresetCollection * _tunePresets;

    // channel
    int _channel;

private:
    XmlLoader*  _xmlLoader;
    GameControl*     _game;
    Heartbeat*  _heartbeat;

    MasterSender*           _out;
    SenderOscMidiGeneric*   _senderOscMidiGeneric;
    SenderReaktor*          _senderReaktor;

    QList<QObject*> _menu;
    QList<QObject*> _pitchColors;
    QList<QObject*> _rootNoteSetter;
    QList<QObject*> _scaleSwitch;
    OctaveRanger* _OctaveRanger;
    PlayArea*     _PlayArea;

    OpenArchive*        _openArchive;
    TogglePresets*      _togglePresets;
    ToggleMenu*         _toggleMenu;
    OctaveShift*        _octUp;
    OctaveShift*        _octDown;

    // layout
    int _botOct;
    int _topOct;

    void recalcMainView();

};

#endif // WLAYOUT_H

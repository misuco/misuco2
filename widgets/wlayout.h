#ifndef WLAYOUT_H
#define WLAYOUT_H

#include <QWidget>
#include <QGridLayout>
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

class wlayout : public QWidget
{
    Q_OBJECT
public:
    explicit wlayout(QWidget *parent = 0);
    ~wlayout();

    Pitch * MWPitch[BSCALE_SIZE+1];

protected:
    void resizeEvent(QResizeEvent *);

signals:
    void setBaseNote(Pitch * p);
    void initialSet();
    void scaleUpdate();

private slots:    
    void currentHeader(int i);
    void currentMainView(int i);
    void changePitch(int v);
    void togglePresets();
    void toggleMenu();
    void toggleBW();
    void onSetBaseNote(Pitch * p);
    void setSound(MWSound * s);
    void onScaleUpdate();
    void onSoundSustainUpdate(int);
    void onChannelChange(int v);
    void onToggleSender(int v);
    void onSymbolsChange(int v);
    void onShowFreqsChange();

private:
    QGridLayout *layout;
    SenderMulti * out;

    MWBaseNoteSetter * BaseNoteSetter[BSCALE_SIZE+1];
    MWBScaleSwitch * bScaleSwitch[BSCALE_SIZE+1];
    MWOctaveRanger * OctaveRanger;
    QStackedWidget * mainArea;

    // synth ctl faders
    MWFaderParamCtl * faderParamCtl[10];

    // header widgets
    QWidget * H[3];

    // main area widgets
    QWidget * M[4];

    // right menu
    QWidget * HS[15];

    // preset buttons
    static const int nPresetBtn = 5;
    static const int nSoundPresetBtn = 2;
    QWidget * PB[nPresetBtn + nSoundPresetBtn];

    MWFaderParamCtl * faderPitchTopRange;
    MWFaderParamCtl * faderPitchBottomRange;
    MWFaderParamCtl * faderChannel;
    MWHeaderSetter * pitchVert;
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

    void recalcMainView();
};

#endif // WLAYOUT_H

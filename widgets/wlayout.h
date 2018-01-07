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

private:
    QGridLayout *layout;

    MWBaseNoteSetter * BaseNoteSetter[BSCALE_SIZE+1];
    MWBScaleSwitch * bScaleSwitch[BSCALE_SIZE+1];
    MWOctaveRanger * OctaveRanger;
    QStackedWidget * mainArea;

    // synth ctl faders
    MWFaderParamCtl * faderParamCtl[10];

    // header widgets
    QWidget * H[3];

    // main area widgets
    QWidget * M[3];

    // right menu
    QWidget * HS[15];

    // preset buttons
    QWidget * PB[15];

    void recalcMainView();
};

#endif // WLAYOUT_H

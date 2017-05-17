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

class wlayout : public QWidget
{
    Q_OBJECT
public:
    explicit wlayout(QWidget *parent = 0);
    ~wlayout();

    Pitch * MWPitch[BSCALE_SIZE+1];

protected:
    void resizeEvent(QResizeEvent *E);

private slots:    
    void currentHeader(int i);
    void currentMainView(int i);
    void changePitch(int v);

private:
    QGridLayout *layout;
    //MWPlayArea * M[0];
    MWBaseNoteSetter * BaseNoteSetter[BSCALE_SIZE+1];
    MWOctaveRanger * OctaveRanger;
    //QStackedWidget * header;
    QStackedWidget * mainArea;

    // current header widget
    // QWidget * header;

    // header widgets
    QWidget * H[3];

    // main area widgets
    QWidget * M[2];

};

#endif // WLAYOUT_H

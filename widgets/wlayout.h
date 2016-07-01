#ifndef WLAYOUT_H
#define WLAYOUT_H

#include <QWidget>
#include <QGridLayout>
#include "widgets/mwplayarea.h"
#include "widgets/mwbasenotesetter.h"
#include "comm/sendermobilesynth.h"
#include "widgets/mwoctaveranger.h"
#include "conf/types.h"

class wlayout : public QWidget
{
    Q_OBJECT
public:
    explicit wlayout(QWidget *parent = 0);
    ~wlayout();

protected:
    void resizeEvent(QResizeEvent *E);

private:
    QGridLayout *layout;
    MWPlayArea *PlayArea;
    MWBaseNoteSetter * BaseNoteSetter[BSCALE_SIZE+1];
    MWOctaveRanger * OctaveRanger;
};

#endif // WLAYOUT_H

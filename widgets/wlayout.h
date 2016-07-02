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

private slots:    
    currentHeader(int i);

private:
    QGridLayout *layout;
    MWPlayArea *PlayArea;
    MWBaseNoteSetter * BaseNoteSetter[BSCALE_SIZE+1];
    MWOctaveRanger * OctaveRanger;

    // current header widget
    QWidget * header;

    // header widgets
    QWidget * H[3];
};

#endif // WLAYOUT_H

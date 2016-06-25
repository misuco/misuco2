#ifndef WLAYOUT_H
#define WLAYOUT_H

#include <QWidget>
#include <QGridLayout>
#include "widgets/mwplayarea.h"
#include "widgets/mwbasenotesetter.h"
#include "comm/sendermobilesynth.h"
#include "widgets/mwoctaveranger.h"

class MWPlayArea;

class wlayout : public QWidget
{
    Q_OBJECT
public:
    explicit wlayout(QWidget *parent = 0);
    ~wlayout();
    FreqTriple * getNote(int i);

private:
    QGridLayout *layout;
    MWPlayArea *PlayArea;
    MWBaseNoteSetter * BaseNoteSetter[];
    MWOctaveRanger * OctaveRanger;
    FreqTriple * note[128];
};

#endif // WLAYOUT_H

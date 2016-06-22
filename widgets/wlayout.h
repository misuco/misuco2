#ifndef WLAYOUT_H
#define WLAYOUT_H

#include <QWidget>
#include <QGridLayout>
#include "widgets/mwplayarea.h"
#include "widgets/mwbasenotesetter.h"
#include "comm/sendermobilesynth.h"
#include "widgets/mwoctaveranger.h"

class wlayout : public QWidget
{
    Q_OBJECT
public:
    explicit wlayout(QWidget *parent = 0);
    ~wlayout();

signals:

public slots:

private:
    QGridLayout *layout;
    QWidget * widgets[160];
    MWPlayArea PlayArea;
    MWBaseNoteSetter * BaseNoteSetter[];
    MWOctaveRanger * OctaveRanger;
};

#endif // WLAYOUT_H

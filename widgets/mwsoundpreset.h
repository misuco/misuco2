#ifndef MWSound_H
#define MWSound_H

#include "misuwidget.h"
#include "conf/types.h"

class MWSoundPreset : public MisuWidget
{
    Q_OBJECT

public:
    MWSoundPreset(QWidget *parent);
    virtual void processTouchEvent(misuTouchEvent e);

protected:
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *);

signals:
    void setSound(MWSound * s);

private:
    MWSound PresetSound;
    int pressed=0;
};

#endif // MWSound_H

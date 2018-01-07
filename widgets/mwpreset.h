#ifndef MWPRESET_H
#define MWPRESET_H

#include "misuwidget.h"
#include "conf/types.h"

class MWPreset : public MisuWidget
{
    Q_OBJECT

public:
    MWPreset(Pitch * p[], QWidget *parent);
    virtual void processTouchEvent(misuTouchEvent e);
    MWScale PresetScale;

protected:
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *);

public slots:
    void initialSet();

signals:
    void setScale(MWScale *);
    void scaleUpdate();

private:
    Pitch ** MWPitch;
    int pressed=0;
    void setBrush(Pitch *p, int s, int l, QPainter &painter);
    bool isSelected();
};

#endif // MWPRESET_H

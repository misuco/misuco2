#ifndef MWFADDER_H
#define MWFADDER_H

#include "misuwidget.h"
#include "conf/color.h"

class MWFadder : public MisuWidget
{
    Q_OBJECT

public:
    MWFadder(QWidget *parent, Color * c);
    enum orientation {
        horizontal,
        vertical
    };
    enum FadeMode {
        coarse,
        fine
    };

    void setMinValue(int value);
    void setMaxValue(int value);

protected:
    void paintEvent(QPaintEvent *);
    void processTouchEvent(misuTouchEvent e);
    void resizeEvent(QResizeEvent *);

signals:
    void valueChange(int v);

private:
    // Fadder state
    int value;
    int minValue;
    int maxValue;
    int valRange;
    int step1;
    int step2;
    int pressed;
    int orient;
    int fadderY;
    int knobSize;
    int fineness;
    Color * color;

    // Touch event state
    int xTouchBegin;
    int yTouchBegin;
    int valTouchBegin;
    int fadeMode;

    void calcGeo();
};

#endif // MWFADDER_H

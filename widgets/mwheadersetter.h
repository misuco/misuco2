#ifndef MWHEADERSETTER_H
#define MWHEADERSETTER_H

#include "misuwidget.h"

class MWHeaderSetter : public MisuWidget
{
    Q_OBJECT

public:
    MWHeaderSetter(int headerId, QWidget *parent);
    virtual void processTouchEvent(misuTouchEvent e);

protected:
    void paintEvent(QPaintEvent *E);
    void resizeEvent(QResizeEvent *E);

signals:
    void currentHeader(int i);

private:
    int headerId;
    int pressed;
};

#endif // MWHEADERSETTER_H

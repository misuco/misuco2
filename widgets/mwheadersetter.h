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
    void currentMainView(int i);
    void setBendHori(bool b);
    void setBendVertTop(int b);
    void setBendVertBot(int b);
    void togglePresets();
    void toggleMenu();

private:
    int headerId;
    int pressed;
    int state;
};

#endif // MWHEADERSETTER_H

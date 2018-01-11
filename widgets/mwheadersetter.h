#ifndef MWHEADERSETTER_H
#define MWHEADERSETTER_H

#include "misuwidget.h"

class MWHeaderSetter : public MisuWidget
{
    Q_OBJECT

public:
    MWHeaderSetter(int headerId, QWidget *parent);
    MWHeaderSetter(int headerId, int state, QWidget *parent);
    virtual void processTouchEvent(misuTouchEvent e);

protected:
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *);

signals:
    void currentHeader(int i);
    void currentMainView(int i);
    void setBendHori(bool b);
    void setBendVertTop(int b);
    void setBendVertBot(int b);
    void togglePresets();
    void toggleMenu();
    void toggleBW();
    void scaleUpdate();
    void toggleSender(int i);

private:
    int headerId;
    int pressed;
    int state;
    QString midi2TextUrl(int midinote);

};

#endif // MWHEADERSETTER_H

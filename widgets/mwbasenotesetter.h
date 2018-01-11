#ifndef MWBASENOTESETTER_H
#define MWBASENOTESETTER_H

#include "misuwidget.h"
#include "comm/isender.h"
#include "conf/freqtriple.h"

class MWBaseNoteSetter : public MisuWidget
{
    Q_OBJECT

public:
    MWBaseNoteSetter(Pitch *pitch, QWidget *parent);
    ~MWBaseNoteSetter();
    virtual void processTouchEvent(misuTouchEvent e);
    void setOut(ISender *value);

protected:
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *);

public slots:
    void setOctMid(int o);
    void pitchChange();
    void onSetBaseNote(Pitch *pitch);
    void onScaleSet(MWScale*scale);
    void onScaleUpdate();


signals:
    void setBaseNote(Pitch *);
    void scaleUpdate();

private:
    ISender * out;
    Pitch * p;
    FreqTriple * f;
    int vId;
    int pressed;
    bool selected;
};

#endif // MWBASENOTESETTER_H

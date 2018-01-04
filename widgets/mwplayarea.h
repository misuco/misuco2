#ifndef MWPLAYAREA_H
#define MWPLAYAREA_H

#include <QPainter>
#include "misuwidget.h"
#include "conf/types.h"
#include "comm/isender.h"

#define MAX_COLS 255
#define MAX_ROWS 3
#define EVENT_STACK_SIZE 64


class MWPlayArea : public MisuWidget
{
    Q_OBJECT

public:
    struct eventStackElement {
        int eventId;
        int voiceId;
        int midinote;
        int row;
        int col;
        int x;
        int y;
        float f;
    };

    MWPlayArea(Pitch * p[], QWidget *parent);
    ~MWPlayArea();
    virtual void processTouchEvent(misuTouchEvent e);

    void setOut(ISender *value);

protected:
    void paintEvent(QPaintEvent *E);
    void resizeEvent(QResizeEvent *E);

public slots:
    void setBaseNote(Pitch * p);
    void setOctConf(int bottom, int top);
    void setBscale(int n, bool v);
    void setScale(MWScale *s);
    void setBendHori(bool b);
    void setBendVertTop(int b);
    void setBendVertBot(int b);
    void pitchChange();    
    void toggleBW();

signals:
    void menuTouch();

private:
    // INFRASTRUCTURE
    // - painting
    QLinearGradient linearGrad;
    QString cap;
    QFont font3;
    QFont font8;
    // - networking
    ISender * out;
    // - processing
    FreqTriple fcalc;
    Pitch pcalc;

    // WORKING MEMORY
    // - touch field configuration
    Pitch ** MWPitch;
    MWPlayfield fields[MAX_ROWS][MAX_COLS];
    int rows;
    int cols;
    // - event stack/hashmap
    eventStackElement eventStack[EVENT_STACK_SIZE];
    int chan = 0;

    // CONFIGURATION
    // - scale
    MWScale Scale;
    // - bending
    bool bendHoriz;
    int bendVertTop;
    int bendVertBot;
    // - visual
    int colwidth[MAX_COLS];
    int rowheight[MAX_ROWS];

    // - menu button area
    int menux1;
    int menux2;
    int menuy1;
    int menuy2;

    // helper functions
    void config();
    void setColumn(int col, int midinote, int basenote);
    void calcGeo();
    void paintField(int r, int c, int x, int y);
    void paintField(int r, int c);
};

#endif // MWPLAYAREA_H

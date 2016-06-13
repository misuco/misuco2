#ifndef MWPLAYAREA_H
#define MWPLAYAREA_H

#include <QPainter>
#include "misuwidget.h"
#include "conf/types.h"

#define MAX_COLS 255
#define MAX_ROWS 3
#define EVENT_STACK_SIZE 64

class MWPlayArea : public MisuWidget
{

public:
    MWPlayArea(QWidget *parent);
    ~MWPlayArea();
    void config();

protected:
    void paintEvent(QPaintEvent *E);
    void resizeEvent(QResizeEvent *E);

public slots:
    void processTouchEvent(misuTouchEvent e);

private:
    // painting infrastructure
    // QPainter painter;
    QLinearGradient linearGrad;
    QString cap;

    // working memory
    MWPlayfield fields[MAX_ROWS][MAX_COLS];
    int rows;
    int cols;

    // event stack/hashmap
    eventStackElement eventStack[EVENT_STACK_SIZE];

    // configuration
    MWScale Scale;
    bool bendHoriz;
    int bendVertTop;
    int bendVertBot;

    // visual properties
    int colwidth[MAX_COLS];
    int rowheight[MAX_ROWS];

    void setColumn(int col, int midinote);
    void calcGeo();
    void paintField(int r, int c, int x, int y);
    void paintField(int r, int c);
};

#endif // MWPLAYAREA_H

#ifndef MWPLAYAREA_H
#define MWPLAYAREA_H

#include "misuwidget.h"
#include "conf/types.h"

#define MAX_COLS 255
#define MAX_ROWS 3

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
    // working memory
    MWPlayfield fields[MAX_ROWS][MAX_COLS];
    int rows;
    int cols;

    // event stack/hashmap


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
};

#endif // MWPLAYAREA_H

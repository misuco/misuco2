#ifndef MISUWIDGET_H
#define MISUWIDGET_H

#include <QWidget>
#include "conf/types.h"

class MisuWidget : public QWidget
{    
    Q_OBJECT

public:
    struct misuTouchEvent {
        int id;
        int x;
        int y;
        int state;
        long t;
    };

    explicit MisuWidget(QWidget *parent = 0);
    ~MisuWidget();

    virtual void processTouchEvent(misuTouchEvent e)=0;

    static QFont font1;
    static QColor fgcolor;
    static QColor bgcolor;
    static QColor highlightcolor;

    static bool bwmode;
    static QColor bkeycolor;
    static QColor wkeycolor;
    static QColor hlkeycolor;

    static int lOff;
    static int lOn;
    static int sOff;
    static int sOn;
    static MWScale Scale;
    static bool overwrite;

protected:
    bool event(QEvent *event);
    int id;

};

#endif // MISUWIDGET_H

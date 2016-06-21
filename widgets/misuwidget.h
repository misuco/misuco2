#ifndef MISUWIDGET_H
#define MISUWIDGET_H

#include <QWidget>

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

protected:
    bool event(QEvent *event);
    int id;

};

#endif // MISUWIDGET_H

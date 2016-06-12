#ifndef MISUWIDGET_H
#define MISUWIDGET_H

#include <QWidget>

class MisuWidget : public QWidget
{

    struct point {
        int id;
        int x;
        int y;
        int state;
        long t;
    };

    Q_OBJECT
public:
    explicit MisuWidget(QWidget *parent = 0);
    ~MisuWidget();

protected:
    bool event(QEvent *event);
    int id;

signals:
    void touchEvent(int,int,int,int,long);

public slots:
    virtual void processTouchEvent(int id, int state, int x, int y, long t)=0;

};

#endif // MISUWIDGET_H

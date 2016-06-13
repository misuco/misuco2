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

    struct eventStackElement {
        int eventId;
        int row;
        int col;
        float f;
    };

    explicit MisuWidget(QWidget *parent = 0);
    ~MisuWidget();

protected:
    bool event(QEvent *event);
    int id;

signals:
    void touchEvent(misuTouchEvent e);

public slots:
    virtual void processTouchEvent(misuTouchEvent e)=0;
};

#endif // MISUWIDGET_H

#ifndef WNOTE_H
#define WNOTE_H

#include "misuwidget.h"

class wNote : public MisuWidget
{
    Q_OBJECT
public:
    explicit wNote(QWidget *parent = 0);
    ~wNote();
    QColor color;

protected:
    void paintEvent(QPaintEvent *);

signals:

public slots:
    void processTouchEvent(misuTouchEvent e);

private:
    QString cap;
};

#endif // WNOTE_H

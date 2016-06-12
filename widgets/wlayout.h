#ifndef WLAYOUT_H
#define WLAYOUT_H

#include <QWidget>
#include <QGridLayout>

class wlayout : public QWidget
{
    Q_OBJECT
public:
    explicit wlayout(QWidget *parent = 0);
    ~wlayout();

signals:

public slots:

private:
    QGridLayout *layout;
    QWidget * widgets[160];

};

#endif // WLAYOUT_H

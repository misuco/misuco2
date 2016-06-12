#include "mainwindow.h"
#include "widgets/wlayout.h"
#include <QWidget>
#include <QGridLayout>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    w=new wlayout(this);
    setCentralWidget(w);
    //startTimer(1000);
}

MainWindow::~MainWindow()
{

}

void MainWindow::timerEvent(QTimerEvent *e)
{
//    w->update();
    qDebug() << "MeinWindow::timerEvent";
}

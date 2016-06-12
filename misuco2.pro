#-------------------------------------------------
#
# Project created by QtCreator 2016-05-14T21:51:07
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = misuco2
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    widgets/wnote.cpp \
    widgets/wlayout.cpp \
    widgets/misuwidget.cpp \
    widgets/mwplayarea.cpp \
    conf/freqtriple.cpp

HEADERS  += mainwindow.h \
    widgets/wnote.h \
    widgets/wlayout.h \
    widgets/misuwidget.h \
    widgets/mwplayarea.h \
    conf/types.h \
    conf/freqtriple.h

CONFIG += mobility
MOBILITY = 


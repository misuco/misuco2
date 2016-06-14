#-------------------------------------------------
#
# Project created by QtCreator 2016-05-14T21:51:07
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = misuco2
TEMPLATE = app

INCLUDEPATH += comm
INCLUDEPATH += comm/libofqf

SOURCES += main.cpp\
        mainwindow.cpp \
    widgets/wnote.cpp \
    widgets/wlayout.cpp \
    widgets/misuwidget.cpp \
    widgets/mwplayarea.cpp \
    conf/freqtriple.cpp \
    comm/libofqf/qoscclient.cpp \
    comm/libofqf/qosctypes.cpp \
    comm/libofqf/qosctcpserverthread.cpp \
    comm/libofqf/qosctcpserver.cpp \
    comm/libofqf/qosctcpclient.cpp \
    comm/libofqf/qoscserver.cpp \
    comm/libofqf/qoscproxyinterface.cpp \
    comm/libofqf/qoscproxy.cpp \
    comm/libofqf/qoscmsgparser.cpp \
    comm/senderdebug.cpp

HEADERS  += mainwindow.h \
    widgets/wnote.h \
    widgets/wlayout.h \
    widgets/misuwidget.h \
    widgets/mwplayarea.h \
    conf/types.h \
    conf/freqtriple.h \
    comm/libofqf/qoscclient.h \
    comm/libofqf/qosctypes.h \
    comm/libofqf/qosctcpserverthread.h \
    comm/libofqf/qosctcpserver.h \
    comm/libofqf/qosctcpclient.h \
    comm/libofqf/qoscserverinterface.h \
    comm/libofqf/qoscserver.h \
    comm/libofqf/qoscproxyinterface.h \
    comm/libofqf/qoscproxy.h \
    comm/libofqf/qoscmsgparser.h \
    comm/libofqf/qoscclientinterface.h \
    comm/senderdebug.h \
    comm/isender.h

CONFIG += mobility
MOBILITY = 


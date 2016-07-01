#-------------------------------------------------
#
# Project created by QtCreator 2016-05-14T21:51:07
#
#-------------------------------------------------

QT       += core gui network multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = misuco2
TEMPLATE = app

INCLUDEPATH += conf
INCLUDEPATH += comm
INCLUDEPATH += comm/libofqf
INCLUDEPATH += mobilesynth/mobilesynth/Classes/synth
INCLUDEPATH += mobilesynth/mobilesynth/Classes
INCLUDEPATH += mobilesynth/lib

SOURCES += main.cpp\
        mainwindow.cpp \
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
    comm/senderdebug.cpp \
    mobilesynth/mobilesynth/Classes/synth/arpeggio.cpp \
    mobilesynth/mobilesynth/Classes/synth/controller.cpp \
    mobilesynth/mobilesynth/Classes/synth/envelope.cpp \
    mobilesynth/mobilesynth/Classes/synth/filter.cpp \
    mobilesynth/mobilesynth/Classes/synth/key_stack.cpp \
    mobilesynth/mobilesynth/Classes/synth/lag_processor.cpp \
    mobilesynth/mobilesynth/Classes/synth/modulation.cpp \
    mobilesynth/mobilesynth/Classes/synth/oscillator.cpp \
    mobilesynth/mobilesynth/Classes/synth/parameter.cpp \
    comm/sendermobilesynth.cpp \
    mobilesynth/mobilesynth/Classes/mobilesynthqt52.cpp \
    proc/noteproc.cpp \
    widgets/mwbasenotesetter.cpp \
    widgets/mwoctaveranger.cpp \
    widgets/mwbscaleswitch.cpp

HEADERS  += mainwindow.h \
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
    comm/isender.h \
    mobilesynth/mobilesynth/Classes/AudioOutput.h \
    mobilesynth/mobilesynth/Classes/mobilesynthViewControllerRc1.h \
    mobilesynth/mobilesynth/Classes/synth/arpeggio.h \
    mobilesynth/mobilesynth/Classes/synth/controller.h \
    mobilesynth/mobilesynth/Classes/synth/envelope.h \
    mobilesynth/mobilesynth/Classes/synth/filter.h \
    mobilesynth/mobilesynth/Classes/synth/key_stack.h \
    mobilesynth/mobilesynth/Classes/synth/lag_processor.h \
    mobilesynth/mobilesynth/Classes/synth/modulation.h \
    mobilesynth/mobilesynth/Classes/synth/oscillator.h \
    mobilesynth/mobilesynth/Classes/synth/parameter.h \
    mobilesynth/mobilesynth/Classes/synth/test_util.h \
    comm/sendermobilesynth.h \
    mobilesynth/mobilesynth/Classes/mobilesynthviewcontrollerrc1.hpp \
    mobilesynth/mobilesynth/Classes/mobilesynthqt52.h \
    proc/noteproc.h \
    widgets/mwbasenotesetter.h \
    widgets/mwoctaveranger.h \
    widgets/mwbscaleswitch.h \
    conf/platform.h

CONFIG += mobility
MOBILITY = 


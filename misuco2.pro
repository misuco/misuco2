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
    widgets/mwbasenotesetter.cpp \
    widgets/mwoctaveranger.cpp \
    widgets/mwbscaleswitch.cpp \
    widgets/mwheadersetter.cpp \
    widgets/mwfadder.cpp \
    conf/pitch.cpp \
    widgets/mwpreset.cpp \
    conf/color.cpp \
    widgets/mwfaderpitch.cpp \
    widgets/mwfaderparamctl.cpp \
    widgets/mwsoundpreset.cpp \
    comm/sendermulti.cpp \
    comm/senderoscmidigeneric.cpp \
    comm/senderoscpuredata.cpp \
    comm/senderoscxy.cpp \
    comm/senderreaktor.cpp \
    comm/sendersupercollider.cpp \
    widgets/mwmicrotunepreset.cpp

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
    widgets/mwbasenotesetter.h \
    widgets/mwoctaveranger.h \
    widgets/mwbscaleswitch.h \
    conf/platform.h \
    widgets/mwheadersetter.h \
    widgets/mwfadder.h \
    conf/pitch.h \
    widgets/mwpreset.h \
    conf/color.h \
    widgets/mwfaderpitch.h \
    widgets/mwfaderparamctl.h \
    widgets/mwsoundpreset.h \
    comm/sendermulti.h \
    comm/senderoscmidigeneric.h \
    comm/senderoscpuredata.h \
    comm/senderoscxy.h \
    comm/sendersupercollider.h \
    comm/senderreaktor.h \
    widgets/mwmicrotunepreset.h \
    conf/initxml.h

CONFIG += mobility
MOBILITY = 

DISTFILES += \
    COPYING \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat \
    ios/Images.xcassets/AppIcon.appiconset/Contents.json \
    ios/Images.xcassets/Brand Assets.launchimage/Contents.json \
    ios/Info.plist \
    ios/Images.xcassets/AppIcon.appiconset/Icon-60@2x.png \
    ios/Images.xcassets/AppIcon.appiconset/Icon-60@3x.png \
    ios/Images.xcassets/AppIcon.appiconset/Icon-72.png \
    ios/Images.xcassets/AppIcon.appiconset/Icon-72@2x.png \
    ios/Images.xcassets/AppIcon.appiconset/Icon-76.png \
    ios/Images.xcassets/AppIcon.appiconset/Icon-76@2x.png \
    ios/Images.xcassets/AppIcon.appiconset/Icon-83.5@2x.png \
    ios/Images.xcassets/AppIcon.appiconset/Icon-Small-1.png \
    ios/Images.xcassets/AppIcon.appiconset/Icon-Small-40.png \
    ios/Images.xcassets/AppIcon.appiconset/Icon-Small-40@2x-1.png \
    ios/Images.xcassets/AppIcon.appiconset/Icon-Small-40@2x.png \
    ios/Images.xcassets/AppIcon.appiconset/Icon-Small-50.png \
    ios/Images.xcassets/AppIcon.appiconset/Icon-Small-50@2x.png \
    ios/Images.xcassets/AppIcon.appiconset/Icon-Small.png \
    ios/Images.xcassets/AppIcon.appiconset/Icon-Small@2x.png \
    ios/Images.xcassets/AppIcon.appiconset/Icon-Small@3x.png \
    ios/Images.xcassets/AppIcon.appiconset/Icon.png \
    ios/Images.xcassets/AppIcon.appiconset/Icon@2x-1.png \
    ios/Images.xcassets/AppIcon.appiconset/Icon@2x.png \
    ios/Images.xcassets/Brand Assets.launchimage/Default-568h@2x.png \
    ios/Images.xcassets/Brand Assets.launchimage/iTunesArtwork.png \
    ios/Images.xcassets/Brand Assets.launchimage/iTunesArtwork@2x.png

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

ios {
    QMAKE_INFO_PLIST = ios/Info.plist

    QMAKE_ASSET_CATALOGS += ios/Images.xcassets

}

SUBDIRS += \
    misuco2.pro

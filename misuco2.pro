#-------------------------------------------------
#
# __/\\\\____________/\\\\__/\\\\\\\\\\\_____/\\\\\\\\\\\____/\\\________/\\\________/\\\\\\\\\_______/\\\\\_________/\\\\\\\\\_____
#  _\/\\\\\\________/\\\\\\_\/////\\\///____/\\\/////////\\\_\/\\\_______\/\\\_____/\\\////////______/\\\///\\\_____/\\\///////\\\___
#   _\/\\\//\\\____/\\\//\\\_____\/\\\______\//\\\______\///__\/\\\_______\/\\\___/\\\/_____________/\\\/__\///\\\__\///______\//\\\__
#    _\/\\\\///\\\/\\\/_\/\\\_____\/\\\_______\////\\\_________\/\\\_______\/\\\__/\\\______________/\\\______\//\\\___________/\\\/___
#     _\/\\\__\///\\\/___\/\\\_____\/\\\__________\////\\\______\/\\\_______\/\\\_\/\\\_____________\/\\\_______\/\\\________/\\\//_____
#      _\/\\\____\///_____\/\\\_____\/\\\_____________\////\\\___\/\\\_______\/\\\_\//\\\____________\//\\\______/\\\______/\\\//________
#       _\/\\\_____________\/\\\_____\/\\\______/\\\______\//\\\__\//\\\______/\\\___\///\\\___________\///\\\__/\\\______/\\\/___________
#        _\/\\\_____________\/\\\__/\\\\\\\\\\\_\///\\\\\\\\\\\/____\///\\\\\\\\\/______\////\\\\\\\\\____\///\\\\\/______/\\\\\\\\\\\\\\\_
#         _\///______________\///__\///////////____\///////////________\/////////___________\/////////_______\/////_______\///////////////__
#
# Project created by c1Audio 2016-05-14T21:51:07
#
#--------------------------------------------------------------------------------------------------------------------------------------------

QT       += core network multimedia quick purchasing
CONFIG   += c++11
DEFINES  += QT_DEPRECATED_WARNINGS

TARGET = misuco2
TEMPLATE = app

INCLUDEPATH += conf
INCLUDEPATH += comm
INCLUDEPATH += comm/libofqf
INCLUDEPATH += mobilesynth/synth
INCLUDEPATH += mobilesynth/mobilesynth/Classes
INCLUDEPATH += mobilesynth/lib


SOURCES += main.cpp\
    app/misuco2.cpp \
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
    mobilesynth/synth/controller.cpp \
    mobilesynth/synth/envelope.cpp \
    mobilesynth/synth/filter.cpp \
    mobilesynth/synth/key_stack.cpp \
    mobilesynth/synth/lag_processor.cpp \
    mobilesynth/synth/modulation.cpp \
    mobilesynth/synth/oscillator.cpp \
    mobilesynth/synth/parameter.cpp \
    comm/sendermobilesynth.cpp \
    mobilesynth/mobilesynthqt52.cpp \
    widgets/mwrootnotesetter.cpp \
    widgets/mwoctaveranger.cpp \
    widgets/mwbscaleswitch.cpp \
    widgets/mwheadersetter.cpp \
    widgets/mwfadder.cpp \
    conf/pitch.cpp \
    widgets/mwfaderpitch.cpp \
    widgets/mwfaderparamctl.cpp \
    widgets/presets/mwsoundpreset.cpp \
    comm/senderoscmidigeneric.cpp \
    comm/senderoscpuredata.cpp \
    comm/senderreaktor.cpp \
    comm/sendersupercollider.cpp \
    widgets/presets/mwmicrotunepreset.cpp \
    conf/mwplayfield.cpp \
    widgets/mwgame.cpp \
    conf/mglob.cpp \
    widgets/presets/mwscalepreset.cpp \
    widgets/presets/presetcollection.cpp \
    conf/purchases.cpp \
    comm/mastersender.cpp

HEADERS  += \
    app/misuco2.h \
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
    mobilesynth/AudioOutput.h \
    mobilesynth/mobilesynthViewControllerRc1.h \
    mobilesynth/synth/controller.h \
    mobilesynth/synth/envelope.h \
    mobilesynth/synth/filter.h \
    mobilesynth/synth/key_stack.h \
    mobilesynth/synth/lag_processor.h \
    mobilesynth/synth/modulation.h \
    mobilesynth/synth/oscillator.h \
    mobilesynth/synth/parameter.h \
    mobilesynth/synth/test_util.h \
    comm/sendermobilesynth.h \
    mobilesynth/mobilesynthviewcontrollerrc1.hpp \
    mobilesynth/mobilesynthqt52.h \
    widgets/mwrootnotesetter.h \
    widgets/mwoctaveranger.h \
    widgets/mwbscaleswitch.h \
    widgets/mwheadersetter.h \
    widgets/mwfadder.h \
    conf/pitch.h \
    widgets/mwfaderpitch.h \
    widgets/mwfaderparamctl.h \
    widgets/presets/mwsoundpreset.h \
    comm/senderoscmidigeneric.h \
    comm/senderoscpuredata.h \
    comm/sendersupercollider.h \
    comm/senderreaktor.h \
    widgets/presets/mwmicrotunepreset.h \
    conf/mwplayfield.h \
    widgets/mwgame.h \
    conf/mglob.h \
    widgets/presets/mwscalepreset.h \
    widgets/presets/presetcollection.h \
    conf/purchases.h \
    comm/mastersender.h

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
    ios/Images.xcassets/Brand Assets.launchimage/iTunesArtwork@2x.png \

OBJECTIVE_SOURCES += \
    mobilesynth/mobilesynthViewControllerRc1.mm \
    mobilesynth/AudioOutput.m \

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

ios {
    QT               = core network quick purchasing
    QMAKE_INFO_PLIST = ios/Info.plist
    QMAKE_ASSET_CATALOGS += ios/Images.xcassets

}

SUBDIRS += \
    misuco2.pro

RESOURCES += \
    qml.qrc

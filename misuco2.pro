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

SOURCES += main.cpp\
    app/misuco2.cpp \
    conf/freqtriple.cpp \
    comm/libofqf/qoscclient.cpp \
    comm/libofqf/qosctypes.cpp \
    comm/libofqf/qosctcpclient.cpp \
    comm/libofqf/qoscserver.cpp \
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
    widgets/faders/mwfaderpitch.cpp \
    widgets/faders/mwfaderparamctl.cpp \
    widgets/presets/mwsoundpreset.cpp \
    comm/senderoscmidigeneric.cpp \
    comm/senderoscpuredata.cpp \
    comm/senderreaktor.cpp \
    comm/sendersupercollider.cpp \
    widgets/presets/mwmicrotunepreset.cpp \
    widgets/presets/mwscalepreset.cpp \
    widgets/presets/presetcollection.cpp \
    conf/purchases.cpp \
    comm/mastersender.cpp \
    app/heartbeat.cpp \
    widgets/buttons/buttonbase.cpp \
    widgets/buttons/openarchive.cpp \
    conf/pitchcolor.cpp \
    widgets/faders/mwfader.cpp \
    app/xmlloader.cpp \
    widgets/buttons/bendhorizontal.cpp \
    widgets/buttons/sendcc1.cpp \
    widgets/buttons/togglesender.cpp \
    widgets/buttons/togglebw.cpp \
    widgets/buttons/showfreqs.cpp \
    widgets/core/scaleswitch.cpp \
    widgets/buttons/toggleheader.cpp \
    widgets/buttons/setmainview.cpp \
    widgets/buttons/togglemenu.cpp \
    widgets/buttons/togglepresets.cpp \
    widgets/buttons/octaveshift.cpp \
    conf/playfield.cpp \
    widgets/core/gamecontrol.cpp \
    widgets/core/octaveranger.cpp \
    widgets/core/rootnotesetter.cpp \
    widgets/core/playarea.cpp

HEADERS  += \
    app/misuco2.h \
    conf/types.h \
    conf/freqtriple.h \
    comm/libofqf/qoscclient.h \
    comm/libofqf/qosctypes.h \
    comm/libofqf/qosctcpclient.h \
    comm/libofqf/qoscserverinterface.h \
    comm/libofqf/qoscserver.h \
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
    widgets/faders/mwfaderpitch.h \
    widgets/faders/mwfaderparamctl.h \
    widgets/presets/mwsoundpreset.h \
    comm/senderoscmidigeneric.h \
    comm/senderoscpuredata.h \
    comm/sendersupercollider.h \
    comm/senderreaktor.h \
    widgets/presets/mwmicrotunepreset.h \
    widgets/presets/mwscalepreset.h \
    widgets/presets/presetcollection.h \
    conf/purchases.h \
    comm/mastersender.h \
    app/heartbeat.h \
    widgets/buttons/buttonbase.h \
    widgets/buttons/openarchive.h \
    conf/pitchcolor.h \
    conf/colormacros.h \
    widgets/faders/mwfader.h \
    app/xmlloader.h \
    widgets/buttons/bendhorizontal.h \
    widgets/buttons/sendcc1.h \
    widgets/buttons/togglesender.h \
    widgets/buttons/togglebw.h \
    widgets/buttons/showfreqs.h \
    widgets/core/scaleswitch.h \
    widgets/buttons/toggleheader.h \
    widgets/buttons/setmainview.h \
    widgets/buttons/togglemenu.h \
    widgets/buttons/togglepresets.h \
    widgets/buttons/octaveshift.h \
    conf/playfield.h \
    widgets/core/gamecontrol.h \
    widgets/core/octaveranger.h \
    widgets/core/rootnotesetter.h \
    widgets/core/playarea.h

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

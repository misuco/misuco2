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

INCLUDEPATH += lib/qmidi/src

SOURCES += main.cpp\
    app/misuco2.cpp \
    lib/misulib/models/freqtriple.cpp \
    lib/misulib/comm/libofqf/qoscclient.cpp \
    lib/misulib/comm/libofqf/qosctypes.cpp \
    lib/misulib/comm/libofqf/qosctcpclient.cpp \
    lib/misulib/comm/libofqf/qoscserver.cpp \
    lib/misulib/comm/libofqf/qoscmsgparser.cpp \
    lib/misulib/comm/senderdebug.cpp \
    lib/mobilesynth/synth/controller.cpp \
    lib/mobilesynth/synth/envelope.cpp \
    lib/mobilesynth/synth/filter.cpp \
    lib/mobilesynth/synth/key_stack.cpp \
    lib/mobilesynth/synth/lag_processor.cpp \
    lib/mobilesynth/synth/modulation.cpp \
    lib/mobilesynth/synth/oscillator.cpp \
    lib/mobilesynth/synth/parameter.cpp \
    lib/misulib/comm/sendermobilesynth.cpp \
    lib/mobilesynth/mobilesynthqt52.cpp \
    lib/misulib/widgets/faders/mwfaderpitch.cpp \
    lib/misulib/widgets/faders/mwfaderparamctl.cpp \
    lib/misulib/widgets/presets/mwsoundpreset.cpp \
    lib/misulib/comm/senderoscmidigeneric.cpp \
    lib/misulib/comm/senderoscpuredata.cpp \
    lib/misulib/comm/senderreaktor.cpp \
    lib/misulib/comm/sendersupercollider.cpp \
    lib/misulib/widgets/presets/mwmicrotunepreset.cpp \
    lib/misulib/widgets/presets/mwscalepreset.cpp \
    lib/misulib/widgets/presets/presetcollection.cpp \
    lib/misulib/models/purchases.cpp \
    lib/misulib/comm/mastersender.cpp \
    lib/misulib/comm/heartbeat.cpp \
    lib/misulib/widgets/buttons/buttonbase.cpp \
    lib/misulib/widgets/buttons/openarchive.cpp \
    lib/misulib/models/pitchcolor.cpp \
    lib/misulib/widgets/faders/mwfader.cpp \
    app/xmlloader.cpp \
    lib/misulib/widgets/buttons/bendhorizontal.cpp \
    lib/misulib/widgets/buttons/sendcc1.cpp \
    lib/misulib/widgets/buttons/togglesender.cpp \
    lib/misulib/widgets/buttons/togglebw.cpp \
    lib/misulib/widgets/buttons/showfreqs.cpp \
    lib/misulib/widgets/core/scaleswitch.cpp \
    lib/misulib/widgets/buttons/toggleheader.cpp \
    lib/misulib/widgets/buttons/setmainview.cpp \
    lib/misulib/widgets/buttons/togglemenu.cpp \
    lib/misulib/widgets/buttons/togglepresets.cpp \
    lib/misulib/widgets/buttons/octaveshift.cpp \
    lib/misulib/models/playfield.cpp \
    lib/misulib/widgets/core/gamecontrol.cpp \
    lib/misulib/widgets/core/octaveranger.cpp \
    lib/misulib/widgets/core/rootnotesetter.cpp \
    lib/misulib/widgets/core/playarea.cpp \
    lib/misulib/widgets/core/touchhistory.cpp \
    lib/misulib/models/touchhistoryevent.cpp \
    lib/misulib/widgets/presets/songtextimport.cpp \
    lib/misulib/comm/senderthread.cpp \
    lib/qmidi/src/QMidiOut.cpp \
    lib/qmidi/src/QMidiFile.cpp \
    lib/misulib/comm/senderqmidi.cpp

HEADERS  += \
    app/misuco2.h \
    lib/misulib/models/types.h \
    lib/misulib/models/freqtriple.h \
    lib/misulib/comm/libofqf/qoscclient.h \
    lib/misulib/comm/libofqf/qosctypes.h \
    lib/misulib/comm/libofqf/qosctcpclient.h \
    lib/misulib/comm/libofqf/qoscserverinterface.h \
    lib/misulib/comm/libofqf/qoscserver.h \
    lib/misulib/comm/libofqf/qoscmsgparser.h \
    lib/misulib/comm/libofqf/qoscclientinterface.h \
    lib/misulib/comm/senderdebug.h \
    lib/misulib/comm/isender.h \
    lib/mobilesynth/AudioOutput.h \
    lib/mobilesynth/mobilesynthViewControllerRc1.h \
    lib/mobilesynth/synth/controller.h \
    lib/mobilesynth/synth/envelope.h \
    lib/mobilesynth/synth/filter.h \
    lib/mobilesynth/synth/key_stack.h \
    lib/mobilesynth/synth/lag_processor.h \
    lib/mobilesynth/synth/modulation.h \
    lib/mobilesynth/synth/oscillator.h \
    lib/mobilesynth/synth/parameter.h \
    lib/mobilesynth/synth/test_util.h \
    lib/misulib/comm/sendermobilesynth.h \
    lib/mobilesynth/mobilesynthviewcontrollerrc1.hpp \
    lib/mobilesynth/mobilesynthqt52.h \
    lib/misulib/widgets/faders/mwfaderpitch.h \
    lib/misulib/widgets/faders/mwfaderparamctl.h \
    lib/misulib/widgets/presets/mwsoundpreset.h \
    lib/misulib/comm/senderoscmidigeneric.h \
    lib/misulib/comm/senderoscpuredata.h \
    lib/misulib/comm/sendersupercollider.h \
    lib/misulib/comm/senderreaktor.h \
    lib/misulib/widgets/presets/mwmicrotunepreset.h \
    lib/misulib/widgets/presets/mwscalepreset.h \
    lib/misulib/widgets/presets/presetcollection.h \
    lib/misulib/models/purchases.h \
    lib/misulib/comm/mastersender.h \
    lib/misulib/comm/heartbeat.h \
    lib/misulib/widgets/buttons/buttonbase.h \
    lib/misulib/widgets/buttons/openarchive.h \
    lib/misulib/models/pitchcolor.h \
    lib/misulib/models/colormacros.h \
    lib/misulib/widgets/faders/mwfader.h \
    app/xmlloader.h \
    lib/misulib/widgets/buttons/bendhorizontal.h \
    lib/misulib/widgets/buttons/sendcc1.h \
    lib/misulib/widgets/buttons/togglesender.h \
    lib/misulib/widgets/buttons/togglebw.h \
    lib/misulib/widgets/buttons/showfreqs.h \
    lib/misulib/widgets/core/scaleswitch.h \
    lib/misulib/widgets/buttons/toggleheader.h \
    lib/misulib/widgets/buttons/setmainview.h \
    lib/misulib/widgets/buttons/togglemenu.h \
    lib/misulib/widgets/buttons/togglepresets.h \
    lib/misulib/widgets/buttons/octaveshift.h \
    lib/misulib/models/playfield.h \
    lib/misulib/widgets/core/gamecontrol.h \
    lib/misulib/widgets/core/octaveranger.h \
    lib/misulib/widgets/core/rootnotesetter.h \
    lib/misulib/widgets/core/playarea.h \
    lib/misulib/widgets/core/touchhistory.h \
    lib/misulib/models/touchhistoryevent.h \
    lib/misulib/widgets/presets/songtextimport.h \
    lib/misulib/comm/senderthread.h \
    lib/qmidi/src/QMidiOut.h \
    lib/qmidi/src/QMidiFile.h \
    lib/misulib/comm/senderqmidi.h

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
    lib/mobilesynth/mobilesynthViewControllerRc1.mm \
    lib/mobilesynth/AudioOutput.m \

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

ios {
    QT               = core network quick purchasing
    QMAKE_INFO_PLIST = ios/Info.plist
    QMAKE_ASSET_CATALOGS += ios/Images.xcassets

}


win32 {
        LIBS += -lwinmm
        SOURCES += lib/qmidi/src/OS/QMidi_Win32.cpp
}

linux* {
        LIBS += -lasound
        SOURCES += lib/qmidi/src/OS/QMidi_ALSA.cpp

}

SUBDIRS += \
    misuco2.pro

RESOURCES += \
    qml.qrc

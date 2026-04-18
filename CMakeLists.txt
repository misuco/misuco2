#--------------------------------------------------------------------------------------------------------------------------------------------
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

cmake_minimum_required(VERSION 3.16)

project(misuco2 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(QT_QML_GENERATE_QMLLS_INI ON)

find_package(Qt6 REQUIRED COMPONENTS Quick Multimedia Core5Compat)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(appMisuco2
    main.cpp
    app/misuco2.cpp
    lib/misulib/models/freqtriple.cpp
    lib/misulib/comm/libofqf/qoscclient.cpp
    lib/misulib/comm/libofqf/qosctypes.cpp
    lib/misulib/comm/libofqf/qosctcpclient.cpp
    lib/misulib/comm/libofqf/qoscserver.cpp
    lib/misulib/comm/libofqf/qoscmsgparser.cpp
    lib/misulib/comm/sendermobilesynth.cpp
    lib/misulib/widgets/faders/mwfaderpitch.cpp
    lib/misulib/widgets/faders/mwfaderparamctl.cpp
    lib/misulib/widgets/presets/mwsoundpreset.cpp
    lib/misulib/comm/senderoscmidigeneric.cpp
    lib/misulib/comm/senderreaktor.cpp
    lib/misulib/comm/sendersupercollider.cpp
    lib/misulib/widgets/presets/mwmicrotunepreset.cpp
    lib/misulib/widgets/presets/mwscalepreset.cpp
    lib/misulib/widgets/presets/presetcollection.cpp
    lib/misulib/models/purchases.cpp
    lib/misulib/comm/mastersender.cpp
    lib/misulib/comm/heartbeat.cpp
    lib/misulib/widgets/buttons/buttonbase.cpp
    lib/misulib/widgets/buttons/openarchive.cpp
    lib/misulib/models/pitchcolor.cpp
    lib/misulib/widgets/faders/mwfader.cpp
    app/xmlloader.cpp
    lib/misulib/widgets/buttons/bendhorizontal.cpp
    lib/misulib/widgets/buttons/sendcc1.cpp
    lib/misulib/widgets/buttons/togglesender.cpp
    lib/misulib/widgets/buttons/togglebw.cpp
    lib/misulib/widgets/buttons/showfreqs.cpp
    lib/misulib/widgets/core/scaleswitch.cpp
    lib/misulib/widgets/buttons/toggleheader.cpp
    lib/misulib/widgets/buttons/setmainview.cpp
    lib/misulib/widgets/buttons/togglemenu.cpp
    lib/misulib/widgets/buttons/togglepresets.cpp
    lib/misulib/widgets/buttons/octaveshift.cpp
    lib/misulib/models/playfield.cpp
    lib/misulib/widgets/core/octaveranger.cpp
    lib/misulib/widgets/core/rootnotesetter.cpp
    lib/misulib/widgets/core/playarea.cpp
    lib/misulib/widgets/core/touchhistory.cpp
    lib/misulib/models/touchhistoryevent.cpp
    lib/misulib/widgets/presets/songtextimport.cpp
    lib/misulib/comm/senderthread.cpp
    lib/qmidi/src/QMidiOut.cpp
    lib/qmidi/src/QMidiFile.cpp
    lib/misulib/comm/senderqmidi.cpp
    app/misuco2.h
    lib/misulib/models/types.h
    lib/misulib/models/freqtriple.h
    lib/misulib/comm/libofqf/qoscclient.h
    lib/misulib/comm/libofqf/qosctypes.h
    lib/misulib/comm/libofqf/qosctcpclient.h
    lib/misulib/comm/libofqf/qoscserverinterface.h
    lib/misulib/comm/libofqf/qoscserver.h
    lib/misulib/comm/libofqf/qoscmsgparser.h
    lib/misulib/comm/libofqf/qoscclientinterface.h
    lib/misulib/comm/sendermobilesynth.h
    lib/misulib/widgets/faders/mwfaderpitch.h
    lib/misulib/widgets/faders/mwfaderparamctl.h
    lib/misulib/widgets/presets/mwsoundpreset.h
    lib/misulib/comm/senderoscmidigeneric.h
    lib/misulib/comm/sendersupercollider.h
    lib/misulib/comm/senderreaktor.h
    lib/misulib/widgets/presets/mwmicrotunepreset.h
    lib/misulib/widgets/presets/mwscalepreset.h
    lib/misulib/widgets/presets/presetcollection.h
    lib/misulib/models/purchases.h
    lib/misulib/comm/mastersender.h
    lib/misulib/comm/heartbeat.h
    lib/misulib/widgets/buttons/buttonbase.h
    lib/misulib/widgets/buttons/openarchive.h
    lib/misulib/models/pitchcolor.h
    lib/misulib/models/colormacros.h
    lib/misulib/widgets/faders/mwfader.h
    app/xmlloader.h
    lib/misulib/widgets/buttons/bendhorizontal.h
    lib/misulib/widgets/buttons/sendcc1.h
    lib/misulib/widgets/buttons/togglesender.h
    lib/misulib/widgets/buttons/togglebw.h
    lib/misulib/widgets/buttons/showfreqs.h
    lib/misulib/widgets/core/scaleswitch.h
    lib/misulib/widgets/buttons/toggleheader.h
    lib/misulib/widgets/buttons/setmainview.h
    lib/misulib/widgets/buttons/togglemenu.h
    lib/misulib/widgets/buttons/togglepresets.h
    lib/misulib/widgets/buttons/octaveshift.h
    lib/misulib/models/playfield.h
    lib/misulib/widgets/core/octaveranger.h
    lib/misulib/widgets/core/rootnotesetter.h
    lib/misulib/widgets/core/playarea.h
    lib/misulib/widgets/core/touchhistory.h
    lib/misulib/models/touchhistoryevent.h
    lib/misulib/widgets/presets/songtextimport.h
    lib/misulib/comm/senderthread.h
    lib/qmidi/src/QMidiOut.h
    lib/qmidi/src/QMidiFile.h
    lib/misulib/comm/senderqmidi.h
    polymobilesynth/qt6/mobilesynth.h
    polymobilesynth/qt6/mobilesynth.cpp
    polymobilesynth/qt6/qt68wraper.h
    polymobilesynth/qt6/qt68wraper.cpp
    polymobilesynth/mobilesynth/Classes/synth/arpeggio.h
    polymobilesynth/mobilesynth/Classes/synth/arpeggio.cpp
    polymobilesynth/mobilesynth/Classes/synth/controller.h
    polymobilesynth/mobilesynth/Classes/synth/controller.cpp
    polymobilesynth/mobilesynth/Classes/synth/envelope.h
    polymobilesynth/mobilesynth/Classes/synth/envelope.cpp
    polymobilesynth/mobilesynth/Classes/synth/filter.h
    polymobilesynth/mobilesynth/Classes/synth/filter.cpp
    polymobilesynth/mobilesynth/Classes/synth/key_stack.h
    polymobilesynth/mobilesynth/Classes/synth/key_stack.cpp
    polymobilesynth/mobilesynth/Classes/synth/lag_processor.h
    polymobilesynth/mobilesynth/Classes/synth/lag_processor.cpp
    polymobilesynth/mobilesynth/Classes/synth/modulation.h
    polymobilesynth/mobilesynth/Classes/synth/modulation.cpp
    polymobilesynth/mobilesynth/Classes/synth/oscillator.h
    polymobilesynth/mobilesynth/Classes/synth/oscillator.cpp
    polymobilesynth/mobilesynth/Classes/synth/parameter.h
    polymobilesynth/mobilesynth/Classes/synth/parameter.cpp
)

qt_add_qml_module(appMisuco2
    URI misuco2
    RESOURCES qml.qrc

    # QML_FILES
    #     qml/main.qml
    #     qml/Menu.qml
    #     lib/misulib/qml/playfield/PlayArea.qml
    #     lib/misulib/qml/playfield/PlayFieldNormal.qml
    #     lib/misulib/qml/playfield/PlayFieldBendVertTop.qml
    #     lib/misulib/qml/playfield/PlayFieldBendVertBot.qml
    #     lib/misulib/qml/playfield/PlayFieldBendHori.qml
    #     lib/misulib/qml/elements/RootNoteSetter.qml
    #     lib/misulib/qml/elements/ScaleSwitch.qml
    #     lib/misulib/qml/elements/OctaveRanger.qml
    #     lib/misulib/qml/confareas/TuneArea.qml
    #     lib/misulib/qml/presets/ScalePresetScroll.qml
    #     lib/misulib/qml/elements/Fader.qml
    #     lib/misulib/qml/confareas/SynthArea.qml
    #     lib/misulib/qml/confareas/ConfArea.qml
    #     lib/misulib/qml/elements/Button.qml
    #     lib/misulib/qml/presets/SynthPresetScroll.qml
    #     lib/misulib/qml/presets/icons/Sine.qml
    #     lib/misulib/qml/presets/icons/Square.qml
    #     lib/misulib/qml/presets/icons/Rnd.qml
    #     lib/misulib/qml/presets/icons/Saw.qml
    #     lib/misulib/qml/presets/icons/Tri.qml
    #     lib/misulib/qml/presets/icons/Envelope.qml
    #     lib/misulib/qml/dialogs/DialogPreset.qml
    #     lib/misulib/qml/playfield/PlayFieldBendVertHorizBot.qml
    #     lib/misulib/qml/playfield/PlayFieldBendVertHorizTop.qml
    #     lib/misulib/qml/dialogs/DialogGame.qml
    #     lib/misulib/qml/dialogs/DialogTextImport.qml
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appMisuco2 PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appMisuco2
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
    QT_ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android"
)


target_include_directories(appMisuco2 PUBLIC
    polymobilesynth/mobilesynth/Classes
)

target_link_libraries(appMisuco2
    PRIVATE Qt6::Quick
    PRIVATE Qt6::Multimedia
    PRIVATE Qt6::Core5Compat #QRegExp
)

install(TARGETS appMisuco2
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

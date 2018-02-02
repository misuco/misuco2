import QtQuick 2.0
import QtQuick.Window 2.2

Window {
    property bool portrait: height > width

    property int fontSize1: portrait ? height / 20 : width / 20
    property int fontSize2: portrait ? height / 32 : width / 32
    property int fontSize3: portrait ? height / 48 : width / 48

    property int buttonSize: portrait ? width/6 : width/12
    property int menuButtonSize: width/7

    property int faderRadius:  buttonSize/2
    property int buttonRadius: buttonSize/5
    property int borderWidth: 1+buttonSize/50
    property color borderColor: "#333333"

    property int columnWidth: width/8;

    property color llColor: "#000000"
    property color bgColor: "#333333"
    property color fgColor: "#CCCCCC"
    property color hlColor: "#CC0000"

    visible: true
    width: 480
    height: 640
    title: qsTr("misuco2")
    color: bgColor

    ScalePresetScroll {
        id: scalePresetCol
        visible: layout.scalePresetsVisible
        anchors.top: rootNoteSetterRow.bottom
        anchors.left: parent.left
        width: layout.scalePresetsVisible ? (portrait ? parent.width : columnWidth) : 0
        height: layout.scalePresetsVisible ? (portrait ? buttonSize : parent.height - buttonSize) : 0
    }

    SynthPresetScroll {
        id: synthPresetCol
        visible: layout.synthPresetsVisible
        anchors.top: rootNoteSetterRow.bottom
        anchors.left: parent.left
        width: layout.synthPresetsVisible ? (portrait ? parent.width : columnWidth) : 0
        height: layout.synthPresetsVisible ? (portrait ? buttonSize : playAreaRow.height) : 0
    }

    Menu {
        id: menuRow
        visible: layout.menuVisible
        anchors.top: parent.top
        width: parent.width
        height: layout.menuVisible ? buttonSize : 0
    }

    OctaveRanger {
        id: octaveRangerRow
        visible: layout.octaveRangerVisible && (layout.playAreaVisible || layout.synthAreaVisible)
        controller: layout.octaveRanger
        anchors.top: menuRow.bottom
        width: parent.width
        height: layout.octaveRangerVisible && (layout.playAreaVisible || layout.synthAreaVisible) ? buttonSize : 0
    }

    BScaleSwitch {
        id: bScaleSwitchRow
        visible: layout.bScaleSwitchVisible && (layout.playAreaVisible || layout.synthAreaVisible)
        anchors.top: octaveRangerRow.bottom
        width: parent.width - buttonSize
        height: layout.bScaleSwitchVisible && (layout.playAreaVisible || layout.synthAreaVisible) ? buttonSize : 0
    }

    Loader {
        id: openArchiveButton
        visible: layout.bScaleSwitchVisible && (layout.playAreaVisible || layout.synthAreaVisible)
        anchors {
            top: octaveRangerRow.bottom
            left: bScaleSwitchRow.right
        }
        width: buttonSize
        height: buttonSize

        property bool transparent: true
        property var modelData: layout.openArchive
        source: "Button.qml"
    }

    RootNoteSetter {
        id: rootNoteSetterRow
        anchors.top: bScaleSwitchRow.bottom
        visible: layout.rootNoteSetterVisible && (layout.playAreaVisible || layout.synthAreaVisible)
        width: parent.width
        height: layout.rootNoteSetterVisible && (layout.playAreaVisible || layout.synthAreaVisible) ? buttonSize : 0
    }

    PlayArea {
        id: playAreaRow
        visible: layout.playAreaVisible || (layout.confAreaVisible && !portrait)
        controller: layout.playArea
        anchors.top: portrait ? scalePresetCol.bottom : rootNoteSetterRow.bottom
        anchors.bottom: parent.bottom
        anchors.left: portrait ? parent.left : (layout.confAreaVisible ? confAreaRow.right : scalePresetCol.right)
        width: layout.scalePresetsVisible && !portrait ? parent.width - columnWidth : parent.width
    }

    TuneArea {
        id: tuneAreaRow
        visible: layout.tuneAreaVisible
        anchors.top: rootNoteSetterRow.bottom
        anchors.bottom: parent.bottom
        anchors.left: portrait ? parent.left : scalePresetCol.right
        anchors.right: parent.right
    }

    SynthArea {
        id: synthAreaRow
        visible: layout.synthAreaVisible
        anchors.top: portrait ? synthPresetCol.bottom : rootNoteSetterRow.bottom
        anchors.bottom: parent.bottom
        anchors.left: portrait ? parent.left : synthPresetCol.right
        anchors.right: parent.right
    }

    ConfArea {
        id: confAreaRow
        visible: layout.confAreaVisible
        anchors.top: menuRow.bottom
        anchors.bottom: parent.bottom
        anchors.left: scalePresetCol.right
        width: layout.confAreaVisible ? 6 * buttonSize : 0
    }

    Loader {
        id: showPresetsButton
        visible: layout.playAreaVisible
        anchors {
            top: playAreaRow.top
            left: playAreaRow.left
        }
        width: buttonSize
        height: buttonSize

        property var modelData: layout.showPresets
        property bool transparent: true
        source: "Button.qml"
    }

    Loader {
        id: showMenuButton
        visible: layout.playAreaVisible
        anchors {
            top: playAreaRow.top
            right: parent.right
        }
        width: buttonSize
        height: buttonSize

        property var modelData: layout.showMenu
        property bool transparent: true
        source: "Button.qml"
    }

    Loader {
        id: octDown
        visible: (layout.playAreaVisible|| layout.tuneAreaVisible)  && layout.botOct > 0
        anchors {
            bottom: playAreaRow.bottom
            left: playAreaRow.left
        }
        width: buttonSize
        height: buttonSize

        property var modelData: layout.octDown
        property bool transparent: true
        source: "Button.qml"
    }


    Loader {
        id: octUp
        visible: (layout.playAreaVisible || layout.tuneAreaVisible) && layout.topOct < 9
        anchors {
            bottom: playAreaRow.bottom
            right: parent.right
        }
        width: buttonSize
        height: buttonSize

        property var modelData: layout.octUp
        property bool transparent: true
        source: "Button.qml"
    }

    DialogPreset {
        id: presetDialog
        visible: layout.dialogPresetsVisible
        anchors.fill: parent
    }

    DialogGame {
        id: gameDialog
        visible: layout.game.visible
        anchors.fill: parent
    }

}

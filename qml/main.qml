import QtQuick 2.9
import QtQuick.Window 2.2

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("misuco2")

    property int fontSize1: width/20;
    property int fontSize2: width/32;
    property int fontSize3: width/48;
    property int buttonSize: width/12;
    property int menuSize: width/7;

    RootNoteSetter {
        id: rootNoteSetterRow
        visible: layout.rootNoteSetterVisible
        width: parent.width
        height: layout.rootNoteSetterVisible ? buttonSize : 0
    }

    BScaleSwitch {
        id: bScaleSwitchRow
        visible: layout.bScaleSwitchVisible
        anchors.top: rootNoteSetterRow.bottom
        width: parent.width
        height: layout.bScaleSwitchVisible ? buttonSize : 0
    }

    OctaveRanger {
        id: octaveRangerRow
        visible: layout.octaveRangerVisible
        controller: layout.octaveRanger
        anchors.top: bScaleSwitchRow.bottom
        width: parent.width
        height: layout.octaveRangerVisible ? buttonSize : 0
    }

    Menu {
        id: menuRow
        anchors.top: octaveRangerRow.bottom
        width: parent.width
        height: buttonSize
    }

    PlayArea {
        id: playAreaRow
        visible: layout.playAreaVisible
        controller: layout.playArea
        anchors.top: menuRow.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
    }

    TuneArea {
        id: tuneAreaRow
        visible: layout.tuneAreaVisible
        anchors.top: menuRow.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
    }
}

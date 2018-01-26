import QtQuick 2.9
import QtQuick.Window 2.2

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("misuco2")

    property int fontSize1: width/20;
    property int buttonSize: width/12;

    RootNoteSetter {
        id: rootNoteSetterRow
        width: parent.width
        height: buttonSize
    }

    BScaleSwitch {
        id: bScaleSwitchRow
        anchors.top: rootNoteSetterRow.bottom
        width: parent.width
        height: buttonSize
    }

    PlayArea {
        controller: layout.playArea
        anchors.top: bScaleSwitchRow.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
    }
}

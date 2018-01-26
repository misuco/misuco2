import QtQuick 2.9
import QtQuick.Window 2.2

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("misuco2")

    property int fontSize1: width/20;
    property int buttonSize: width/12;

    Row {
        id: baseNoteSetterRow
        Repeater  {
            model: layout.baseNoteSetter

            Rectangle {
                color: modelData.color
                width: buttonSize
                height: buttonSize

                Text {
                    anchors.fill: parent
                    text: modelData.text1
                    font.pixelSize: fontSize1
                    color: modelData.fontColor
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignTop
                }

                Text {
                    anchors.fill: parent
                    text: modelData.text2
                    color: modelData.fontColor
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignBottom
                }

                MouseArea {
                    anchors.fill: parent
                    onPressed: modelData.onPressed()
                    onReleased: modelData.onReleased()
                }
            }
        }
    }

    Row {
        id: bScaleSwitchRow
        anchors.top: baseNoteSetterRow.bottom
        Repeater  {
            model: layout.bScaleSwitch

            Rectangle {
                color: modelData.color
                width: buttonSize
                height: buttonSize

                Text {
                    anchors.fill: parent
                    text: modelData.text1
                    font.pixelSize: fontSize1
                    color: modelData.fontColor
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignTop
                }

                Text {
                    anchors.fill: parent
                    text: modelData.text2
                    color: modelData.fontColor
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignBottom
                }

                MouseArea {
                    anchors.fill: parent
                    onPressed: modelData.onPressed()
                    onReleased: modelData.onReleased()
                }
            }
        }
    }

    PlayArea {
        controller: layout.playArea
        anchors.top: bScaleSwitchRow.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
    }


}

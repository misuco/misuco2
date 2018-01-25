import QtQuick 2.9
import QtQuick.Window 2.2

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    property int fontSize1: width/20;
    property int buttonSize: width/12;

    Row {
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
}

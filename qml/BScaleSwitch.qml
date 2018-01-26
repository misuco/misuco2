import QtQuick 2.0

Item {
    Row {
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
}

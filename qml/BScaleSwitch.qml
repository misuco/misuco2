import QtQuick 2.0

Item {
    Row {
        Repeater  {
            model: layout.bScaleSwitch

            Rectangle {
                color: modelData.selected ? layout.pitches[modelData.pitchId].color1 : layout.pitches[modelData.pitchId].color0
                width: buttonSize
                height: buttonSize
                radius: buttonRadius
                border.width: borderWidth
                border.color: modelData.selected ? hlColor: borderColor

                Text {
                    anchors.fill: parent
                    text: modelData.text1
                    font.pixelSize: fontSize1
                    color: modelData.selected ? hlColor : fgColor
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignTop
                }

                Text {
                    anchors.fill: parent
                    text: modelData.text2
                    font.pixelSize: fontSize3
                    color: modelData.selected ? hlColor : fgColor
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

import QtQuick 2.0

Item {
    Rectangle {
        color: modelData.colorF1
        anchors.fill: parent
        radius: buttonRadius
        border.width: borderWidth
        border.color: borderColor

        Text {
            anchors.fill: parent
            text: modelData.text1
            font.pixelSize: fontSizeM
            color: fgColor
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignTop
        }

        Text {
            anchors.fill: parent
            text: modelData.text2
            color: fgColor
            font.pixelSize: fontSizeL
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

        Text {
            anchors.fill: parent
            text: modelData.text3
            color: fgColor
            font.pixelSize: fontSizeM
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignBottom
        }
    }
}

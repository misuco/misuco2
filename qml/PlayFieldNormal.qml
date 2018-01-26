import QtQuick 2.0

Item {
    Rectangle {
        color: modelData.colorF1
        anchors.fill: parent

        Text {
            anchors.fill: parent
            text: modelData.text1
            font.pixelSize: fontSize1
            color: modelData.colorF1
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignTop
        }

        Text {
            anchors.fill: parent
            text: modelData.text2
            font.pixelSize: fontSize1
            color: modelData.fontColor
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignBottom
        }
    }
}

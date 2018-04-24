import QtQuick 2.0
import QtGraphicalEffects 1.0

Item {
    Rectangle {
        anchors.fill: parent
        radius: buttonRadius
        border.width: borderWidth
        border.color: borderColor
        color: "Transparent"

        gradient: Gradient {
            GradientStop { position: 0.0; color: modelData.colorF1 }
            GradientStop { position: 1.0; color: modelData.colorF1b }
        }

        Text {
            anchors.fill: parent
            text: modelData.text1
            font.pixelSize: fontSizeM
            color: modelData.colorF1
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignTop
        }

        Text {
            anchors.fill: parent
            text: modelData.text3
            font.pixelSize: fontSizeM
            color: modelData.colorF1b
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignBottom
        }
    }
}


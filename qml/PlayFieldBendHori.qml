import QtQuick 2.0
import QtGraphicalEffects 1.0

Item {
    LinearGradient {
        anchors.fill: parent
        start: Qt.point(0, 0)
        end: Qt.point(width, 0)
        gradient: Gradient {
            GradientStop { position: 0.0; color: modelData.colorF1 }
            GradientStop { position: 1.0; color: modelData.colorF1b }
        }

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
            color: modelData.fontColor
            font.pixelSize: fontSize1
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignBottom
        }
    }
}

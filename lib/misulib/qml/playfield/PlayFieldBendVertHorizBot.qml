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
            GradientStop { position: 1.0; color: modelData.colorF1b }
            GradientStop { position: 0.0; color: modelData.colorF1 }
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
        Rectangle {
            anchors.centerIn: parent
            width: parent.height
            height: parent.width
            radius: buttonRadius
            border.width: borderWidth
            border.color: borderColor
            gradient: Gradient {
                GradientStop { position: 0.0; color: Qt.rgba(modelData.colorF1.r,modelData.colorF1.g,modelData.colorF1.b,0.5) }
                GradientStop { position: 1.0; color: Qt.rgba(modelData.colorF2.r,modelData.colorF2.g,modelData.colorF2.b,0.5) }
            }
            rotation: -90.0;

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
                color: modelData.colorF2
                font.pixelSize: fontSizeM
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignBottom
            }

        }
    }
}


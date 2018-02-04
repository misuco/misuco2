import QtQuick 2.0

Item {
    Rectangle {

        color: "Transparent"

        anchors.fill: parent

        Rectangle {
            visible: layout.game.buttonsVisible
            anchors.fill: parent
            color: "#AA000000"
        }

        Rectangle {
            anchors.fill: gameText
            color: bgColor
        }

        Text {
            id: gameText
            anchors {
                top:parent.top
                margins: buttonSize
            }
            width: parent.width
            height: buttonSize
            text: layout.game.text

            horizontalAlignment: Qt.AlignHCenter

            color: fgColor
            font.pixelSize: fontSize1
        }

        Rectangle {
            visible: layout.game.buttonsVisible
            anchors {
                top: gameText.bottom
                left: parent.left
                margins: buttonSize
            }
            width: buttonSize*1.5
            height: buttonSize
            radius: buttonRadius
            color: bgColor
            border.width: 1
            border.color: fgColor

            Text {
                anchors.fill: parent
                text: "NO"
                color: fgColor
                font.pixelSize: fontSize1
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
            }

            MultiPointTouchArea {
                anchors.fill: parent
                onPressed: {
                    layout.game.no();
                }
            }
        }

        Rectangle {
            visible: layout.game.buttonsVisible
            anchors {
                top: gameText.bottom
                right: parent.right
                margins: buttonSize
            }
            width: buttonSize*1.5
            height: buttonSize
            radius: buttonRadius
            color: bgColor
            border.width: 1
            border.color: fgColor

            Text {
                anchors.fill: parent
                text: "YES"
                color: fgColor
                font.pixelSize: fontSize1
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
            }

            MultiPointTouchArea {
                anchors.fill: parent
                onPressed: {
                    layout.game.yes();
                }
            }
        }
    }
}

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
                bottom: parent.bottom
                left: parent.left
                margins: buttonSize
            }
            width: buttonSize*2
            height: buttonSize
            color: "Transparent"

            Text {
                anchors.fill: parent
                text: "NO"
                color: fgColor
                font.pixelSize: fontSize1
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
                bottom: parent.bottom
                right: parent.right
                margins: buttonSize
            }
            width: buttonSize*2
            height: buttonSize
            color: "Transparent"

            Text {
                anchors.fill: parent
                text: "YES"
                color: fgColor
                font.pixelSize: fontSize1
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

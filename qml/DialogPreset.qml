import QtQuick 2.0

Item {
    Rectangle {

        color: "#99000000"

        anchors.fill: parent
        MultiPointTouchArea {
            anchors.fill: parent
        }

        Rectangle {
            color: bgColor

            anchors.fill: parent
            anchors.leftMargin: buttonSize * 3

            Text {
                anchors {
                    top: parent.top
                    left: parent.left
                    margins: buttonSize
                }
                text: "Would you like to "
                color: fgColor
                font.pixelSize: fontSize1
            }

            Rectangle {
                anchors {
                    bottom: parent.bottom
                    left: parent.left
                    margins: buttonSize
                }
                width: buttonSize*2
                height: buttonSize
                color: bgColor

                Text {
                    anchors.fill: parent
                    text: "Cancel "
                    color: fgColor
                    font.pixelSize: fontSize1
                }

                MultiPointTouchArea {
                    anchors.fill: parent
                    onPressed: {
                        layout.closeDialogPreset();
                    }
                }
            }

            Rectangle {
                anchors {
                    bottom: parent.bottom
                    right: parent.right
                    margins: buttonSize
                }
                width: buttonSize*2
                height: buttonSize
                color: bgColor

                Text {
                    anchors.fill: parent
                    text: "Overwrite "
                    color: fgColor
                    font.pixelSize: fontSize1
                }

                MultiPointTouchArea {
                    anchors.fill: parent
                    onPressed: {
                        layout.overwritePreset();
                    }
                }
            }
        }
    }
}

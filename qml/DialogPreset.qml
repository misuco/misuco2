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
            anchors.topMargin: portrait ? buttonSize * 2 : 0
            anchors.leftMargin: portrait ? 0 : buttonSize * 2

            Text {
                id: mainText
                anchors {
                    top: parent.top
                    left: parent.left
                    right: parent.right
                }

                anchors.margins: buttonSize
                text: "You have pressed and hold a preset button. Would you like to overwrite the preset with the current scale? "
                color: fgColor
                font.pixelSize: fontSize1
                wrapMode: Text.WordWrap
            }

            Row {
                anchors {
                    top: mainText.bottom
                    topMargin: buttonSize
                }

                Rectangle {
                    width: portrait ? buttonSize*1.5 : buttonSize*2
                    height: buttonSize
                    radius: buttonRadius
                    color: bgColor
                    border.width: 1
                    border.color: fgColor

                    Text {
                        anchors.fill: parent
                        text: "Cancel"
                        color: fgColor
                        font.pixelSize: fontSize2
                        horizontalAlignment: Qt.AlignHCenter
                        verticalAlignment: Qt.AlignVCenter
                    }

                    MultiPointTouchArea {
                        anchors.fill: parent
                        onPressed: {
                            layout.scalePresets.closeDialogPreset();
                        }
                    }
                }

                Rectangle {
                    width: portrait ? buttonSize*1.5 : buttonSize*2
                    height: buttonSize
                    radius: buttonRadius
                    color: bgColor
                    border.width: 1
                    border.color: fgColor

                    Text {
                        anchors.fill: parent
                        text: "Overwrite"
                        color: fgColor
                        font.pixelSize: fontSize2
                        horizontalAlignment: Qt.AlignHCenter
                        verticalAlignment: Qt.AlignVCenter
                    }

                    MultiPointTouchArea {
                        anchors.fill: parent
                        onPressed: {
                            layout.scalePresets.overwritePreset();
                        }
                    }
                }

                Rectangle {
                    width: portrait ? buttonSize*1.5 : buttonSize*2
                    height: buttonSize
                    radius: buttonRadius
                    color: bgColor
                    border.width: 1
                    border.color: fgColor

                    Text {
                        anchors.fill: parent
                        text: "Buy\nmanager"
                        color: fgColor
                        font.pixelSize: fontSize2
                        horizontalAlignment: Qt.AlignHCenter
                        verticalAlignment: Qt.AlignVCenter
                    }

                    MultiPointTouchArea {
                        anchors.fill: parent
                        onPressed: {
                            layout.scalePresets.buyPresetManager();
                        }
                    }
                }

            }
        }
    }
}

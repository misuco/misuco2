import QtQuick 2.0
import "icons"

Item {
    GridView {
        model: layout.synthPresets

        anchors.fill: parent
        cellWidth: columnWidth
        cellHeight: buttonSize

        delegate:
        Item {

            Rectangle {
                color: "Transparent"
                width: columnWidth
                height: buttonSize
                radius: buttonRadius
                border.width: borderWidth
                border.color: borderColor

                Text {
                    id: synthPresetTitle
                    anchors.fill: parent
                    color: modelData.selected ? hlColor : fgColor
                    text: index
                }

                Loader {
                    id: iconWave

                    anchors {
                        top: parent.top
                        bottom: parent.bottom
                        left: parent.left
                        margins: height/5
                    }

                    width: parent.width / 2

                    source: {
                        switch(modelData.wave) {
                        case 0:
                            return "icons/Square.qml"
                        case 1:
                            return "icons/Saw.qml"
                        case 2:
                            return "icons/Sine.qml"
                        case 3:
                            return "icons/Tri.qml"
                        case 4:
                            return "icons/Rnd.qml"
                        }

                    }

                }

                Envelope {
                    anchors {
                        top: parent.top
                        bottom: parent.bottom
                        left: iconWave.right
                        margins: height/5
                    }
                    width: parent.width / 2
                }

                MouseArea {
                    anchors.fill: parent
                    onPressed: modelData.onPressed()
                    onPressAndHold: modelData.onPressAndHold()
                    onCanceled: modelData.onCanceled()
                    onReleased: modelData.onReleased()
                }

            }
        }
    }
}

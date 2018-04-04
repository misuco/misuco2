import QtQuick 2.0
import "icons"

Item {
    GridView {
        model: layout.synthPresets.items

        anchors.fill: parent
        cellWidth: portrait ? buttonSize : columnWidth
        cellHeight: buttonSize

        delegate:
        Item {

            Rectangle {
                color: "Transparent"
                width: portrait ? buttonSize : columnWidth
                height: buttonSize
                radius: buttonRadius
                border.width: borderWidth
                border.color: modelData.selected ? hlColor: borderColor

                Text {
                    id: synthPresetTitle
                    anchors.fill: parent
                    font.pixelSize: fontSize2
                    verticalAlignment: portrait ? Qt.AlignTop : Qt.AlignVCenter
                    horizontalAlignment: portrait ? Qt.AlignHCenter : Qt.AlignLeft
                    color: modelData.selected ? hlColor : fgColor
                    text: index
                }

                Loader {
                    id: iconWave

                    anchors {
                        verticalCenter: parent.verticalCenter
                        left: parent.left
                        leftMargin: parent.width/6
                        topMargin: parent.width/6
                        bottomMargin: parent.width/6
                    }
                    width: parent.width / 3
                    height: parent.width / 3
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
                        verticalCenter: parent.verticalCenter
                        left: iconWave.right
                        leftMargin: parent.width/6
                        topMargin: parent.width/6
                        bottomMargin: parent.width/6
                    }
                    width: parent.width / 3
                    height: parent.width / 3
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

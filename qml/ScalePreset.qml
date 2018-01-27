import QtQuick 2.0

Item {
    Column {
        Repeater  {
            model: layout.scalePresets

            Rectangle {
                color: bgColor
                width: columnWidth
                height: buttonSize
                radius: buttonRadius
                border.width: borderWidth
                border.color: borderColor
                clip: true

                Text {
                    anchors.fill: parent
                    text: index
                }

                property int presetNoteWidth: columnWidth / (modelData.bScaleSize - 2);
                property int presetSelected: modelData.selected;

                MouseArea {
                    anchors.fill: parent
                    onPressed: modelData.onPressed()
                    onReleased: modelData.onReleased()
                }

                Row {
                    Repeater {
                        model: modelData.bScale

                        Rectangle {
                            width:  modelData == "1" ? presetNoteWidth : 0
                            height: buttonSize
                            color: presetSelected ? layout.pitches[index].color1 : layout.pitches[index].color0
                        }
                    }
                }
            }
        }
    }
}

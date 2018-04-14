import QtQuick 2.0

Item {
    GridView {
        model: layout.scalePresets.items

        anchors.fill: parent
        cellWidth: portrait ? buttonSize : columnWidth
        cellHeight: buttonSize

        flow: portrait ? GridView.FlowTopToBottom : GridView.FlowLeftToRight

        delegate:
        Item {
            property int cellWidth: portrait ? buttonSize : columnWidth
            property int presetNoteWidth: cellWidth / (modelData.ScaleSize - 1);
            property int presetSelected: modelData.selected;
            property int presetRootNote: modelData.rootNote;

            Row {
                Repeater {
                    model: modelData.Scale

                    Rectangle {
                        property int pitchIndex: (index+presetRootNote)%12
                        width:  modelData == "1" ? presetNoteWidth : 0
                        height: buttonSize
                        radius: buttonRadius
                        color: presetSelected ? layout.pitchColors[pitchIndex].colorOn
                                              : layout.pitchColors[pitchIndex].colorOff
                    }
                }
            }

            Rectangle {
                color: "Transparent"
                width: cellWidth
                height: buttonSize
                //radius: buttonRadius
                border.width: borderWidth
                border.color: modelData.selected ? hlColor: borderColor

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

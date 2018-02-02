import QtQuick 2.0

Item {
    GridView {
        model: layout.scalePresets

        anchors.fill: parent
        cellWidth: columnWidth
        cellHeight: buttonSize

        flow: portrait ? GridView.FlowTopToBottom : GridView.FlowLeftToRight

        delegate:
        Item {
            property int presetNoteWidth: columnWidth / (modelData.bScaleSize - 1);
            property int presetSelected: modelData.selected;
            property int presetRootNote: modelData.rootNote;

            Row {
                Repeater {
                    model: modelData.bScale

                    Rectangle {
                        property int pitchIndex: (index+presetRootNote)%12
                        width:  modelData == "1" ? presetNoteWidth : 0
                        height: buttonSize
                        radius: buttonRadius
                        color: presetSelected ? layout.pitches[pitchIndex].color1 : layout.pitches[pitchIndex].color0
                    }
                }
            }

            Rectangle {
                color: "Transparent"
                width: columnWidth
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

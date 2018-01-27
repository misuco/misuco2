import QtQuick 2.0

Item {
    GridView {
        model: layout.scalePresets

        anchors.fill: parent
        cellWidth: columnWidth
        cellHeight: buttonSize


        delegate:
        Rectangle {
            color: bgColor
            width: columnWidth
            height: buttonSize

            Text {
                anchors.fill: parent
                text: index
            }

            property int presetNoteWidth: columnWidth / (modelData.bScaleSize - 1);
            property int presetSelected: modelData.selected;
            property int presetRootNote: modelData.rootNote;

            MouseArea {
                anchors.fill: parent
                onPressed: modelData.onPressed()
                onPressAndHold: modelData.onPressAndHold()
                onCanceled: modelData.onCanceled()
                onReleased: modelData.onReleased()
            }

            Row {
                Repeater {
                    model: modelData.bScale

                    Rectangle {
                        property int pitchIndex: (index+presetRootNote)%12
                        width:  modelData == "1" ? presetNoteWidth : 0
                        height: buttonSize
                        color: presetSelected ? layout.pitches[pitchIndex].color1 : layout.pitches[pitchIndex].color0
                    }
                }
            }
        }
    }
}

import QtQuick 2.0

Item {
    id: root

    Row {
        anchors.fill: parent

        onHeightChanged: {
            for(var i=0;i<layout.tuneArea.length;i++) {
                layout.tuneArea[i].onResize(height);
            }
        }

        Repeater  {
            model: layout.tuneArea
            Loader {
                width: buttonSize
                height: parent.height
                property var faderColor: modelData.selected ? layout.pitchColors[modelData.rootNote].colorOn
                                                            : layout.pitchColors[modelData.rootNote].colorOff
                source: "../elements/Fader.qml"
            }
        }
    }
}

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
            model: layout.synthArea
            Loader {
                width: buttonSize
                height: parent.height
                property color faderColor: fgColor
                source: "../elements/Fader.qml"
            }
        }
    }
}

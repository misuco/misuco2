import QtQuick 2.0
import QtQuick.Shapes 1.0

Item {
    property int xStep: width / 4

    property int ySustain: height * (1-modelData.sustain/1000)
    property int xAttack: xStep * (1-modelData.attack/1000)
    property int xDecay: xStep * (modelData.decay/1000)
    property int xRelease: xStep * (modelData.release/1000)

    Shape {
        anchors.fill: parent

        ShapePath {
            strokeWidth: 1+height/50
            strokeColor: modelData.selected ? hlColor : fgColor
            fillColor: "Transparent"

            startX: 0
            startY: height

            PathLine { x: xAttack; y: height}
            PathLine { x: xStep; y: 0}
            PathLine { x: xStep + xDecay; y: ySustain}
            PathLine { x: xStep + xDecay + xStep; y: ySustain}
            PathLine { x: xStep + xDecay + xStep + xRelease; y: height}
        }
    }
}

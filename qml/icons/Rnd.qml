import QtQuick 2.0
import QtQuick.Shapes 1.0

Item {
    property int xStep: width / 8
    property int yStep: height / 8

    Shape {

        anchors.fill: parent

        ShapePath {
            strokeWidth: 1+height/50
            strokeColor: modelData.selected ? hlColor : fgColor
            fillColor: "Transparent"

            startX: 0
            startY: yStep * 4

            PathLine { x: xStep * 1; y: yStep  *  2}
            PathLine { x: xStep * 2; y: yStep  *  7}
            PathLine { x: xStep * 3; y: yStep  *  3}
            PathLine { x: xStep * 4; y: yStep  *  8}
            PathLine { x: xStep * 5; y: yStep  *  1}
            PathLine { x: xStep * 6; y: yStep  *  6}
            PathLine { x: xStep * 7; y: yStep  *  3}
            PathLine { x: xStep * 8; y: yStep  *  4}
        }
    }
}

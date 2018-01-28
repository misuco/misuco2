import QtQuick 2.0
import QtQuick.Shapes 1.0

Item {
    property int xStep: width / 2
    property int yStep: height / 2
    property int radius: height / 4

    Shape {

        anchors.fill: parent

        ShapePath {
            strokeWidth: 1+height/50
            strokeColor: modelData.selected ? hlColor : fgColor
            fillColor: "Transparent"

            startX: 0; startY: yStep

            PathArc { x: xStep; y: yStep; radiusX: radius; radiusY: radius }
            PathArc { x: xStep*2; y: yStep; radiusX: radius; radiusY: radius; direction: PathArc.Counterclockwise}
        }
    }

}

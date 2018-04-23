import QtQuick 2.0

Item {
    property int xStep: width / 2
    property int yStep: height / 2

    property var bgColorHl: modelData.selected ? hlColor : bgColor
    property int strokeWidth: 1+height/50

    Rectangle {
        anchors.fill: parent
        color: bgColorHl
    }

    Canvas {
        anchors.fill: parent
        onPaint: {
            var ctx = getContext("2d")
            ctx.lineWidth = strokeWidth
            ctx.strokeStyle = fgColor
            ctx.fillStyle = fgColor

            ctx.beginPath()
            ctx.moveTo(0,yStep)
            ctx.lineTo(0, 1)
            ctx.lineTo(xStep, 1)
            ctx.lineTo(xStep, yStep * 2 -1)
            ctx.lineTo(xStep * 2, yStep * 2 -1)
            ctx.lineTo(xStep * 2, yStep)

            ctx.stroke()
        }
    }
}

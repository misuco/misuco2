import QtQuick 2.0

Item {
    property int xStep: width / 8
    property int yStep: height / 8

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
            ctx.moveTo(0,yStep * 4)
            ctx.lineTo(xStep * 1, yStep * 2)
            ctx.lineTo(xStep * 2, yStep * 7)
            ctx.lineTo(xStep * 3, yStep * 3)
            ctx.lineTo(xStep * 4, yStep * 8)
            ctx.lineTo(xStep * 5, yStep * 1)
            ctx.lineTo(xStep * 6, yStep * 6)
            ctx.lineTo(xStep * 7, yStep * 3)
            ctx.lineTo(xStep * 8, yStep * 4)

            ctx.stroke()
        }
    }
}

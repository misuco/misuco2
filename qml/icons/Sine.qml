import QtQuick 2.0

Item {
    property int xStep: width / 4
    property int yStep: height / 2
    property int radius: width / 4

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
            ctx.arc(xStep,yStep,radius,Math.PI,Math.PI*2)
            ctx.stroke()
            ctx.beginPath()
            ctx.arc(3*xStep,yStep,radius,0,Math.PI)
            ctx.stroke()
        }
    }
}

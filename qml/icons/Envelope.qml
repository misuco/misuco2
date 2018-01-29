import QtQuick 2.0
//import QtQuick.Shapes 1.0

Item {
    property int xStep: width / 4

    property int ySustain: height * (1-modelData.sustain/1000)
    property int xAttack: xStep * (1-modelData.attack/1000)
    property int xDecay: xStep * (modelData.decay/1000)
    property int xRelease: xStep * (modelData.release/1000)
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
            ctx.moveTo(0,height)
            ctx.lineTo(xAttack,height)
            ctx.lineTo(xStep,0)
            ctx.lineTo(xStep+ xDecay,ySustain)
            ctx.lineTo(xStep+ xDecay + xStep,ySustain)
            ctx.lineTo(xStep+ xDecay + xStep + xRelease,height)
            ctx.closePath()

            ctx.fill()
            ctx.stroke()
        }
    }
}

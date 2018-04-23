import QtQuick 2.0

Item {
    property int knobSize: buttonSize

    Text {
        id: faderValueText
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: fontSize2
        text: modelData.text1
        font.pixelSize: fontSize2
        color: fgColor
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignTop
    }

    Text {
        id: faderCaptionText
        anchors.top: faderValueText.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        height: fontSize3
        text: modelData.text2
        font.pixelSize: fontSize3
        color: fgColor
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignTop
    }

    Rectangle {
        id: faderBackground
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.topMargin: knobSize
        radius: faderRadius
        border.width: borderWidth
        border.color: borderColor

        color: llColor
        width: buttonSize
        height: parent.height

        Rectangle {
            color: faderColor
            width: buttonSize
            height: knobSize
            radius: faderRadius
            border.width: borderWidth
            border.color: borderColor
            y: modelData.faderY * (parent.height - knobSize)
        }

        MultiPointTouchArea {
            anchors.fill: parent

            onPressed: {
                for(var i=0;i<touchPoints.length;i++) {
                    modelData.onPressed(touchPoints[i].pointId,touchPoints[i].x,touchPoints[i].y,height,width)
                }
            }

            onTouchUpdated: {
                for(var i=0;i<touchPoints.length;i++) {
                    modelData.onUpdated(touchPoints[i].pointId,touchPoints[i].y,height,width)
                }
            }

            onUpdated: {
                for(var i=0;i<touchPoints.length;i++) {
                    modelData.onUpdated(touchPoints[i].pointId,touchPoints[i].y,height,width)
                }
            }

            onCanceled: {
                for(var i=0;i<touchPoints.length;i++) {
                    modelData.onReleased(touchPoints[i].pointId)
                }
            }

            onReleased: {
                for(var i=0;i<touchPoints.length;i++) {
                    modelData.onReleased(touchPoints[i].pointId)
                }
            }

            onGestureStarted: {
                gesture.grab();
            }
        }
    }
}

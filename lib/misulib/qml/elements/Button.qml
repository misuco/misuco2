import QtQuick 2.0

Item {

    Rectangle {
        anchors.fill: parent
        color: transparent ? "Transparent" :
            modelData.selected ? hlColor : llColor;

        radius: buttonRadius
        border.width: borderWidth
        border.color: borderColor

        Text {
            anchors.fill: parent
            text: modelData.text
            font.pixelSize: transparent ? fontSize3 : fontSize2
            color: fgColor
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

        MultiPointTouchArea {
            anchors.fill: parent

            onPressed: {
                for(var i=0;i<touchPoints.length;i++) {
                    modelData.onPressed(touchPoints[i].pointId)
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

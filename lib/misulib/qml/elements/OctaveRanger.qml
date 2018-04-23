import QtQuick 2.0

Item {
    id: root

    property var controller
    property int segWidth: parent.width / controller.nOctaves

    onWidthChanged: controller.onResize(width)

    Rectangle {
        anchors.fill: parent
        color: bgColor
        radius: buttonRadius
        border.width: borderWidth
        border.color: borderColor

        Rectangle {
            x: controller.bottom * segWidth
            y: 0
            color: hlColor
            width: (controller.top - controller.bottom + 1) * segWidth
            height: parent.height
            radius: buttonRadius
            border.width: borderWidth
            border.color: borderColor
        }

        Row {
            anchors.fill: parent
            Repeater {
                model: controller.nOctaves
                Rectangle {
                    width: segWidth
                    height: parent.height
                    color: "Transparent"
                    Text {
                        anchors.fill: parent
                        text: index
                        color: fgColor
                        font.pixelSize: fontSize1
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignTop
                    }
                }
            }
        }

        MultiPointTouchArea {
            anchors.fill: parent

            onPressed: {
                for(var i=0;i<touchPoints.length;i++) {
                    controller.onUpdated(touchPoints[i].pointId,touchPoints[i].x)
                }
            }

            onTouchUpdated: {
                for(var i=0;i<touchPoints.length;i++) {
                    controller.onUpdated(touchPoints[i].pointId,touchPoints[i].x)
                }
            }

            onUpdated: {
                for(var i=0;i<touchPoints.length;i++) {
                    controller.onUpdated(touchPoints[i].pointId,touchPoints[i].x)
                }
            }

            onCanceled: {
                for(var i=0;i<touchPoints.length;i++) {
                    controller.onReleased(touchPoints[i].pointId)
                }
            }

            onReleased: {
                for(var i=0;i<touchPoints.length;i++) {
                    controller.onReleased(touchPoints[i].pointId)
                }
            }

            onGestureStarted: {
                gesture.grab();
            }
        }
    }
}

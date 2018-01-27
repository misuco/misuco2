import QtQuick 2.0

Item {
    id: root

    Row {
        Repeater  {
            model: layout.menu

            Rectangle {
                color: modelData.selected ? hlColor : llColor
                width: menuSize
                height: buttonSize
                radius: buttonRadius
                border.width: borderWidth
                border.color: borderColor

                Text {
                    anchors.fill: parent
                    text: modelData.text
                    font.pixelSize: fontSize2
                    color: fgColor
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignTop
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
    }
}

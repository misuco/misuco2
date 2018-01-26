import QtQuick 2.0

Item {
    id: root

    property QtObject controller

    onWidthChanged: controller.resize(width,height)
    onHeightChanged: controller.resize(width,height)

    property int playFieldWidth: root.width/controller.cols
    property int playFieldHeight: root.height/controller.rows

    MultiPointTouchArea {
        anchors.fill: parent

        onPressed: {
            for(var i=0;i<touchPoints.length;i++) {
                controller.onPressed(touchPoints[i].pointId,touchPoints[i].x,touchPoints[i].y)
            }
        }

        onTouchUpdated: {
            for(var i=0;i<touchPoints.length;i++) {
                controller.onUpdated(touchPoints[i].pointId,touchPoints[i].x,touchPoints[i].y)
            }
        }

        onUpdated: {
            for(var i=0;i<touchPoints.length;i++) {
                controller.onUpdated(touchPoints[i].pointId,touchPoints[i].x,touchPoints[i].y)
            }
        }

        onCanceled: {
            for(var i=0;i<touchPoints.length;i++) {
                controller.onReleased()
            }
        }

        onReleased: {
            for(var i=0;i<touchPoints.length;i++) {
                controller.onReleased()
            }
        }

        onGestureStarted: {
            gesture.grab();
        }
    }

    Row {
        id: playAreaRow0
        anchors.top: root.top
        width: root.width
        height: root.height/controller.rows
        Repeater  {
            model: controller.row0

            delegate:
                Component {
                Loader {
                    width: root.playFieldWidth
                    height: root.playFieldHeight
                    source: {
                        if(modelData.fieldType===1) return "PlayFieldBendVertTop.qml"
                        if(modelData.fieldType===2) return "PlayFieldBendHori.qml"
                        return "PlayFieldNormal.qml";
                    }
                }
            }
        }
    }

    Row {
        id: playAreaRow1
        anchors.top: playAreaRow0.bottom
        width: root.width
        height: root.height/controller.rows
        visible: controller.rows>1
        Repeater  {
            model: controller.row1
            delegate:
                Component {
                Loader {
                    width: root.playFieldWidth
                    height: root.playFieldHeight
                    source: {
                        if(modelData.fieldType===1) return "PlayFieldBendVertBot.qml"
                        if(modelData.fieldType===2) return "PlayFieldBendHori.qml"
                        return "PlayFieldNormal.qml";
                    }
                }
            }
        }
    }

    Row {
        id: playAreaRow2
        anchors.top: playAreaRow1.bottom
        width: root.width
        height: root.height/controller.rows
        visible: controller.rows>2
        Repeater  {
            model: controller.row2
            delegate:
                Component {
                Loader {
                    width: root.playFieldWidth
                    height: root.playFieldHeight
                    source: {
                        if(modelData.fieldType===1) return "PlayFieldBendVertBot.qml"
                        if(modelData.fieldType===2) return "PlayFieldBendHori.qml"
                        return "PlayFieldNormal.qml";
                    }
                }
            }
        }
    }
}


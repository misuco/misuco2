import QtQuick 2.9
import QtQuick.Window 2.2
import QtGraphicalEffects 1.0

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("misuco2")

    property int fontSize1: width/20;
    property int buttonSize: width/12;

    Row {
        id: baseNoteSetterRow
        Repeater  {
            model: layout.baseNoteSetter

            Rectangle {
                color: modelData.color
                width: buttonSize
                height: buttonSize

                Text {
                    anchors.fill: parent
                    text: modelData.text1
                    font.pixelSize: fontSize1
                    color: modelData.fontColor
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignTop
                }

                Text {
                    anchors.fill: parent
                    text: modelData.text2
                    color: modelData.fontColor
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignBottom
                }

                MouseArea {
                    anchors.fill: parent
                    onPressed: modelData.onPressed()
                    onReleased: modelData.onReleased()
                }
            }
        }
    }

    Row {
        id: bScaleSwitchRow
        anchors.top: baseNoteSetterRow.bottom
        Repeater  {
            model: layout.bScaleSwitch

            Rectangle {
                color: modelData.color
                width: buttonSize
                height: buttonSize

                Text {
                    anchors.fill: parent
                    text: modelData.text1
                    font.pixelSize: fontSize1
                    color: modelData.fontColor
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignTop
                }

                Text {
                    anchors.fill: parent
                    text: modelData.text2
                    color: modelData.fontColor
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignBottom
                }

                MouseArea {
                    anchors.fill: parent
                    onPressed: modelData.onPressed()
                    onReleased: modelData.onReleased()
                }
            }
        }
    }

    Item {
        id: playArea

        anchors.top: bScaleSwitchRow.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        onWidthChanged: layout.playArea.resize(width,height)
        onHeightChanged: layout.playArea.resize(width,height)

        property int playFieldWidth: playArea.width/layout.playArea.cols
        property int playFieldHeight: playArea.height/layout.playArea.rows

        MultiPointTouchArea {
            anchors.fill: parent
            onPressed: {
                for(var i=0;i<touchPoints.length;i++) {
                    layout.playArea.onPressed(touchPoints[i].pointId,touchPoints[i].x,touchPoints[i].y)
                }
            }
            onUpdated: {
                for(var i=0;i<touchPoints.length;i++) {
                    layout.playArea.onUpdated(touchPoints[i].pointId,touchPoints[i].x,touchPoints[i].y)
                }
            }
            onReleased: {
                for(var i=0;i<touchPoints.length;i++) {
                    layout.playArea.onReleased(touchPoints[i].pointId,touchPoints[i].x,touchPoints[i].y)
                }
            }
        }

        Row {
            id: playAreaRow0
            anchors.top: playArea.top
            width: playArea.width
            height: playArea.height/2
            Repeater  {
                model: layout.playArea.row0

                Item {
                    width: playArea.playFieldWidth
                    height: playArea.playFieldHeight

                    LinearGradient {
                        anchors.fill: parent
                        start: Qt.point(0, 0)
                        end: Qt.point(0, height)
                        gradient: Gradient {
                            GradientStop { position: 0.0; color: modelData.colorF1b }
                            GradientStop { position: 1.0; color: modelData.colorF1 }
                        }

                        Text {
                            anchors.fill: parent
                            text: modelData.text1
                            font.pixelSize: fontSize1
                            color: modelData.colorF1
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignTop
                        }

                        Text {
                            anchors.fill: parent
                            text: modelData.text2
                            color: modelData.fontColor
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignBottom
                        }
                    }
                }
            }
        }

        Row {
            id: playAreaRow1
            anchors.top: playAreaRow0.bottom
            width: playArea.width
            height: playArea.height/2
            Repeater  {
                model: layout.playArea.row1

                Rectangle {
                    color: modelData.colorF1
                    width: playArea.playFieldWidth
                    height: playArea.playFieldHeight

                    Text {
                        anchors.fill: parent
                        text: modelData.text1
                        font.pixelSize: fontSize1
                        color: modelData.colorF1
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignTop
                    }

                    Text {
                        anchors.fill: parent
                        text: modelData.text2
                        color: modelData.fontColor
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignBottom
                    }
                }
            }
        }
    }

}

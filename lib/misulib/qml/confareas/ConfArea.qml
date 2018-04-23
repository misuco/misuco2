import QtQuick 2.0

Item {
    id: root
    Rectangle {
        id: confBendArea

        anchors {
            top: parent.top
            left: parent.left
        }
        width: buttonSize * 2
        height: parent.height

        color: "Transparent"

        Text {
            id: confBendAreaTitle
            anchors.top: parent.top
            width: buttonSize * 2
            height: fontSize2
            text: "BEND"
            font.pixelSize: fontSize2
            color: fgColor
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignTop
        }

        Row {
            id: confBendFadersRow

            anchors {
                top: confBendAreaTitle.bottom
                left: parent.left
            }
            width: buttonSize * 2
            height: parent.height - buttonSize            

            Loader {
                id: bendTopFader

                width: buttonSize
                height: parent.height - 2 * buttonSize

                property QtObject modelData: layout.confPitchTopFader
                property color faderColor: fgColor

                source: "../elements/Fader.qml"
            }

            Loader {
                id: bendBottomFader

                width: buttonSize
                height: parent.height - 2 * buttonSize

                property QtObject modelData: layout.confPitchBottomFader
                property color faderColor: fgColor

                source: "../elements/Fader.qml"
            }

        }

        Loader {
            anchors {
                top: confBendFadersRow.bottom
                left: parent.left
            }
            width: buttonSize * 2
            height: buttonSize

            property var modelData: layout.confPitchHorizButton
            property bool transparent: false
            source: "../elements/Button.qml"
        }

    }

    Rectangle {
        id: confDisplayArea

        anchors {
            top: parent.top
            left: confBendArea.right
        }
        width: buttonSize
        height: parent.height

        color: "Transparent"

        Text {
            id: confDisplayAreaTitle
            anchors.top: parent.top
            width: buttonSize
            height: fontSize2
            text: "DISP"
            font.pixelSize: fontSize2
            color: fgColor
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignTop
        }

        Loader {
            id:symbolsFader
            anchors {
                top: confDisplayAreaTitle.bottom
                left: parent.left
            }
            width: buttonSize
            height: parent.height - 2 * buttonSize

            property QtObject modelData: layout.confSymbolFader
            property color faderColor: fgColor

            source: "../elements/Fader.qml"
        }

        Loader {
            id: showFreqsButton
            anchors {
                top: symbolsFader.bottom
                left: parent.left
            }
            width: buttonSize
            height: buttonSize

            property var modelData: layout.confShowFreqsButton
            property bool transparent: false
            source: "../elements/Button.qml"
        }

        Loader {
            anchors {
                top: showFreqsButton.bottom
                left: parent.left
            }
            width: buttonSize
            height: buttonSize

            property var modelData: layout.confBWModeButton
            property bool transparent: false
            source: "../elements/Button.qml"
        }
    }

    Rectangle {
        id: confSendArea

        anchors {
            top: parent.top
            left: confDisplayArea.right
        }
        width: buttonSize * 2
        height: parent.height

        color: "Transparent"

        Text {
            id: confSendAreaTitle
            anchors.top: parent.top
            width: buttonSize * 2
            height: fontSize2
            text: "SEND"
            font.pixelSize: fontSize2
            color: fgColor
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignTop
        }

        Loader {
            id: channelFader
            anchors {
                top: confSendAreaTitle.bottom
                left: parent.left
            }
            width: buttonSize
            height: parent.height - 2 * buttonSize

            property QtObject modelData: layout.confChannelFader
            property color faderColor: fgColor

            source: "../elements/Fader.qml"
        }

        Loader {
            anchors {
                top: channelFader.bottom
                left: parent.left
            }
            width: buttonSize
            height: buttonSize

            property var modelData: layout.confCC1Button
            property bool transparent: false
            source: "../elements/Button.qml"
        }

        Loader {
            id: sendMobileButton
            anchors {
                bottom: sendPuredataButton.top
                left: channelFader.right
            }
            width: buttonSize * 2
            height: buttonSize

            property var modelData: layout.confSendMobile
            property bool transparent: false
            source: "../elements/Button.qml"
        }

        Loader {
            id: sendPuredataButton
            anchors {
                bottom: sendReaktorButton.top
                left: channelFader.right
            }
            width: buttonSize * 2
            height: buttonSize

            property var modelData: layout.confSendPuredata
            property bool transparent: false
            source: "../elements/Button.qml"
        }

        Loader {
            id: sendReaktorButton
            anchors {
                bottom: sendSupercolliderButton.top
                left: channelFader.right
            }
            width: buttonSize * 2
            height: buttonSize

            property var modelData: layout.confSendReaktor
            property bool transparent: false
            source: "../elements/Button.qml"
        }

        Loader {
            id: sendSupercolliderButton
            anchors {
                bottom: parent.bottom
                left: channelFader.right
            }
            width: buttonSize * 2
            height: buttonSize

            property var modelData: layout.confSendSupercollider
            property bool transparent: false
            source: "../elements/Button.qml"
        }
    }
}

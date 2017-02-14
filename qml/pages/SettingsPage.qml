import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: page

    allowedOrientations: Orientation.All

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: content.height

        Column {
            id: content
            width: parent.width
            spacing: Theme.paddingMedium
            PageHeader {
                title: qsTr("Settings")
            }

            SectionHeader {
                text: qsTr("Text")
            }

            TextField {
                width: parent.width
                placeholderText: qsTr("Enter text to show on the ticker")
                label: qsTr("LED ticker text")
                inputMethodHints: Qt.ImhNoPredictiveText
                validator: RegExpValidator { regExp: /[a-zA-Z0-9\s,.:!?()+\-*\/%=<>]+/ }
                errorHighlight: !acceptableInput && text.length > 0
                EnterKey.enabled: acceptableInput
                EnterKey.iconSource: "image://theme/icon-m-enter-accept"
                EnterKey.onClicked: appSettings.setValue("tickerText", text)
            }

            SectionHeader {
                text: qsTr("Color")
            }

            Item {
                width: parent.width
                height: colorButton.height

                GlassItem {
                    anchors.right: colorButton.left
                    anchors.verticalCenter: colorButton.verticalCenter
                    radius: 0.5
                    falloffRadius: 0.16
                    color: appSettings.ledColor
                }
                Button {
                    id: colorButton
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: qsTr("Select color")
                    onClicked: {
                        var dialog = pageStack.push("Sailfish.Silica.ColorPickerDialog")
                        dialog.accepted.connect(function() {
                            appSettings.setValue("ledColor", dialog.color)
                        })
                    }
                }
                GlassItem {
                    anchors.left: colorButton.right
                    anchors.verticalCenter: colorButton.verticalCenter
                    radius: 0.5
                    falloffRadius: 0.16
                    color: appSettings.ledColor
                }
            }

            SectionHeader {
                text: qsTr("Animation")
            }

            ComboBox {
                label: qsTr("Tick speed")
                currentIndex: 3

                menu: ContextMenu {
                    MenuItem { text: "100 ms" }
                    MenuItem { text: "200 ms" }
                    MenuItem { text: "500 ms" }
                    MenuItem { text: "1000 ms" }
                    MenuItem { text: "1200 ms" }
                    MenuItem { text: "1500 ms" }
                }
            }
        }
    }
}

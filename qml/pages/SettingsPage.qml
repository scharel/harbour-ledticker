import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: page

    allowedOrientations: Orientation.Landscape

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

            TextField {
                x: Theme.horizontalPageMargin
                width: parent.width - 2 * Theme.horizontalPageMargin
                text: appSettings.tickerText
                placeholderText: qsTr("Enter text to show on the ticker")
                label: qsTr("LED ticker text")
                inputMethodHints: Qt.ImhNoPredictiveText
                validator: RegExpValidator { regExp: /[a-zA-Z0-9\s,.:!?()+\-*\/%=<>]+/ }
                //errorHighlight: !acceptableInput && text.length > 0
                EnterKey.enabled: acceptableInput
                EnterKey.iconSource: "image://theme/icon-m-enter-accept"
                EnterKey.onClicked: appSettings.setValue("tickerText", text)
            }

            SectionHeader {
                text: qsTr("Animation")
            }

            Slider {
                width: parent.width
                minimumValue: 100
                maximumValue: 2000
                stepSize: 100
                value: appSettings.tickerSpeed
                valueText: value + " ms"
                label: qsTr("Ticker speed")
                onValueChanged: appSettings.setValue("tickerSpeed", value)
            }

            Row {
                x: Theme.horizontalPageMargin
                width: parent.width - 2 * Theme.horizontalPageMargin
                height: colorButton.height
                spacing: Theme.paddingMedium

                GlassItem {
                    width: height
                    height: colorButton.height
                    radius: 0.5
                    falloffRadius: 0.16
                    color: appSettings.ledColor
                }
                Button {
                    id: colorButton
                    text: qsTr("Select color")
                    onClicked: {
                        var dialog = pageStack.push("Sailfish.Silica.ColorPickerDialog")
                        dialog.accepted.connect(function() {
                            appSettings.setValue("ledColor", dialog.color)
                        })
                    }
                }
                Button {
                    id: resetColorButton
                    text: qsTr("Set color to Ambiente")
                    visible: appSettings.ledColor !== Theme.highlightColor
                    onClicked: appSettings.setValue("ledColor", Theme.highlightColor)
                }
            }
        }
    }
}

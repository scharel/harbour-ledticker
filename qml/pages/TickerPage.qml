import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.ledticker 1.0

Page {
    id: page

    allowedOrientations: Orientation.Landscape

    property bool drawingMode: true

    SilicaGridView {
        id: tickerGrid
        anchors.fill: parent
        cellWidth: page.width / fontBitmap.columns
        cellHeight: page.height / fontBitmap.rows

        PullDownMenu {
            MenuItem {
                text: qsTr("Enable drawing mode")
                visible: false// !drawingModecolumns
                onClicked: drawingMode = true
            }
            MenuItem {
                text: qsTr("Cancel")
                visible: drawingMode
                onClicked: drawingMode = false
            }
            MenuItem {
                text: qsTr("Apply drawing")
                visible: drawingMode
                onClicked: drawingMode = false
            }
            MenuItem {
                text: qsTr("Settings")
                visible: !drawingMode
                onClicked: pageStack.push(Qt.resolvedUrl("SettingsPage.qml"))
            }
        }

        model: FontBitmap {
            id: fontBitmap
            rows: 9
            columns: 16
            Component.onCompleted: init()
        }
        delegate: BackgroundItem {
            width: tickerGrid.cellWidth
            height: tickerGrid.cellHeight
            enabled: drawingMode

            GlassItem {
                id: glassItem
                anchors.centerIn: parent
                dimmed: !bitOn
                radius: 0.5
                falloffRadius: 0.16
                opacity: dimmed? 0.2: 1
                color: appSettings.ledColor
            }
            onClicked: {
                //glassItem.dimmed = !glassItem.dimmed
                bitOn = !bitOn
                var column = index % fontBitmap.columns
                var row = parseInt(index / fontBitmap.columns)
                console.log("Row " + row + " / " + "Col " + column + ": " + bitOn)
            }
        }
    }
}


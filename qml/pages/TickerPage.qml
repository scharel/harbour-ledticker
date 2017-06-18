import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.ledticker 1.0

Page {
    id: page

    allowedOrientations: Orientation.LandscapeMask

    property bool drawingMode: false

    SilicaFlickable {
        id: flickable
        anchors.fill: parent
        contentWidth: tickerGrid.width
        contentHeight: height

        PullDownMenu {
            MenuItem {
                text: qsTr("Enable drawing mode")
                visible: !drawingMode
                onClicked: drawingMode = true
            }
            MenuItem {
                text: qsTr("Cancel")
                visible: drawingMode
                onClicked: drawingMode = false
            }
            MenuItem {
                text: qsTr("Add 8 columns")
                visible: drawingMode
                onClicked: console.log("Add 8 columns")
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

        SilicaGridView {
            id: tickerGrid
            width: drawingMode? bitmap.virtualColumns * cellWidth : page.width
            height: parent.height
            cellWidth: page.width / bitmap.columns
            cellHeight: page.height / bitmap.rows

            model: BitmapModel {
                id: bitmap
                columns: 16
                rows: 9
                virtualColumns: 32
                Component.onCompleted: init()   // DEBUG
            }
            delegate: BackgroundItem {
                width: tickerGrid.cellWidth
                height: tickerGrid.cellHeight
                enabled: drawingMode

                GlassItem {
                    id: glassItem
                    anchors.centerIn: parent
                    dimmed: !on
                    radius: 0.5
                    falloffRadius: dimmed ? 0.1 : 0.2
                    opacity: dimmed ? 0.4 : 1
                    color: appSettings.ledColor
                }
                onClicked: {
                    on = !on
                    console.log("Column " + column + " / " + "Row " + row + ": " + on)
                }
            }
        }
    }
}


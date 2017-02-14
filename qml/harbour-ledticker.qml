import QtQuick 2.0
import Sailfish.Silica 1.0
import org.nemomobile.configuration 1.0
import "pages"

ApplicationWindow
{
    ConfigurationGroup {
        id: appSettings
        path: "/apps/harbour-ledticker/settings"
        property color ledColor: value("ledColor", Theme.primaryColor)
        property string tickerText: value("tickerText", "SailfishOS rules!")
    }

    initialPage: Component { TickerPage { } }
    cover: Qt.resolvedUrl("cover/CoverPage.qml")
    allowedOrientations: defaultAllowedOrientations
}

import QtQuick 2.0
import Sailfish.Silica 1.0
import org.nemomobile.configuration 1.0
import "pages"

ApplicationWindow
{
    ConfigurationGroup {
        id: appSettings
        path: "/apps/harbour-ledticker/settings"
        property string tickerText: value("tickerText", "SailfishOS rules!")
        property int tickerSpeed: value("tickerSpeed", 800)
        property color ledColor: value("ledColor", "red")
    }

    initialPage: Component { TickerPage { } }
    cover: Qt.resolvedUrl("cover/CoverPage.qml")
    allowedOrientations: defaultAllowedOrientations
}

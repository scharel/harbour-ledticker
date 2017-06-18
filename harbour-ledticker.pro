# NOTICE:
#
# Application name defined in TARGET has a corresponding QML filename.
# If name defined in TARGET is changed, the following needs to be done
# to match new name:
#   - corresponding QML filename must be changed
#   - desktop icon filename must be changed
#   - desktop filename must be changed
#   - icon definition filename in desktop file must be changed
#   - translation filenames have to be changed

# The name of your application
TARGET = harbour-ledticker

CONFIG += sailfishapp

SOURCES += src/harbour-ledticker.cpp \
    src/bitmapmodel.cpp

OTHER_FILES += qml/harbour-ledticker.qml \
    qml/cover/CoverPage.qml \
    rpm/harbour-ledticker.changes.in \
    rpm/harbour-ledticker.spec \
    rpm/harbour-ledticker.yaml \
    translations/*.ts \
    harbour-ledticker.desktop

SAILFISHAPP_ICONS = 86x86 108x108 128x128 256x256

# to disable building translations every time, comment out the
# following CONFIG line
CONFIG += sailfishapp_i18n

# German translation is enabled as an example. If you aren't
# planning to localize your app, remember to comment out the
# following TRANSLATIONS line. And also do not forget to
# modify the localized app name in the the .desktop file.
TRANSLATIONS += translations/harbour-ledticker-de.ts

DISTFILES += \
    qml/pages/TickerPage.qml \
    qml/pages/SettingsPage.qml

HEADERS += \
    src/bitmapmodel.h \
    src/font4x7.h \
    src/font7x9.h \
    src/font5x8.h

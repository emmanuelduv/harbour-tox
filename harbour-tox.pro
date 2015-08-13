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
TARGET = harbour-tox
QT += multimedia
CONFIG += sailfishapp

SOURCES += src/qTest.cpp \
    src/toxwrapper.cpp \
    src/toxfriend.cpp \
    src/cstring.cpp \
    src/cdata.cpp \
    src/settings.cpp \
    src/corefile.cpp \
    src/toxfile.cpp \
    src/multimedia/videoprobe.cpp \
    src/multimedia/toxvideosurface.cpp

OTHER_FILES += qml/qTest.qml \
    qml/cover/CoverPage.qml \
    qml/pages/FirstPage.qml \
    qml/pages/SecondPage.qml \
    rpm/qTest.changes.in \
    translations/*.ts \
    translations/qTest-fr.ts \
    qml/js/Contacts.js \
    qml/js/Settings.js \
    qml/pages/ContactAdd.qml \
    tox/install.txt \
    qml/pages/ContactWrite.qml \
    harbour-tox.desktop \
    rpm/harbour-tox.spec \
    rpm/harbour-tox.yaml \
    res/settings.ini \
    qml/pages/VideoTest.qml \
    qml/pages/VideoRecord.qml

# to disable building translations every time, comment out the
# following CONFIG line
CONFIG += sailfishapp_i18n
TRANSLATIONS += translations/qTest-de.ts \
    translations/qTest-fr.ts

HEADERS += \
    src/toxwrapper.h \
    src/toxfriend.h \
    src/cstring.h \
    src/cdata.h \
    src/settings.h \
    src/corefile.h \
    src/toxfile.h \
    src/multimedia/videoprobe.h \
    src/multimedia/toxvideosurface.h \
    src/config.h


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../usr/share/harbour-tox/lib/release/ -ltoxcore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../usr/share/harbour-tox/lib/debug/ -ltoxcore
else:unix: LIBS += -L/usr/share/harbour-tox/lib/ -ltoxcore -ltoxav -ltoxdns
else: LIBS += -L/usr/share/harbour-tox/lib/ -ltoxcore -ltoxav -ltoxdns

INCLUDEPATH += /usr/share/harbour-tox/include
DEPENDPATH += /usr/share/harbour-tox/include

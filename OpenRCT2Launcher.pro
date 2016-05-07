#-------------------------------------------------
#
# Project created by QtCreator 2016-01-15T22:23:35
#
#-------------------------------------------------

QT       += core gui widgets network

TARGET = OpenRCT2
TEMPLATE = app

VERSION = 0.0.5


SOURCES  += main.cpp\
    mainwindow.cpp \
    updater.cpp \
    configuration.cpp

HEADERS  += mainwindow.h \
    updater.h \
    platform.h \
    configuration.h \
    configuration_data.h

FORMS    += mainwindow.ui \
    configuration.ui

RESOURCES += resources.qrc \
    languages.qrc

TRANSLATIONS = Languages/en-GB.ts \
    Languages/en-US.ts \
    Languages/de-DE.ts \
    Languages/nl-NL.ts \
    Languages/fr-FR.ts \
    Languages/hu-HU.ts \
    Languages/pl-PL.ts \
    Languages/es-ES.ts \
    Languages/sv-SE.ts \
    Languages/it-IT.ts \
    Languages/pt-BR.ts \
    Languages/zh-TW.ts \
    Languages/zh-CN.ts \
    Languages/fi-FI.ts \
    Languages/ko-KR.ts \
    Languages/ru-RU.ts \
    Languages/cs-CZ.ts \
    Languages/ja-JP.ts


linux {
    SOURCES += archives/gzipreadfilter.cpp \
        archives/tarextractor.cpp
    HEADERS += archives/gzipreadfilter.h \
        archives/tarextractor.h
} else {
    SOURCES += archives/zipextractor.cpp
    HEADERS += archives/zipextractor.h
}


CONFIG += c++11

DEFINES += APP_VERSION=\\\"$$VERSION\\\"

macx: ICON = Icon/OpenRCT2.icns
win32: RC_FILE = Icon/OpenRCT2Launcher.rc

# Use Qt5's Zlib if avaiable, otherwise use system's
exists($$[QT_INSTALL_PREFIX]/include/QtZlib) {
    INCLUDEPATH += $$[QT_INSTALL_PREFIX]/include/QtZlib
} else {
    LIBS += -lz
}

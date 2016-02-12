#-------------------------------------------------
#
# Project created by QtCreator 2016-01-15T22:23:35
#
#-------------------------------------------------

QT       += core gui widgets network multimedia

TARGET = OpenRCT2
TEMPLATE = app

VERSION = 0.0.3


SOURCES  += main.cpp\
    mainwindow.cpp \
    updater.cpp \
    configuration.cpp \
    archives/zipextractor.cpp \
    archives/gzipreadfilter.cpp \
    archives/tarextractor.cpp

HEADERS  += mainwindow.h \
    updater.h \
    platform.h \
    configuration.h \
    configuration_data.h \
    archives/zipextractor.h \
    archives/gzipreadfilter.h \
    archives/tarextractor.h

FORMS    += mainwindow.ui \
    configuration.ui

RESOURCES += resources.qrc


CONFIG += c++11

DEFINES += APP_VERSION=\\\"$$VERSION\\\"

macx:ICON = Icon/OpenRCT2.icns

# Use Qt5's Zlib if avaiable, otherwise use system's
exists($$[QT_INSTALL_PREFIX]/include/QtZlib) {
    INCLUDEPATH += $$[QT_INSTALL_PREFIX]/include/QtZlib
} else {
    LIBS += -lz
}

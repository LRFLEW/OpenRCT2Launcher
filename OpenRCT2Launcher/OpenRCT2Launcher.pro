#-------------------------------------------------
#
# Project created by QtCreator 2016-01-15T22:23:35
#
#-------------------------------------------------

QT       += core gui widgets network multimedia

TARGET = OpenRCT2
TEMPLATE = app

VERSION = v0.0.2


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

# the libarchive code is intentionally using deprecated code
# to maintain compatibility with libarchive 2
QMAKE_CXXFLAGS += -Wno-deprecated-declarations

DEFINES += APP_VERSION=\\\"$$VERSION\\\"

macx {
    INCLUDEPATH += /usr/local/opt/libarchive/include
    ICON = Icon/OpenRCT2.icns
}

LIBS += -lz

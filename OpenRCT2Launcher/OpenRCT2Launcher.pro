#-------------------------------------------------
#
# Project created by QtCreator 2016-01-15T22:23:35
#
#-------------------------------------------------

QT       += core gui widgets network multimedia

TARGET = OpenRCT2
TEMPLATE = app


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

RESOURCES += resources.qrc

CONFIG += c++11

# the libarchive code is intentionally using deprecated code
# to maintain compatibility with libarchive 2
QMAKE_CXXFLAGS += -Wno-deprecated-declarations

macx {
    INCLUDEPATH += /usr/local/opt/libarchive/include
    ICON = Icon/OpenRCT2.icns
}

LIBS += -larchive

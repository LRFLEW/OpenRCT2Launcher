#-------------------------------------------------
#
# Project created by QtCreator 2016-01-15T22:23:35
#
#-------------------------------------------------

QT       += core gui widgets network webkitwidgets

TARGET = OpenRCT2Launcher
TEMPLATE = app


SOURCES += main.cpp\
           mainwindow.cpp \
           updater.cpp

HEADERS  += mainwindow.h \
            updater.h

FORMS    += mainwindow.ui

CONFIG += c++11

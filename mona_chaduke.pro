#-------------------------------------------------
#
# Project created by QtCreator 2017-05-30T23:20:21
#
#-------------------------------------------------

QT       += core gui network webenginewidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mona_chaduke
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    network_access.cpp \
    func_var.cpp \
    config.cpp

HEADERS += \
        mainwindow.h \
    network_access.h \
    func_var.h \
    config.h

FORMS += \
        mainwindow.ui \
    config.ui

RESOURCES += \
    resources.qrc

TRANSLATIONS = ja.ts

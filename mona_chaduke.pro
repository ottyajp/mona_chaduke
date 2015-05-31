#-------------------------------------------------
#
# Project created by QtCreator 2015-05-14T22:31:39
#
#-------------------------------------------------

QT       += core gui network webkitwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mona_chaduke
TEMPLATE = app

CODECFORSRC = UTF-8
TRANSLATIONS = ja.ts
RESOURCES += lang.qrc

SOURCES += main.cpp\
        mainwindow.cpp \
    setting_window.cpp \
    user_profile_window.cpp \
    jsobj.cpp

HEADERS  += mainwindow.h \
    setting_window.h \
    user_profile_window.h \
    jsobj.h

FORMS    += mainwindow.ui \
    setting_window.ui \
    user_profile_window.ui

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
    jsobj.cpp \
    post_message.cpp \
    send_mona_to_res_window.cpp \
    image_window.cpp \
    func_var.cpp \
    withdraw_window.cpp \
    withdraw_deposit_history.cpp \
    send_receive_history.cpp

HEADERS  += mainwindow.h \
    setting_window.h \
    user_profile_window.h \
    jsobj.h \
    post_message.h \
    send_mona_to_res_window.h \
    image_window.h \
    func_var.h \
    withdraw_window.h \
    withdraw_deposit_history.h \
    send_receive_history.h

FORMS    += mainwindow.ui \
    setting_window.ui \
    user_profile_window.ui \
    post_message.ui \
    send_mona_to_res_window.ui \
    image_window.ui \
    withdraw_window.ui \
    withdraw_deposit_history.ui \
    send_receive_history.ui

win32: RC_FILE = mona_chaduke.rc

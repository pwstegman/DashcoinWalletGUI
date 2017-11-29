#-------------------------------------------------
#
# Project created by QtCreator 2014-10-26T15:02:42
#
#-------------------------------------------------

QT       += core gui

QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DashcoinWalletGUI
TEMPLATE = app


SOURCES += main.cpp \
    dashcoinwallet.cpp

HEADERS  += dashcoinwallet.h

FORMS    += dashcoinwallet.ui

win32:RC_ICONS += dashcoin.ico

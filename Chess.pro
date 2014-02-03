#-------------------------------------------------
#
# Project created by QtCreator 2014-01-28T10:22:20
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Chess
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    chessboard.cpp \
    chesspiece.cpp \
    chessmove.cpp

HEADERS  += mainwindow.h \
    ../chessrank.h \
    chessrank.h \
    chesscolor.h \
    chessboard.h \
    chesspiece.h \
    chessmove.h

FORMS    += mainwindow.ui
CONFIG += c++11

RESOURCES += \
    images.qrc

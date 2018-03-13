#-------------------------------------------------
#
# Project created by QtCreator 2015-03-24T18:44:00
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HelpMessageSender
TEMPLATE = app


SOURCES += main.cpp\
        helpmessagewindow.cpp

HEADERS  += helpmessagewindow.h

FORMS    += helpmessagewindow.ui

QMAKE_CXXFLAGS += -std=c++11

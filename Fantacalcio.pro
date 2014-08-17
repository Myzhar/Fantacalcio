#-------------------------------------------------
#
# Project created by QtCreator 2014-08-16T10:44:19
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Fantacalcio
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    src/qplayer.cpp \
    src/qteam.cpp \
    src/qseason.cpp

HEADERS  += mainwindow.h \
    include/qplayer.h \
    include/config.h \
    include/qteam.h \
    include/qseason.h

FORMS    += mainwindow.ui

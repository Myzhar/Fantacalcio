#-------------------------------------------------
#
# Project created by QtCreator 2014-08-16T10:44:19
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Fantacalcio
TEMPLATE = app

INCLUDEPATH += ./include


SOURCES += main.cpp\
        mainwindow.cpp \
        src/qplayer.cpp \
        src/qteam.cpp \
        src/qseason.cpp \
    qteamwidget.cpp \
    qcessionedlg.cpp

HEADERS  += mainwindow.h \
            include/config.h \
            include/qplayer.h \
            include/qseason.h \
            include/qteam.h \
    qteamwidget.h \
    qcessionedlg.h

FORMS    += mainwindow.ui \
    qteamwidget.ui \
    qcessionedlg.ui

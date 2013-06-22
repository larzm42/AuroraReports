#-------------------------------------------------
#
# Project created by QtCreator 2013-06-14T10:09:38
#
#-------------------------------------------------

QT       += core gui sql

TARGET = AuroraReports
TEMPLATE = app
RC_FILE = aurora.rc

SOURCES += main.cpp\
        mainwindow.cpp \
    dbwriter.cpp \
    dbreader.cpp

HEADERS  += mainwindow.h \
    dbwriter.h \
    dbreader.h

FORMS    += mainwindow.ui

RESOURCES += \
    Resources.qrc

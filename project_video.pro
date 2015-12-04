#-------------------------------------------------
#
# Project created by QtCreator 2015-12-04T00:40:58
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = project_video
TEMPLATE = app

SOURCES += main.cpp\
        main_window.cpp \
    video_handler.cpp \
    utils.cpp

HEADERS  += main_window.h \
    video_handler.h \
    utils.h

INCLUDEPATH += /usr/local/include

LIBS += -L/usr/local/lib \
    -lopencv_core \
    -lopencv_highgui

QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.11

#-------------------------------------------------
#
# Project created by QtCreator 2012-09-22T12:59:37
#
#-------------------------------------------------

QT       += core widgets

QT       += gui

TARGET = PhotoMosaic
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += \
    src/main.cpp \
    src/findaveragecolors.cpp \
    src/findavgcolorsindirectory.cpp \
    src/findavgcolorsinmain.cpp \
    src/mainwindow.cpp \
    src/imageinfo.cpp \
    src/photomosaic.cpp \
    src/grabvideoframes.cpp

HEADERS += \
     src/findaveragecolors.h \
     src/findavgcolorsindirectory.h \
    src/findavgcolorsinmain.h \
     src/mainwindow.h \
     src/imageinfo.h \
     src/photomosaic.h \
     src/grabvideoframes.h

FORMS += \
     src/mainwindow.ui




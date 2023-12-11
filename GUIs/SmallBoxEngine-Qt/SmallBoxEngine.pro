#-------------------------------------------------
#
# Project created by QtCreator 2011-08-22T20:07:31
#
#-------------------------------------------------

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = lib-communi
TEMPLATE = app

QT       += core gui

TARGET = SmallBoxEngine
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    smallboxWidget.cpp

HEADERS  += mainwindow.h \
    smallboxWidget.h

FORMS    += mainwindow.ui

unix:MOC_DIR = ./mocs
win32:MOC_DIR = c:/myproject/tmp

win32:INCLUDEPATH+=C:\Users\TEST\Downloads\irrlicht-1.8.4\irrlicht-1.8.4\include
else:unix:INCLUDEPATH+= /home/netpipe/gamedev/irrlicht-1.8.4/include/ /home/Dev/libs/game/irrlicht/Irrlicht-SVN/lib/Linux/

win32:LIBS += -LC:\Users\TEST\Downloads\irrlicht-1.8.4\irrlicht-1.8.4\lib\Win32-visualstudio -lIrrlicht
else:unix:LIBS += -L/home/netpipe/gamedev/irrlicht-1.8.4/lib/Linux/ -LC:\Users\TEST\Downloads\irrlicht-1.8.4\irrlicht-1.8.4\lib\Win32-visualstudio   -lQt5OpenGL -lQt5Widgets -lQt5Gui -lQt5Core -lIrrlicht -lGL -lX11 -lXxf86vm
#-luchardet


#-------------------------------------------------
#
# Project created by QtCreator 2018-07-13T09:30:13
#
#-------------------------------------------------

QT       += core gui network opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qtirc
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += IRC_STATIC

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += .
INCLUDEPATH += ./IrcCore
INCLUDEPATH += ./IrcModel
INCLUDEPATH += ./IrcUtil

unix:MOC_DIR = ./mocs
win32:MOC_DIR = ./mocs

win32:INCLUDEPATH+=C:\Users\TEST\Downloads\irrlicht-1.8.4\irrlicht-1.8.4\include
else:unix:INCLUDEPATH+= /home/Dev/libs/net/IRC/libcommuni/include/IrcCore  /home/Dev/libs/net/IRC/libcommuni/include/IrcModel  /home/Dev/libs/net/IRC/libcommuni/include/IrcUtil  /home/Dev/libs/game/irrlicht/Irrlicht-SVN/include/ /home/Dev/libs/game/irrlicht/Irrlicht-SVN/lib/Linux/ /home/Dev/libs/net/IRC/libcommuni/include

win32:LIBS += -LC:\Users\TEST\Downloads\irrlicht-1.8.4\irrlicht-1.8.4\lib\Win32-visualstudio -lIrrlicht
else:unix:LIBS += -L/home/Dev/libs/net/IRC/libcommuni/lib/ -L/home/Dev/libs/game/irrlicht/Irrlicht-SVN/lib/Linux/ -LC:\Users\TEST\Downloads\irrlicht-1.8.4\irrlicht-1.8.4\lib\Win32-visualstudio  -lIrcCore -lIrcUtil -lIrcModel -lQt5OpenGL -lQt5Widgets -lQt5Gui -lQt5Core -lIrrlicht -lGL -lX11 -lXxf86vm

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    ircclient.cpp \
    ircmessageformatter.cpp \
    irrlichtwidget.cpp \
    irrutil.cpp \
    irrwidget.cpp \
    movemodelanimator.cpp

HEADERS += \
        mainwindow.h \
    ircclient.h \
    ircmessageformatter.h \
    irrlichtwidget.h \
    irrutil.h \
    irrwidget.h \
    movemodelanimator.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

FORMS += \
    dialog.ui

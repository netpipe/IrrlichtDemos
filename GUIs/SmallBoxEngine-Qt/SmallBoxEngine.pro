#-------------------------------------------------
#
# Project created by QtCreator 2011-08-22T20:07:31
#
#-------------------------------------------------

QT       += core gui

TARGET = SmallBoxEngine
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    smallboxWidget.cpp

HEADERS  += mainwindow.h \
    smallboxWidget.h

FORMS    += mainwindow.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../usr/lib/release/ -lIrrlicht
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../usr/lib/debug/ -lIrrlicht
else:unix:!symbian: LIBS += -L$$PWD/../../../../../usr/lib/ -lIrrlicht -lGL -lXext -lX11 -lGLU

INCLUDEPATH += $$PWD/../../../../../usr/include/irrlicht
DEPENDPATH += $$PWD/../../../../../usr/include/irrlicht

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../../../usr/lib/release/Irrlicht.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../../../usr/lib/debug/Irrlicht.lib
else:unix:!symbian: PRE_TARGETDEPS += $$PWD/../../../../../usr/lib/libIrrlicht.a

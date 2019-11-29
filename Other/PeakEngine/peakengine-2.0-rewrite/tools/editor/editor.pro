#
TEMPLATE = app
#
TARGET =
#
DEPENDPATH += . include src
#
INCLUDEPATH += . include /usr/include/
#
LIBS += -lHorde3D -lHorde3DUtils
#
 
#
QT += opengl
#
 
#
# Input
#
HEADERS += include/glwidget.h
SOURCES += main.cpp src/glwidget.cpp
 

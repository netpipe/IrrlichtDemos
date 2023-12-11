SOURCES += main.cpp
INCLUDEPATH += /usr/include/Box2D/ /opt/Box2D/include
LIBS +=-L/opt/Box2D/lib -lBox2D
QMAKE_LFLAGS_DEBUG+=-Wl,-rpath,/opt/Box2D/lib
QT += core widgets gui opengl

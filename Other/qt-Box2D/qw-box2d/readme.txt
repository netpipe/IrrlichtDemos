Qt Widget based Box2d Example
=============================

The idea is to have a simple Box2D example, using just Qt's widget painting.

Build
=====

* copy a Box2D (http://www.box2d.org/) checkout somewhere.
* import the Box2D project to Qt Creator with the cmake import
 * see http://doc.qt.nokia.com/qtcreator-snapshot/creator-project-cmake.html).
* setup a dependency in Qt Creator (see Projects -> Dependencies)
 * from this project to Box2D project
* Adapt (.pro file) for our local include and library paths

e.g.

INCLUDEPATH += <BOX2D_PROJECT_FOLDER>/Box2D_v2.1.2/Box2D
LIBPATH += <BOX2D_PROJECT_FOLDER>/Box2D_v2.1.2/Box2D/Build/Box2D
LIBS += -lBox2D

Now you should be able to compile this project.


TARGET = widgets_wiggly
QT += widgets

HEADERS       = wigglywidget.h \
                dialog.h \
    mainwindow.h
SOURCES       = wigglywidget.cpp \
                dialog.cpp \
                main.cpp \
    mainwindow.cpp


qtConfig(thread): SUBDIRS += widgets_mandelbrot
qtHaveModule(mqtt):qtHaveModule(websockets): SUBDIRS += mqtt_simpleclient

FORMS += \
    mainwindow.ui

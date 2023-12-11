#ifndef SMALLBOXWIDGET_H
#define SMALLBOXWIDGET_H

#include <QWidget>
#include <QResizeEvent>
#include <irrlicht.h>

#include <QTextStream>
#ifdef Q_WS_X11
    #include <X11/Xlib.h>
#endif

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;

// Our Irrlicht rendering widget
// To have everything compile, we need to run MOC on this file
class smallBoxWidget : public QWidget
{
    // Macro for the meta-object compiler MOC
    Q_OBJECT

public:
    explicit smallBoxWidget(QWidget *parent = 0);
    ~smallBoxWidget();

    // Returns a pointer to the Irrlicht Device
    IrrlichtDevice* getIrrlichtDevice();

    // Create the Irrlicht device and connect the signals and slots
    void init(QWidget* container);

signals:
    // Signal that its time to update the frame
    void updateIrrlichtQuery(IrrlichtDevice* device);

public slots:
    // Function called in response to updateIrrlichtQuery. Renders the scene in the widget
    void updateIrrlicht(IrrlichtDevice* device);

protected:
    virtual void paintEvent(QPaintEvent* event);
    virtual void timerEvent(QTimerEvent* event);
    virtual void resizeEvent(QResizeEvent* event);

    IrrlichtDevice *device;
    // We keep the camera inside this widget so we can resize the window dynamically
    ICameraSceneNode* camera;

    QWidget* renderContainer;
};

#endif


//irrcorerenderer.h

#ifndef IRRCORERENDER_H
#define IRRCORERENDER_H

#include <QObject>
#include <QWidget>
#include <irrlicht.h>
#include <qevent.h>
using namespace irr;

class IrrCoreRenderer : public QWidget
{
    Q_OBJECT

public:
    IrrCoreRenderer(QWidget *irrRenderTarget, bool softwareRenderer);
    ~IrrCoreRenderer();
    void init();
    void resizeIrrWidget(int x, int y, int newWidth, int newHeight);

    virtual void keyPressEvent(QKeyEvent* event);
    virtual void keyReleaseEvent(QKeyEvent* event);
    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent* event);

signals:
    void keyPressed(QKeyEvent* event);

private:
    QWidget *irrRenderTarget;
    video::E_DRIVER_TYPE rendererType;
    void irrlichtMouseEvent(QMouseEvent* event, bool keyPressed = true);

    void irrlichtKeyEvent(QKeyEvent* event, bool pressed);
    void animatedMoveModelToPosition(irr::core::vector3df transition, irr::scene::ISceneNode* modelNode = 0);
    irr::core::vector3df getCursoreIntersation();
protected:
    SIrrlichtCreationParameters params;
    IrrlichtDevice* device;

    virtual void timerEvent(QTimerEvent* event);
};

#endif // IRRCORERENDER_H

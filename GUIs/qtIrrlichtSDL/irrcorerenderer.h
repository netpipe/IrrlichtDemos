
//irrcorerenderer.h

#ifndef IRRCORERENDER_H
#define IRRCORERENDER_H

#include <QObject>
#include <QWidget>
#include <irrlicht.h>

using namespace irr;

class IrrCoreRenderer : public QWidget
{
    Q_OBJECT

public:
    IrrCoreRenderer(QWidget *irrRenderTarget, bool softwareRenderer);
    ~IrrCoreRenderer();
    void init();
    void resizeIrrWidget(int x, int y, int newWidth, int newHeight);

private:
    QWidget *irrRenderTarget;
    video::E_DRIVER_TYPE rendererType;

protected:
    SIrrlichtCreationParameters params;
    IrrlichtDevice* device;

    virtual void timerEvent(QTimerEvent* event);
};

#endif // IRRCORERENDER_H

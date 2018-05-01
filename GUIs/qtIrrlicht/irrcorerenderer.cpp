
//irrcorerenderer.cpp

#include "irrcorerenderer.h"

IrrCoreRenderer::IrrCoreRenderer(QWidget *irrRenderTarget, bool softwareRenderer)
{
    this->irrRenderTarget = irrRenderTarget;

    if(softwareRenderer)
    {
       this->rendererType = video::EDT_BURNINGSVIDEO;
    } else
    {
        this->rendererType = video::EDT_OPENGL;
    }

    //Set Parameters. Most important part here is: params.WindowId. This value sets the output destination of this Irrlicht instance.
    this->params.AntiAlias = 0;
    this->params.Bits = 32;
    this->params.DeviceType = EIDT_X11;
    this->params.Doublebuffer = true;
    this->params.DriverType = rendererType;
    this->params.EventReceiver = 0;
    this->params.Fullscreen = false;
    this->params.HighPrecisionFPU = false;
    this->params.IgnoreInput = false;
    this->params.LoggingLevel = ELL_INFORMATION;
    this->params.Stencilbuffer = true;
    this->params.Stereobuffer = false;
    this->params.Vsync = false;
    this->params.WindowId = reinterpret_cast<void*>(this->irrRenderTarget->winId());
    this->params.WindowSize.Width = this->irrRenderTarget->size().width();
    this->params.WindowSize.Height = this->irrRenderTarget->size().height();
    this->params.WithAlphaChannel = false;
    this->params.ZBufferBits = 16;

    this->device = createDeviceEx(this->params);
}

IrrCoreRenderer::~IrrCoreRenderer()
{
    this->device->drop();
}

void IrrCoreRenderer::init()
{
    if (this->device != 0)
    {
        scene::ISceneManager* smgr = this->device->getSceneManager();

        this->device->getFileSystem()->addFileArchive("media/map-20kdm2.pk3");

        scene::IAnimatedMesh* mesh = smgr->getMesh("20kdm2.bsp");
        scene::ISceneNode* node = 0;

        if (mesh)
        {
            node = smgr->addOctreeSceneNode(mesh->getMesh(0), 0, -1, 1024);
        }

        if (node)
        {
            node->setPosition(core::vector3df(-1300, -144, -1299));
        }

        smgr->addCameraSceneNode();

        startTimer(0);
    }
}

void IrrCoreRenderer::timerEvent(QTimerEvent* event)
{
    if(this->device != 0)
    {
        this->device->getTimer()->tick();

        video::SColor color (255, 100, 100, 140);

        this->device->getVideoDriver()->beginScene(true, true, color);
        this->device->getSceneManager()->drawAll();
        this->device->getVideoDriver()->endScene();
    }
}

void IrrCoreRenderer::resizeIrrWidget(int x, int y, int newWidth, int newHeight)
{
    //Resize the render target widget which shows the Irrlicht output.
    this->irrRenderTarget->setGeometry(x, y, newWidth, newHeight);

    //Resize Irrlicht render output itself
    if(this->device != 0)
    {
        core::dimension2d<u32> widgetSize;
        widgetSize.Width = newWidth;
        widgetSize.Height = newHeight;

        this->device->getVideoDriver()->OnResize(widgetSize);

        scene::ICameraSceneNode *cam = this->device->getSceneManager()->getActiveCamera();
        if (cam != 0)
        {
            cam->setAspectRatio((f32)widgetSize.Width / (f32)widgetSize.Height);
        }
    }
}

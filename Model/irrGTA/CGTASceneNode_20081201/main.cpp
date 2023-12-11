#include "irrlicht.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
#include "CGTASceneNode.h"
#include "CDFFMeshFileLoader.h"

using namespace irr;

int main(int argc, char *argv[])
{
    int gameId = 2; // GTA3

    IrrlichtDevice *device = NULL;
    IVideoDriver *driver = NULL;
    ISceneManager *smgr = NULL;

    // use EDT_OPENGL or EDT_BURNINGSVIDEO
    device = createDevice(video::EDT_OPENGL, dimension2d<u32>(640, 480), 32, false, false, false, 0);

    driver = device->getVideoDriver();
    smgr = device->getSceneManager();

    CDFFMeshFileLoader dffloader = CDFFMeshFileLoader(device);
    smgr->addExternalMeshLoader(&dffloader);

    CGTASceneNode *gtanode = new CGTASceneNode(device, gameId, NULL, smgr, -1, core::vector3df(0.0f, 0.0f, 0.0f), core::vector3df(0.0f, 0.0f, 0.0f), core::vector3df(1.0f, 1.0f, 1.0f));
    gtanode->initialise();

    // we must manually render at least once
    gtanode->render();

    smgr->addCameraSceneNodeFPS(NULL, 100.0f, 50.0f, -1, 0, 0, false, 0.0f);

    while(device->run())
    {
        if (device->isWindowActive())
        {
            driver->beginScene(true, true, SColor(255,100,101,140));

            smgr->drawAll();

            driver->endScene();
        }
    }

    device->drop();

    return 0;
}

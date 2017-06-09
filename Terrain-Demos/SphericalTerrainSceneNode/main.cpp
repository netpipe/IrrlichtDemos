#include <irrlicht.h>
#include "CSphericalTerrainSceneNode/CSphericalTerrainSceneNode.h"

using namespace irr;

int main()
{
    SIrrlichtCreationParameters deviceParameter;
    deviceParameter.WindowSize =  core::dimension2d<u32>(800, 600);
    deviceParameter.DriverType = video::EDT_OPENGL;
    deviceParameter.Fullscreen = false;
    deviceParameter.Vsync = true;
    deviceParameter.Bits = 32;
    deviceParameter.AntiAlias = 0;
    deviceParameter.Stencilbuffer = false;
    deviceParameter.EventReceiver = 0;

	IrrlichtDevice* device = createDeviceEx(deviceParameter);

	if(device == 0)
		return 1;

	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();

    driver->setTextureCreationFlag(video::ETCF_ALWAYS_32_BIT, true);
    smgr->setAmbientLight(video::SColorf(0.050f, 0.050f, 0.050f, 1.0f));

	// add our spherical terrain
    scene::CSphericalTerrainSceneNode* sphericalTerrainSceneNode = new scene::CSphericalTerrainSceneNode("heightmap.png", smgr, 5.0f, 1.0f, smgr->getRootSceneNode(), -1);
	scene::ISceneNode* sphericalTerrainNode = sphericalTerrainSceneNode;
	if (sphericalTerrainNode)
	{
	    sphericalTerrainSceneNode->setMaxHeight(1.5f);
		sphericalTerrainNode->setPosition(core::vector3df(0,0,30));
		sphericalTerrainNode->setMaterialTexture(0, driver->getTexture("surface.jpg"));
		sphericalTerrainNode->setMaterialFlag(video::EMF_LIGHTING, false);
		sphericalTerrainNode->getMaterial(0).setFlag(video::EMF_TRILINEAR_FILTER, true);
        sphericalTerrainNode->getMaterial(0).setFlag(video::EMF_ANISOTROPIC_FILTER, true);
	}

	// add water level using a simple sphere
    scene::ISceneNode *node = smgr->addSphereSceneNode(5.4f, 64);
    if (node)
    {
        node->setPosition(core::vector3df(0,0,30));
        node->setMaterialTexture(0, driver->getTexture("water.jpg"));
        node->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
        node->setMaterialFlag(video::EMF_LIGHTING, false);
		node->getMaterial(0).setFlag(video::EMF_TRILINEAR_FILTER, true);
        node->getMaterial(0).setFlag(video::EMF_ANISOTROPIC_FILTER, true);
    }

    scene::ICameraSceneNode *camera;
	camera = smgr->addCameraSceneNodeFPS(0, 100.0f, 0.004f);
	camera->setNearValue(0.05f);
	device->getCursorControl()->setVisible(false);

	int lastFPS = -1;
	while(device->run())
	{
		driver->beginScene(true, true, video::SColor(255,113,113,133));

		smgr->drawAll();
		device->getGUIEnvironment()->drawAll();

		driver->endScene();

		int fps = driver->getFPS();
        if(lastFPS != fps)
        {
            core::stringw str = L"[";
            str += driver->getName();
            str += "] FPS:";
            str += fps;
            str += ", Tri:";
            str += s32(driver->getPrimitiveCountDrawn());

            device->setWindowCaption(str.c_str());
            lastFPS = fps;
        }
	}

	device->drop();

	return 0;
}


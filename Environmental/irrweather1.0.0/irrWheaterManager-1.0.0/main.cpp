#include <irrlicht.h>
#include "irrWeatherManager.h"

using namespace irr;

IrrlichtDevice* device = 0;
scene::ISceneManager* smgr;

class MyEventReceiver : public IEventReceiver
{
public:

virtual bool OnEvent(const SEvent& event)
{
    if (event.EventType == irr::EET_KEY_INPUT_EVENT&& event.KeyInput.PressedDown)
    {
        switch(event.KeyInput.Key)
        {
            case KEY_ESCAPE:{device->closeDevice();}

			default : break;
        }
    }

    return false;
}
};

int main()
{
    MyEventReceiver receiver;
	device = createDevice(video::EDT_OPENGL, core::dimension2du(800, 600),
		32, false, true, false, &receiver);

	device->setEventReceiver(&receiver);
	video::IVideoDriver* driver = device->getVideoDriver();

	driver->setTextureCreationFlag(video::ETCF_ALWAYS_32_BIT, true);
	smgr = device->getSceneManager();

	scene::ICameraSceneNode* camera = smgr->addCameraSceneNodeFPS(0, 100.f, 1.f);
	camera->setPosition(core::vector3df(1280.f, 256.f, 1280.f));
	camera->setTarget(core::vector3df(2560.f, 256.f, 1280.f));

    irrWeatherManager* atmo = new irrWeatherManager(device);

    scene::ITerrainSceneNode* terrain = smgr->addTerrainSceneNode("../../Data/terrain-heightmap.bmp");

	terrain->setScale(core::vector3df(10.f, 1.0f, 10.f));
	terrain->setMaterialFlag(video::EMF_LIGHTING, true);
    terrain->setPosition(core::vector3df(0,0,0));
	terrain->setMaterialTexture(0, driver->getTexture("../../Data/terrain-texture.jpg"));
	terrain->setMaterialTexture(1, driver->getTexture("../../Data/detailmap3.jpg"));

	terrain->setMaterialType(video::EMT_DETAIL_MAP);

	terrain->scaleTexture(1.0f, 5.0f);

	device->getCursorControl()->setVisible(false);
	s32 lastFPS = -1;

    while(device->run())
    {
        atmo->updateWeather();

        //Get and show fps
        if (driver->getFPS() != lastFPS)
        {
            lastFPS = driver->getFPS();
            core::stringw tmp = L"Irrlicht Day/Night (FPS: ";
            tmp += lastFPS;
            tmp += " | ";
            tmp += driver->getPrimitiveCountDrawn();
            tmp += ")";

            device->setWindowCaption(tmp.c_str());
        }

        driver->beginScene(true, true, video::SColor(0,0,0,0));
        smgr->drawAll();
        driver->endScene();
    }

    device->drop();

    return 0;
}

#include <irrlicht.h>
using namespace irr;

IrrlichtDevice *device;

#include "TXTBlend.h"

TXTBlend* tb;

#include "TXTInterface.h"

int main()
{
    TXTEventReceiver receiver;

    device = createDevice(video::EDT_OPENGL, core::dimension2du(500, 300), 16, false, false, false, &receiver);

    device->setWindowCaption(L"TXTBlend 1.0");

    video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();
	gui::IGUIEnvironment* guienv = device->getGUIEnvironment();

	createInterface();

	tb = new TXTBlend(driver);


	while(device->run())
    {
        driver->beginScene(true, true, backcolor);

		smgr->drawAll();
		guienv->drawAll();

		driver->endScene();
    }

    delete tb;

    device->drop();

    return 0;
}

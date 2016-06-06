// based on Example 002 Quake3Map

#include <irrlicht.h>
#include <iostream>

// include the cloudscenenode header
#include "CloudSceneNode.h"

using namespace irr;

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif

scene::CCloudSceneNode* cloudLayer1;
scene::CCloudSceneNode* cloudLayer2;
scene::CCloudSceneNode* cloudLayer3;

class MyEventReceiver : public IEventReceiver
{
public:
	bool OnEvent(const SEvent& event)
	{
		// check if user presses the key 'p'
		if (event.EventType == irr::EET_KEY_INPUT_EVENT && !event.KeyInput.PressedDown)
		{
			switch (event.KeyInput.Key)
			{
			case irr::KEY_KEY_1:
				// switch cloud visibility layer 1
				cloudLayer1->setVisible(!cloudLayer1->isVisible());
				return true;
			case irr::KEY_KEY_2:
				// switch cloud visibility layer 2
				cloudLayer2->setVisible(!cloudLayer2->isVisible());
				return true;
			case irr::KEY_KEY_3:
				// switch cloud visibility layer 2
				cloudLayer3->setVisible(!cloudLayer3->isVisible());
				return true;
			default:
				break;
			}
		}

		return false;
	}
};
int main()
{
	// ask user for driver
	video::E_DRIVER_TYPE driverType;

	printf("Please select the driver you want for this example:\n"\
		" (a) OpenGL 1.5\n (b) Direct3D 9.0c\n (otherKey) exit\n\n");

	char i;
	std::cin >> i;

	switch(i)
	{
		case 'a': driverType = video::EDT_OPENGL;   break;
		case 'b': driverType = video::EDT_DIRECT3D9;break;
		default: return 1;
	}

	// create device and exit if creation failed
	IrrlichtDevice *device = createDevice(driverType, core::dimension2d<u32>(1024, 768));
	if (device == 0)
		return 1;

	// Get a pointer to the video driver and the SceneManager so that
	// we do not always have to call irr::IrrlichtDevice::getVideoDriver() and
	// irr::IrrlichtDevice::getSceneManager().
	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();

	// create sky box
	driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, false);
	scene::ISceneNode* skyboxNode = smgr->addSkyBoxSceneNode(
		driver->getTexture("media/skybox/glacier_up.png"),
		driver->getTexture("media/skybox/glacier_dn.png"),
		driver->getTexture("media/skybox/glacier_lf.png"),
		driver->getTexture("media/skybox/glacier_rt.png"),
		driver->getTexture("media/skybox/glacier_ft.png"),
		driver->getTexture("media/skybox/glacier_bk.png"));
	driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, true);

	// add 1st cloud layer
	cloudLayer1 = new scene::CCloudSceneNode(smgr->getRootSceneNode(), smgr);
	cloudLayer1->setTranslation(core::vector2df(0.008f, 0.0f));
	cloudLayer1->getMaterial(0).setTexture(0, driver->getTexture("media/clouds/cloud01.png"));
	cloudLayer1->setCloudHeight(0.5f, 0.1f, -0.05f);

	// add 2nd cloud layer
    cloudLayer2 = new scene::CCloudSceneNode(smgr->getRootSceneNode(), smgr);
	cloudLayer2->setTranslation(core::vector2df(0.006f, 0.003f));
	cloudLayer2->getMaterial(0).setTexture(0, driver->getTexture("media/clouds/cloud02.png"));
	cloudLayer2->setCloudHeight(0.4f, 0.05f, -0.1f);
	cloudLayer2->setTextureScale(0.5f);

	// add 3rd cloud layer
	cloudLayer3 = new scene::CCloudSceneNode(smgr->getRootSceneNode(), smgr);
	cloudLayer3->setTranslation(core::vector2df(0.006f, 0.003f));
	cloudLayer3->getMaterial(0).setTexture(0, driver->getTexture("media/clouds/cloud03.png"));
	cloudLayer3->setCloudHeight(0.35f, 0.0f, -0.15f);
	cloudLayer3->setTextureScale(0.4f);

	// we need a camera to look at the Quake 3 map.
	scene::ICameraSceneNode* camera = smgr->addCameraSceneNodeFPS();
	if (camera)
	{
		camera->setPosition(core::vector3df(-400,400,-900));
		camera->setFarValue(20000);
	}

	// The mouse cursor needs not be visible, so we hide it via the
	// irr::IrrlichtDevice::ICursorControl.
	device->getCursorControl()->setVisible(false);

	// create event receiver
	MyEventReceiver receiver;
	device->setEventReceiver(&receiver);

	// We have done everything, so lets draw it.
	static u32 lastUpdate = 0;

	while(device->run())
	{
		if (device->isWindowActive())
		{
			// begin scene
			driver->beginScene(true, true, video::SColor(255,200,200,200));

			// draw everything
			smgr->drawAll();

			// end scene
			driver->endScene();

			u32 now = device->getTimer()->getRealTime();

			// update Info every 500 ticks
			if (now > lastUpdate+500)
			{
				int fps = driver->getFPS();
				core::stringw str = L"Simple CloudSceneNode example [";
				str += driver->getName();
				str += "] FPS:";
				str += fps;
				device->setWindowCaption(str.c_str());

				//save the last query time
				lastUpdate = now;
			}
		}
		else
		//	device->yield();
			device->sleep(10,1);
	}

	// In the end, delete the Irrlicht device.
	device->drop();
	return 0;
}

// That's it. Compile and play around with the program.


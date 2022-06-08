#include <irrlicht.h>
#include <iostream>
#include "storm.h"

#ifndef MAIN_CPP_PROPERTIES
#define MAIN_CPP_PROPERTIES
#define IRRLICHT_DIR    "/home/Dev/libs/game/irrlicht/Irrlicht-SVN/"
#endif


using namespace irr;



IrrlichtDevice* device = 0;
scene::ICameraSceneNode* camera = 0;
StormEffect* effect = 0;

////////////////////////////////////////////////////////////////////////////////
class EventReceiver: public IEventReceiver
{

public:
bool OnEvent(const SEvent& event)
{
	if
	(
		(event.EventType == EET_KEY_INPUT_EVENT)
		&& (event.KeyInput.PressedDown == true)
	)
	{
		if (event.KeyInput.Key == irr::KEY_ESCAPE)
		{
			device->closeDevice();
			device->run();
			return true;
		}


		else if (event.KeyInput.Key == irr::KEY_KEY_W)
		{
		}
		else if (event.KeyInput.Key == irr::KEY_KEY_S)
		{
		}
		else if (event.KeyInput.Key == irr::KEY_KEY_A)
		{
		}
		else if (event.KeyInput.Key == irr::KEY_KEY_D)
		{
		}


		else if (event.KeyInput.Key == irr::KEY_KEY_Q)
		{
		}
		else if (event.KeyInput.Key == irr::KEY_KEY_E)
		{
		}
		else if (event.KeyInput.Key == irr::KEY_KEY_Z)
		{
		}
		else if (event.KeyInput.Key == irr::KEY_KEY_C)
		{
		}
		else if (event.KeyInput.Key == irr::KEY_KEY_N)
		{
		}
		else if (event.KeyInput.Key == irr::KEY_KEY_M)
		{
		}


		else if (event.KeyInput.Key == irr::KEY_KEY_R)
		{
		}
		else if (event.KeyInput.Key == irr::KEY_KEY_X)
		{
		}

		else if (event.KeyInput.Key == irr::KEY_KEY_G)
		{
		}
		else if (event.KeyInput.Key == irr::KEY_KEY_B)
		{
		}


		else if (event.KeyInput.Key == irr::KEY_KEY_I)
		{
		}
		else if (event.KeyInput.Key == irr::KEY_KEY_K)
		{
		}

		else if (event.KeyInput.Key == irr::KEY_KEY_O)
		{
		}
		else if (event.KeyInput.Key == irr::KEY_KEY_U)
		{
		}

		else if (event.KeyInput.Key == irr::KEY_KEY_J)
		{
		}
		else if (event.KeyInput.Key == irr::KEY_KEY_L)
		{
		}

		else if (event.KeyInput.Key == irr::KEY_F1)
		{
		}
		else if (event.KeyInput.Key == irr::KEY_F2)
		{

		}
		else if (event.KeyInput.Key == irr::KEY_F3)
		{
			bool enabled = camera->isInputReceiverEnabled();
			camera->setInputReceiverEnabled(!enabled);
			device->getCursorControl()->setVisible(enabled);
		}
		else if (event.KeyInput.Key == irr::KEY_F4)
		{
			effect->start();
		}
		else if (event.KeyInput.Key == irr::KEY_F5)
		{
			effect->stop();
		}
		else if (event.KeyInput.Key == irr::KEY_F6)
		{
		}
		else if (event.KeyInput.Key == irr::KEY_F7)
		{
		}
		else if (event.KeyInput.Key == irr::KEY_F8)
		{
		}
		else if (event.KeyInput.Key == irr::KEY_F9)
		{
		}
		else if (event.KeyInput.Key == irr::KEY_F10)
		{
		}
		else if (event.KeyInput.Key == irr::KEY_F11)
		{
		}
		else if (event.KeyInput.Key == irr::KEY_F12)
		{
		}


		else if (event.KeyInput.Key == irr::KEY_UP)
		{
		}
		else if (event.KeyInput.Key == irr::KEY_DOWN)
		{
		}
		else if (event.KeyInput.Key == irr::KEY_LEFT)
		{
		}
		else if (event.KeyInput.Key == irr::KEY_RIGHT)
		{
		}
		else if (event.KeyInput.Key == irr::KEY_PRIOR)
		{
		}
		else if (event.KeyInput.Key == irr::KEY_NEXT)
		{
		}

	}
	else
	{

	}
	return false;
}

};


int main()
{
	device = createDevice
	(
		video::EDT_OPENGL, core::dimension2d<u32>(1024, 768),
		32,
		false, false, false
	);

	if (device == 0)
		return 1;

	EventReceiver receiver;
	device->setEventReceiver(&receiver);

	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();

	io::IFileSystem* fs = device->getFileSystem();
//	fs->addFileArchive("/home/smso/downloads/irrlicht/irrlicht-1.7.2/");

	f32 camRotateSpeed = 100.0f;
	f32 camMoveSpeed = 1.0f;
	f32 camJumpSpeed = 3.0f;

	camera = smgr->addCameraSceneNodeFPS(0, camRotateSpeed, camMoveSpeed, -1, 0, 0, false, camJumpSpeed);


	////////////////////////////////////////////////////////////////////////////
	// create skybox
//	fs->addFileArchive(IRRLICHT_DIR);

	// create skybox
	driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, false);

	smgr->addSkyBoxSceneNode
	(
		driver->getTexture("media/irrlicht2_up.jpg"),
		driver->getTexture("media/irrlicht2_dn.jpg"),
		driver->getTexture("media/irrlicht2_lf.jpg"),
		driver->getTexture("media/irrlicht2_rt.jpg"),
		driver->getTexture("media/irrlicht2_ft.jpg"),
		driver->getTexture("media/irrlicht2_bk.jpg")
	);

	driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, true);

	////////////////////////////////////////////////////////////////////////////
	effect = new StormEffect(device);


	////////////////////////////////////////////////////////////////////////////
	device->getCursorControl()->setVisible(false);

	// ----------------------------------------------------

	while (device->run())
	if (device->isWindowActive())
	{
		effect->update();
		driver->beginScene(true, true, 0);
		smgr->drawAll();
		driver->endScene();
	}
	else { device->yield(); }
	if (effect) { delete effect; effect = 0; }
	//if (device) { device->drop(); }

	return 0;
}




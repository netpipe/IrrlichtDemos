
//#include <windows.h>
#include <irrlicht.h>
#include <iostream>

using namespace irr;

#pragma comment(lib, "Irrlicht.lib")
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")


// globals
irr::IrrlichtDevice*		g_device;
irr::video::IVideoDriver*	g_driver;
irr::scene::ISceneManager*	g_smgr;

// func def
bool keys[irr::KEY_KEY_CODES_COUNT];

class MyEventReceiver : public IEventReceiver
{
public:
	virtual bool OnEvent(const SEvent& event)
	{
		if(event.EventType == irr::EET_KEY_INPUT_EVENT)
		{
			keys[event.KeyInput.Key] = event.KeyInput.PressedDown;

			return true;
		}

		return false;
	}
};

int main()
{
	g_device = createDevice(irr::video::EDT_OPENGL, core::dimension2du(640, 480));

	if (g_device == 0) return 1; // could not create selected driver.

	MyEventReceiver rv;
	g_device->setEventReceiver(&rv);

	for (int i = 0; i < irr::KEY_KEY_CODES_COUNT; i++)
	{
		keys[i] = false;
	}

	g_driver = g_device->getVideoDriver();
	g_smgr = g_device->getSceneManager();



	scene::IAnimatedMesh* meshTahta = g_smgr->getMesh("jt_piyon.x");
	scene::IMeshSceneNode* node = g_smgr->addOctreeSceneNode(meshTahta->getMesh(0), 0, 1);
	node->setPosition(core::vector3df(-10, 0, -5));

	//scene::ICameraSceneNode* cam = g_smgr->addCameraSceneNodeFPS(0, 60.f, 0.0001f, -1);
	scene::ICameraSceneNode* cam = g_smgr->addCameraSceneNode(0, core::vector3df(0, 20, -25), core::vector3df(0,0,0));
	cam->setNearValue(0.1f);
	cam->setPosition(core::vector3df(0, 20, -25));
	cam->setTarget(core::vector3df(0, 0, 0));

	g_smgr->setAmbientLight(video::SColorf(0.5f, 0.5f, 0.5f));

	scene::ILightSceneNode* l = g_smgr->addLightSceneNode(0, core::vector3df(0.f, 10.f, 0.f),
														video::SColorf(0.1f, 0.1f, 0.1f, 1.f), 50.f);
	l->setLightType(video::ELT_POINT);

	scene::ISceneNode* selectedPiece = NULL;

	int lastFPS = -1;

	while(g_device->run())
	{
		if (g_device->isWindowActive())
		{
			if (keys[irr::KEY_KEY_W])
			{
				core::vector3df m = cam->getTarget() - cam->getPosition();
				m.normalize();
				m *= 0.05f;
				cam->setPosition(cam->getPosition() + m);
			}
			else if (keys[irr::KEY_KEY_S])
			{
				core::vector3df m = cam->getPosition() - cam->getTarget();
				m.normalize();
				m *= 0.05f;
				cam->setPosition(cam->getPosition() + m);
			}
			else if (keys[irr::KEY_KEY_A])
			{
				core::vector3df m = cam->getTarget() - cam->getPosition();
				m.normalize();
				m.rotateXZBy(90);
				m.Y = 0;
				m *= 0.05f;
				cam->setPosition(cam->getPosition() + m);
			}
			else if (keys[irr::KEY_KEY_D])
			{
				core::vector3df m = cam->getTarget() - cam->getPosition();
				m.normalize();
				m.rotateXZBy(-90);
				m.Y = 0;
				m *= 0.05f;
				cam->setPosition(cam->getPosition() + m);
			}
			else if (keys[irr::KEY_ESCAPE])
			{
				break;
			}

			if (selectedPiece)
			{
				selectedPiece->getMaterial(0).Wireframe = false;
				selectedPiece->setDebugDataVisible(scene::EDS_OFF);
			}

			selectedPiece = g_smgr->getSceneCollisionManager()->getSceneNodeFromScreenCoordinatesBB(g_device->getCursorControl()->getPosition());
			if (selectedPiece)
			{
				selectedPiece->getMaterial(0).Wireframe = true;
				selectedPiece->setDebugDataVisible(scene::EDS_BBOX);
			}


			g_driver->beginScene(true, true, video::SColor(255,200,200,200));
			g_smgr->drawAll();


			g_driver->endScene();


			int fps = g_driver->getFPS();

			if (lastFPS != fps)
			{
				core::stringw str = L"Irrlicht Engine - [";
				str += g_driver->getName();
				str += "] FPS:";
				str += fps;

				g_device->setWindowCaption(str.c_str());
				lastFPS = fps;
			}
		}
		else
			g_device->yield();
	}

	g_device->drop();
	return 0;
}

#include <irrlicht.h>
#include <iostream>
#include "CMeshCombiner.h"
#include <time.h>
using namespace irr;
using namespace core;
using namespace video;
using namespace scene;
using namespace gui;

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif


class MyEventReceiver : public IEventReceiver
{
public:

	virtual bool OnEvent(const SEvent& event)
	{

		if (event.EventType == irr::EET_KEY_INPUT_EVENT)
			KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;
		if (event.EventType == irr::EET_MOUSE_INPUT_EVENT) {
			if (event.MouseInput.Event == irr::EMIE_LMOUSE_PRESSED_DOWN)
				MouseIsDown[0] = true;
			else if (event.MouseInput.Event == irr::EMIE_LMOUSE_LEFT_UP)
				MouseIsDown[0] = false;
			if (event.MouseInput.Event == irr::EMIE_RMOUSE_PRESSED_DOWN)
				MouseIsDown[1] = true;
			else if (event.MouseInput.Event == irr::EMIE_RMOUSE_LEFT_UP)
				MouseIsDown[1] = false;
		}
		return false;
	}


	virtual bool IsKeyDown(EKEY_CODE keyCode) const
	{
		return KeyIsDown[keyCode];
	}
	bool IsMouseDown(char num) {
		return MouseIsDown[num];
	}
	MyEventReceiver()
	{
		for (u32 i=0; i<KEY_KEY_CODES_COUNT; ++i)
			KeyIsDown[i] = false;
		MouseIsDown[0] = false;
		MouseIsDown[1] = false;
	}

private:

	bool KeyIsDown[KEY_KEY_CODES_COUNT];
	bool MouseIsDown[2];
};

int main()
{
	video::E_DRIVER_TYPE driverType = video::EDT_DIRECT3D9;

	printf("Please select the driver you want for this example:\n"\
		" (a) Direct3D 9.0c\n (b) Direct3D 8.1\n (c) OpenGL 1.5\n"\
		" (d) Software Renderer\n (e) Burning's Software Renderer\n"\
		" (f) NullDevice\n (otherKey) exit\n\n");

	char i;
	std::cin >> i;

	switch(i)
	{
		case 'a': driverType = video::EDT_DIRECT3D9;break;
		case 'b': driverType = video::EDT_DIRECT3D8;break;
		case 'c': driverType = video::EDT_OPENGL;   break;
		case 'd': driverType = video::EDT_SOFTWARE; break;
		case 'e': driverType = video::EDT_BURNINGSVIDEO;break;
		case 'f': driverType = video::EDT_NULL;     break;
		default: return 1;
	}

	MyEventReceiver receiver;

	IrrlichtDevice* device = createDevice(driverType,
			core::dimension2d<u32>(1024,768), 32, false, false ,false, &receiver);

	if (device == 0)
		return 1;

	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();
	gui::IGUIEnvironment* env = device->getGUIEnvironment();

	int test = 0;
	bool useMeshCombiner = true;

	srand(time(NULL));

	ITexture* lava = driver->getTexture("Textures/Lava.png");
	ITexture* dirt = driver->getTexture("Textures/Dirt.png");
	ITexture* grass = driver->getTexture("Textures/Grass.png");
	ITexture* snow = driver->getTexture("Textures/Snow.png");

	if (test == 0) {
		int mapWidth = 100;
		int mapHeight = 100;
		array<IMeshSceneNode*> nodes;
		for (int x = 0; x < mapWidth; x++) {
			for (int y = 0; y < mapHeight; y++) {
				IMeshSceneNode* tile = smgr->addMeshSceneNode(smgr->getMesh("Models/Plane.obj"));
				tile->setPosition(vector3df(x*tile->getBoundingBox().getExtent().X,0,y*tile->getBoundingBox().getExtent().Z));
				tile->setMaterialFlag(EMF_LIGHTING,false);
				int texture = rand()%4;
				if (texture == 0)
					tile->setMaterialTexture(0,lava);
				if (texture == 1)
					tile->setMaterialTexture(0,dirt);
				if (texture == 2)
					tile->setMaterialTexture(0,grass);
				if (texture == 3)
					tile->setMaterialTexture(0,snow);
				nodes.push_back(tile);
			}
		}
		if (useMeshCombiner) {
			CMeshCombiner* combiner = new CMeshCombiner(0.8,40,ETPT_TILE);
			IMesh* combinedMesh = combiner->combineMeshes(driver,nodes);
			IMeshSceneNode* k = smgr->addMeshSceneNode(combinedMesh);
			k->setMaterialFlag(EMF_LIGHTING,false);
		}
	}

	smgr->addCameraSceneNodeFPS();

	int lastFPS = -1;
	u32 then = device->getTimer()->getTime();
	while(device->run())
	if (device->isWindowActive())
	{
		const u32 now = device->getTimer()->getTime();
		const f32 frameDeltaTime = (f32)(now - then) / 1000.f;
		then = now;
		driver->beginScene(true, true, 0 );

		smgr->drawAll();
		env->drawAll();

		driver->endScene();


		int fps = driver->getFPS();
		if (lastFPS != fps)
		{
			core::stringw str = L"Mesh Combiner [";
			str += driver->getName();
			str += "] FPS:";
			str += fps;

			device->setWindowCaption(str.c_str());
			lastFPS = fps;
		}
	}

	device->drop();

	return 0;
}

/*
Now you know how to use terrain in Irrlicht.
**/

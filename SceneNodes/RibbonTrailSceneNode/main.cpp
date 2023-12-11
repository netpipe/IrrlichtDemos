
#include <irrlicht.h>
#include <iostream>

using namespace irr;

#pragma comment(lib, "Irrlicht.lib")


class MyEventReceiver : public IEventReceiver
{
public:

	MyEventReceiver(scene::ISceneNode* terrain)
	{
		// store pointer to terrain so we can change its drawing mode
		Terrain = terrain;
	}

	bool OnEvent(SEvent event)
	{
		// check if user presses the key 'W' or 'D'
		if (event.EventType == irr::EET_KEY_INPUT_EVENT && !event.KeyInput.PressedDown)
		{
			switch (event.KeyInput.Key)
			{
			case irr::KEY_KEY_W: // switch wire frame mode
				Terrain->setMaterialFlag(video::EMF_WIREFRAME, !Terrain->getMaterial(0).Wireframe);
				Terrain->setMaterialFlag(video::EMF_POINTCLOUD, false);
				return true;
			case irr::KEY_KEY_P: // switch wire frame mode
				Terrain->setMaterialFlag(video::EMF_POINTCLOUD, !Terrain->getMaterial(0).PointCloud);
				Terrain->setMaterialFlag(video::EMF_WIREFRAME, false);
				return true;
			case irr::KEY_KEY_D: // toggle detail map
				Terrain->setMaterialType(
					Terrain->getMaterial(0).MaterialType == video::EMT_SOLID ?
					video::EMT_DETAIL_MAP : video::EMT_SOLID);
				return true;
			}
		}

		return false;
	}

private:
	scene::ISceneNode* Terrain;
};



int main()
{
	// let user select driver type

	IrrlichtDevice* device = createDevice(video::EDT_OPENGL, core::dimension2d<s32>(640, 480));

	if (device == 0)
		return 1; // could not create selected driver.


	/*
	First, we add standard stuff to the scene: A nice irrlicht engine
	logo, a small help text, a user controlled camera, and we disable
	the mouse cursor.
	*/

	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();
	gui::IGUIEnvironment* env = device->getGUIEnvironment();

	driver->setTextureCreationFlag(video::ETCF_ALWAYS_32_BIT, true);

	// add irrlicht logo
	env->addImage(driver->getTexture("../../media/irrlichtlogoalpha.tga"),
		core::position2d<s32>(10,10));

	// add some help text
	gui::IGUIStaticText* text = env->addStaticText(
		L"Press 'W' to change wireframe mode\nPress 'D' to toggle detail map",
		core::rect<s32>(10,453,200,475), true, true, 0, -1, true);

	// add camera
	scene::ICameraSceneNode* camera =
		smgr->addCameraSceneNodeFPS(0,100.0f,1200.0f);

	camera->setPosition(core::vector3df(1900*2,255*2,3700*2));
	camera->setTarget(core::vector3df(2397*2,343*2,2700*2));
	camera->setFarValue(12000.0f);

	// disable mouse cursor
	device->getCursorControl()->setVisible(false);

	/*
	Here comes the terrain renderer scene node: We add it just like any
	other scene node to the scene using ISceneManager::addTerrainSceneNode().
	The only parameter we use is a file name to the heightmap we use. A heightmap
	is simply a gray scale texture. The terrain renderer loads it and creates
	the 3D terrain from it.
	To make the terrain look more big, we change the scale factor of it to (40, 4.4, 40).
	Because we don't have any dynamic lights in the scene, we switch off the lighting,
	and we set the file terrain-texture.jpg as texture for the terrain and
	detailmap3.jpg as second texture, called detail map. At last, we set
	the scale values for the texture: The first texture will be repeated only one time over
	the whole terrain, and the second one (detail map) 20 times.
	*/

	// add terrain scene node
	scene::ITerrainSceneNode* terrain = smgr->addTerrainSceneNode(
		"../../media/terrain-heightmap.bmp");

	terrain->setScale(core::vector3df(40, 4.4f, 40));
	terrain->setMaterialFlag(video::EMF_LIGHTING, false);

	terrain->setMaterialTexture(0, driver->getTexture("../../media/terrain-texture.jpg"));
	terrain->setMaterialTexture(1, driver->getTexture("../../media/detailmap3.jpg"));

	terrain->setMaterialType(video::EMT_DETAIL_MAP);

	terrain->scaleTexture(1.0f, 20.0f);

	/*
	To be able to do collision with the terrain, we create a triangle selector.
	If you want to know what triangle selectors do, just take a look into the
	collision tutorial. The terrain triangle selector works together with the
	terrain. To demonstrate this, we create a collision response animator
	and attach it to the camera, so that the camera will not be able to fly
	through the terrain.
	*/

	// create triangle selector for the terrain
	scene::ITriangleSelector* selector
		= smgr->createTerrainTriangleSelector(terrain, 0);
	terrain->setTriangleSelector(selector);
	selector->drop();

	// create collision response animator and attach it to the camera
	scene::ISceneNodeAnimator* anim = smgr->createCollisionResponseAnimator(
		selector, camera, core::vector3df(60,100,60),
		core::vector3df(0,0,0),
		core::vector3df(0,50,0));
	camera->addAnimator(anim);
	anim->drop();

	/*
	To make the user be able to switch between normal and wireframe mode, we create
	an instance of the event reciever from above and let Irrlicht know about it. In
	addition, we add the skybox which we already used in lots of Irrlicht examples.
	*/

	// create event receiver
//	MyEventReceiver receiver;
//	device->setEventReceiver(&receiver);

   	// create skybox
	driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, false);

	smgr->addSkyBoxSceneNode(
		driver->getTexture("../../media/irrlicht2_up.jpg"),
		driver->getTexture("../../media/irrlicht2_dn.jpg"),
		driver->getTexture("../../media/irrlicht2_lf.jpg"),
		driver->getTexture("../../media/irrlicht2_rt.jpg"),
		driver->getTexture("../../media/irrlicht2_ft.jpg"),
		driver->getTexture("../../media/irrlicht2_bk.jpg"));

	driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, true);

	/*
	That's it, draw everything. Now you know how to use terrain in Irrlicht.
	*/

	int lastFPS = -1;

	while(device->run())
	if (device->isWindowActive())
	{
		driver->beginScene(true, true, 0 );

		smgr->drawAll();
		env->drawAll();

		driver->endScene();

		// display frames per second in window title
		int fps = driver->getFPS();
		if (lastFPS != fps)
		{
			core::stringw str = L"Terrain Renderer - Irrlicht Engine [";
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


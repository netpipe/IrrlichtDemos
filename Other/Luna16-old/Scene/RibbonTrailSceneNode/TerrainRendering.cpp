
#include <irrlicht.h>
#include <iostream>
#include "RibbonTrailSceneNode.h"

using namespace irr;

#pragma comment(lib, "Irrlicht.lib")

RibbonTrailSceneNode* rt;

class MyEventReceiver : public IEventReceiver
{
public:

	MyEventReceiver(scene::ISceneNode* terrain)
	{
		Terrain = terrain;
	}

	virtual bool OnEvent(const SEvent& event)
	{
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

	video::E_DRIVER_TYPE driverType = video::EDT_OPENGL;

	IrrlichtDevice* device = createDevice(driverType, core::dimension2du(640, 480));

	if (device == 0)
		return 1; // could not create selected driver.

	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();
	gui::IGUIEnvironment* env = device->getGUIEnvironment();

	driver->setTextureCreationFlag( video::ETCF_ALWAYS_32_BIT, true );

	// add irrlicht logo
	env->addImage(driver->getTexture("media/irrlichtlogoalpha.tga"),
		core::position2d<s32>(10,10));

	// add some help text
	gui::IGUIStaticText* text = env->addStaticText(
		L"Press 'W' to change wireframe mode\nPress 'D' to toggle detail map",
		core::rect<s32>(10,453,200,475), true, true, 0, -1, true);

	// add camera
	scene::ICameraSceneNode* camera =
		smgr->addCameraSceneNodeFPS(0,100.0f,1.0f);

	camera->setPosition(core::vector3df(1900*2,255*2,3700*2));
	camera->setTarget(core::vector3df(2397*2,343*2,2700*2));
	camera->setFarValue(12000.0f);

	// disable mouse cursor
	device->getCursorControl()->setVisible(false);

	// add terrain scene node
	scene::ITerrainSceneNode* terrain = smgr->addTerrainSceneNode(
		"media/terrain-heightmap.bmp");

	terrain->setScale(core::vector3df(40, 4.4f, 40));
	terrain->setMaterialFlag(video::EMF_LIGHTING, false);

	terrain->setMaterialTexture(0, driver->getTexture("media/terrain.jpg"));
	terrain->setMaterialTexture(1, driver->getTexture("media/detailmap.jpg"));

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

	MyEventReceiver receiver(terrain);
	device->setEventReceiver(&receiver);

	driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, false);

	smgr->addSkyBoxSceneNode(
		driver->getTexture("media/irrlicht2_up.jpg"),
		driver->getTexture("media/irrlicht2_dn.jpg"),
		driver->getTexture("media/irrlicht2_lf.jpg"),
		driver->getTexture("media/irrlicht2_rt.jpg"),
		driver->getTexture("media/irrlicht2_ft.jpg"),
		driver->getTexture("media/irrlicht2_bk.jpg"));

	driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, true);

	int lastFPS = -1;

	video::ITexture* tex = driver->getTexture( "media/portal7.bmp" );

	RibbonTrailSceneNode* rt = new RibbonTrailSceneNode( device, camera, -1 );
	rt->setPosition( core::vector3df( 0, -10, 300 ) );
    rt->setMaterialTexture( 0, tex );
    rt->setPoint1( core::vector3df(  50, 0, 0 ) );
    rt->setPoint2( core::vector3df( -50, 0, 0 ) );
    rt->setMaxDistance( 10 );
	rt->setMaxQuads( 5000 );
    rt->setStartingAlpha( 100 );
	rt->setShowDebug( true );
	rt->setEnabled( true );

	while(device->run())
	//if (device->isWindowActive())
	{

		driver->beginScene(true, true, 0 );


		smgr->drawAll();
		env->drawAll();
rt->render();
		driver->endScene();

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


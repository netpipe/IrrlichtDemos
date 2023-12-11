// terrainrender v0.0.2, 26.09.2003, irrlicht licence
// a hack from Nikos CGeometryCreator
// if you want to help, please contact
// kortyburns or knightoflight, irrlicht-forummembers

#include <stdio.h>
#include <wchar.h>
#include <irrlicht.h>
#include "terrain.h"


using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#pragma comment(lib, "Irrlicht.lib")

//ok, you better not should not use everytime global variables, but easier...
scene::ISceneManager* smgr=0;
video::IVideoDriver* driver=0;
scene::ISceneNode* nodeterrain = 0;
scene::IAnimatedMesh* meshterrain =0;
scene::ISceneNode* waternode;
scene::IAnimatedMesh* hillmesh =0;
IrrlichtDevice* device = 0;
scene::ICameraSceneNode* camera = 0;
scene::ISceneNode* skyboxNode;
gui::IGUIElement* statusText;
IGUIWindow* window=0;
video::ITexture* terraintexture;

terrain terraintest;



class MyEventReceiver : public IEventReceiver
{
public:
	virtual bool OnEvent(const SEvent& event)
	{
		if (event.EventType == EET_GUI_EVENT)
		{
			s32 id = event.GUIEvent.Caller->getID();
			switch(event.GUIEvent.EventType)
			{
				case EGET_BUTTON_CLICKED:
				if (id ==104)
				{
					device->closeDevice();
					return true;
				}
				if (id==105)
				{
					meshterrain = terraintest.renderterrainscaled("hillmesh",
						core::dimension2d< f32 > (64.0f, 64.0f) ,		//tilesize
						0,
						core::dimension2d< f32 >(1.0f, 1.0f),
						16,
						5) ;		// this is the scale
						nodeterrain = smgr->addAnimatedMeshSceneNode( meshterrain );
						nodeterrain->setMaterialTexture( 0, driver->getTexture("./media/HeightMapTerrain.jpg"));
						smgr->setActiveCamera(camera);
						window->setVisible(false);
						device->getCursorControl()->setVisible(false);
						nodeterrain->setMaterialFlag(video::EMF_LIGHTING, false);
						return true;
				}
				if(id==106)
				{

					meshterrain = terraintest.renderterrain("hillmesh",
						core::dimension2d< f32 > (64.0f, 64.0f) ,		//tilesize
						0,
						core::dimension2d< f32 >(1.0f, 1.0f),
						16);
						nodeterrain = smgr->addAnimatedMeshSceneNode( meshterrain );
						nodeterrain->setMaterialTexture( 0, driver->getTexture("./media/HeightMapTerrain.jpg"));
						smgr->setActiveCamera(camera);
						window->setVisible(false);
						device->getCursorControl()->setVisible(false);
						nodeterrain->setMaterialFlag(video::EMF_LIGHTING, false);
						return true;
				}
				if (id==107)
				{
/*
						meshterrain = terraintest.renderterrainwithvertexcolors("hillmesh",
						terraintexture,
						core::dimension2d< f32 > (64.0f, 64.0f) ,		//tilesize
						0,
						core::dimension2d< f32 >(256.0f, 256.0f),
						16);
						nodeterrain = smgr->addAnimatedMeshSceneNode( meshterrain );
						nodeterrain->setMaterialTexture( 0, driver->getTexture("detail.bmp"));
						smgr->setActiveCamera(camera);
						window->setVisible(false);
						device->getCursorControl()->setVisible(false);
						nodeterrain->setMaterialFlag(video::EMF_LIGHTING, false);
						return true;
*/
				}
			}
		}


		if ( event.EventType == irr::EET_KEY_INPUT_EVENT&&
			!event.KeyInput.PressedDown)
		{
			switch(event.KeyInput.Key)
			{
			case KEY_ESCAPE:
				{
					smgr->setActiveCamera(0);
					device->closeDevice();
				}
			}
		}

		if (device->getSceneManager()->getActiveCamera())
			return device->getSceneManager()->getActiveCamera()->OnEvent(event);

		return false;
	}
};



int main()
{
	int fps, lastfps;
	MyEventReceiver receiver;

	device = createDevice(video::EDT_OPENGL, core::dimension2d<u32>(1024, 768),
		16, false, false, &receiver);

	driver = device->getVideoDriver();
	smgr = device->getSceneManager();
	IGUIEnvironment* guienv = device->getGUIEnvironment();
	statusText = device->getGUIEnvironment()->addStaticText(L"Loading...",rect<int>(10,10,200,30),	true);

	window = guienv->addWindow(
	rect<s32>(50 , 50 , 400 , 300 ),false,L"Main Menu", 0, -1);
	guienv->addButton(rect<s32>(30,30,120,60), window, 104, L"End");
	guienv->addButton(rect<s32>(30,70,120,100), window, 105, L"Terrain Scaled");
	guienv->addButton(rect<s32>(30,110,120,140), window, 106, L"Terrain V001");
	guienv->addButton(rect<s32>(30,150,120,180), window, 107, L"Terrain VertexColors");
	window->getCloseButton()->setVisible(false);
	window->getMinimizeButton()->setVisible(false);
	window->getMaximizeButton()->setVisible(false);
	window->setVisible(true);

	terraintexture = driver->getTexture("media/MapTexture.jpg");

	//load the raw map
	terraintest.loadRawMap("media/heightmap.raw");
	//get mesh from heightmap, test 1 scaled with 8 to get better performance

	//and a irrlicht waternode with transparent water, change the path to you media-directory !
	hillmesh = smgr->addHillPlaneMesh("myHill",
		core::dimension2d<f32>(256*64,256*64),
		core::dimension2d<u32>(4,4), 0, 0,
		core::dimension2d<f32>(5,5),
		core::dimension2d<f32>(10,10));
	smgr->getMeshManipulator()->setVertexColorAlpha(hillmesh->getMesh(0), 160);
	waternode = smgr->addWaterSurfaceSceneNode(hillmesh->getMesh(0), 30.0f, 300.0f, 20.0f);
	waternode->setPosition(core::vector3df(0,1100,0));
	waternode->setMaterialTexture(0,	driver->getTexture("media/water.bmp"));
	waternode->setMaterialType(video::EMT_TRANSPARENT_REFLECTION_2_LAYER );
	waternode->setMaterialFlag(video::EMF_LIGHTING, false);
	waternode->setMaterialFlag(video::EMF_BACK_FACE_CULLING , false);

	//and a irrlicht SkyBox for better look, change the path to your media-directory !
	skyboxNode = smgr->addSkyBoxSceneNode(
		driver->getTexture("media/irrlicht2_up.bmp"),
		driver->getTexture("media/irrlicht2_dn.bmp"),
		driver->getTexture("media/irrlicht2_lf.bmp"),
		driver->getTexture("media/irrlicht2_rt.bmp"),
		driver->getTexture("media/irrlicht2_ft.bmp"),
		driver->getTexture("media/irrlicht2_bk.bmp"));

	camera=smgr->addCameraSceneNodeFPS();
	core::vector3df v = camera->getAbsolutePosition();
	v.Y = 1700.0f;
	camera->setPosition(v);
	camera->setFarValue(15000.0f);
	smgr->setActiveCamera(0);
	lastfps = -1;

device->setEventReceiver(&receiver);

	while(device->run())
	{
		driver->beginScene(true, true, video::SColor(255,90,90,156));
		smgr->drawAll();
		guienv->drawAll();
		driver->endScene();

		fps = driver->getFPS();

		wchar_t tmp[255];
		swprintf(tmp, 255, L"%s fps:%d polys:%d",
			driver->getName(),
			driver->getFPS(),
			driver->getPrimitiveCountDrawn());
		statusText->setText(tmp);
		lastfps = fps;
	}

	device->drop();
	return 0;
}


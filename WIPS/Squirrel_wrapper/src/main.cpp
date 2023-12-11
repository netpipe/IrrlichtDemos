#include "definitions.h"
#include "Scene_Interface.h"
#include "Squirrel_interface.h"

using namespace irr;

// Global variables
       IrrlichtDevice      		*device;
scene::ICameraSceneNode			*camera;
scene::ISceneManager			*smgr;
video::IVideoDriver				*driver;

Scene_Interface					*si;
Squirrel_interface				*sq;

bool							 RunGameLoop;

#define		Sydneys	   16		// number of sydey nodes to generate
u16			CurrentSydney;
bool		FirstLoop;

// -----------------------------------------------------------------------------------------------------------
class MyEventReceiver : public IEventReceiver
{
public:
	virtual bool OnEvent(const SEvent &event)
	{
	    if (event.EventType == irr::EET_KEY_INPUT_EVENT && event.KeyInput.PressedDown)
		{
			switch(event.KeyInput.Key)
			{
				case KEY_SPACE:		// screenshot
				{
					video::IImage* image = device->getVideoDriver()->createScreenShot();
					if (image)
						device->getVideoDriver()->writeImageToFile(image, "screenshot.bmp");
					image->drop();
				}
	   			break;

	   			case KEY_ESCAPE:	// end program
				{
					RunGameLoop = false;
				}
				break;
			}
		}
	return false;
	}
};


void ExecuteMotions()
{
	si->CurrentTick = device->getTimer()->getTime();
	if ((si->CurrentTick - si->LastTick) > (100 / Sydneys) )
	{

		si->setCurrentNode(1000 + CurrentSydney);
		sq->runScript("test");
		CurrentSydney++;
		if (CurrentSydney >= Sydneys)
		{
			si->SydVec = smgr->getSceneNodeFromId(1000)->getPosition();
			CurrentSydney = 0;
		}
		si->LastTick = si->CurrentTick;
	}

	// 'manual' collision and gravity
	core::vector3df campos = camera->getPosition();
	campos.Y = 0;
	if (campos.X < -1950) campos.X = -1950;
	if (campos.X >  1950) campos.X =  1950;
	if (campos.Z < -1950) campos.Z = -1950;
	if (campos.Z >  1950) campos.Z =  1950;
	camera->setPosition(campos);
}

void LoadScripting()
{
	// set up Squirrel
	si = new Scene_Interface();					// new scene class
	si->setSceneManager(smgr);					// set irr variables
	si->setCamera(camera);

	sq = new Squirrel_interface(device, si);	// new interface class
	sq->loadGlobalScript("./nuts/global.nut");	// load some scripts, a global script is also run by this function
	sq->loadScript("./nuts/test.nut", "test");

	si->LastTick = device->getTimer()->getTime(); // set a custom script variable
}

void LoadGame()
{
	// add zip archive (try to add twice to enable run from msvc)
	//device->getFileSystem()->addZipFileArchive("./mediafiles.zip");
	//device->getFileSystem()->addZipFileArchive("../mediafiles.zip");

	// add camera
	camera = smgr->addCameraSceneNodeFPS(0, 100, 250);
	camera->setFOV(6.2832f * (100.0f / 360.0f));	// 100 degrees field of view
	camera->setPosition(core::vector3df(0, 0, 0));
	camera->setTarget(core::vector3df(100, 0, 0));
	camera->setFarValue(10000);

	// add a light
	smgr->addLightSceneNode(0, core::vector3df(0, 500, 0), video::SColorf(1.0f, 1.0f, 1.0f, 0.0f), 500.0f);


	driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, false);
	smgr->addSkyBoxSceneNode(	driver->getTexture("./media/irrlicht2_up.jpg"),
								driver->getTexture("./media/irrlicht2_dn.jpg"),
								driver->getTexture("./media/irrlicht2_lf.jpg"),
								driver->getTexture("./media/irrlicht2_rt.jpg"),
								driver->getTexture("./media/irrlicht2_ft.jpg"),
								driver->getTexture("./media/irrlicht2_bk.jpg"));


	// set up 'world' without collision
	scene::IAnimatedMeshSceneNode *worldnode = smgr->addAnimatedMeshSceneNode(
			smgr->getMesh("./media/world.x"), 0, -1,
			core::vector3df(   0,  -50,    0),
			core::vector3df( 180,    0,    0),
			core::vector3df(2000, 1000, 2000));

	worldnode->setMaterialFlag(video::EMF_LIGHTING, false);

/*	// set up world with collision, gives an error on exiting program
	scene::IAnimatedMesh *worldnodemesh = 0;
	worldnodemesh = smgr->getMesh("./media/world.x");

	scene::ISceneNode* worldnode = smgr->addOctTreeSceneNode(worldnodemesh->getMesh(0));	// gives error on exiting program
	worldnode->setName("worldnode");
	worldnode->setPosition(core::vector3df(   0,  -50,    0));
	worldnode->setRotation(core::vector3df( 180,    0,    0));
	worldnode->setScale   (core::vector3df(2000, 2000, 2000));
	worldnode->setMaterialFlag(video::EMF_LIGHTING, false);

	// set up gravity
	scene::ITriangleSelector* selector = 0;
	selector = smgr->createOctTreeTriangleSelector(worldnodemesh->getMesh(0), worldnode);
	worldnode->setTriangleSelector(selector);
	selector->drop();

	scene::ISceneNodeAnimator* anim = smgr->createCollisionResponseAnimator(
		selector, camera,
		core::vector3df(50,  50, 50),
		core::vector3df( 0, -50,  0),
		core::vector3df( 0,  50,  0));

	camera->addAnimator(anim);
	anim->drop();
	*/

	// create Sydney's
	// first randomize random generator
	srand((unsigned int)time(NULL));

	scene::IAnimatedMeshSceneNode* sydnode;
	for (u8 sydney = 0; sydney < Sydneys; sydney++)
	{
			sydnode = smgr->addAnimatedMeshSceneNode(
			smgr->getMesh("./media/sydney.md2"), 0, 1000 + sydney,
			core::vector3df(	s32((rand() % 500)) + 500 + 10 * sydney,
								-10,
								s32((rand() % 500)) - 10 * sydney  ),
			core::vector3df(0, 90, 0),
			core::vector3df(0.5f, 0.5f, 0.5f));

			sydnode->setMaterialTexture(0, driver->getTexture("./media/sydney.bmp"));
			sydnode->setMD2Animation(scene::EMAT_RUN);
			sydnode->setMaterialFlag(video::EMF_LIGHTING, false);
	}
};

void RunGame()
{
	// return to initial gamestate
	device->getCursorControl()->setVisible(false);

	// set up main loop
	core::stringw		Titel;
	int					FPS, lastFPS=-1;
	u32					Now, LastTime = 0;
	RunGameLoop			= true;
	CurrentSydney		= 0;
	FirstLoop			= true;

	// run main loop
	while(device->run() && RunGameLoop)
	{
		if (device->isWindowActive() )
		{
			driver->beginScene(true, true, 0);

			if (FirstLoop)
			{
				si->LastTick = device->getTimer()->getTime();
				FirstLoop = false;
			}
			else ExecuteMotions();

			smgr->drawAll();

			driver->endScene();

			FPS = driver->getFPS();
			if (lastFPS != FPS)
			{	Titel = L"EVO's Squirrel wrapper for Irrlicht demo - FPS = ";
				Titel += FPS;
				device->setWindowCaption(Titel.c_str());
				lastFPS = FPS;
			}
		}
	}
	device->getCursorControl()->setVisible(true);
}


int main()
{

	device = createDevice(video::EDT_OPENGL, core::dimension2du(ResX, ResY), 32, false, false, false);
	if (device == 0)
		return 1; // could not create selected driver.

	MyEventReceiver receiver;
	device->setEventReceiver(&receiver);
	smgr   = device->getSceneManager();
	driver = device->getVideoDriver();

	// Now load Irrlicht settings and run game loop
	LoadGame();
	LoadScripting();
	RunGame();

	// clean up
	delete sq;
	delete si;
	device->closeDevice();
	device->drop();
	//system("PAUSE");
	return 0;
}

#include <irrlicht.h>
#include "inceptumAction.h"
#include "inceptumEnum.h"

using namespace irr;

int main()
{
	// Event Receiver // ======================================================
	MyEventReceiver receiver;

	// get Screen resolution // ===============================================
	//create a NULL device to detect screen resolution
		IrrlichtDevice *nulldevice = createDevice(video::EDT_NULL);

	core::dimension2d<u32> deskRes = nulldevice->getVideoModeList()->getDesktopResolution();

	nulldevice->drop();
	// Create device // =======================================================
	IrrlichtDevice *device; //1366, 768 //1024, 600
	int FullScreen = 1;
	if (FullScreen == 1)
	{
		device = createDevice(video::EDT_OPENGL, deskRes, 32,
		false, false, false, &receiver);
	}
	else
	{
		device = createDevice(video::EDT_OPENGL, core::dimension2d<u32>(800, 600), 32,
		false, false, true, &receiver);
	}

	// Get a pointers  // ======================================================
	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();
	gui::IGUIEnvironment* guienv = device->getGUIEnvironment();

	// Disable mip maps // =====================================================
	//driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, false);

	// Image // ================================================================
	video::ITexture* controlsPic = driver->getTexture("./media/Controls.png");
	video::ITexture* irrlichtLogo = driver->getTexture("./media/irrlichtlogo2.png");

	// Terrain // ==============================================================
	scene::ITerrainSceneNode* terrain = smgr->addTerrainSceneNode("./media/heightmap3.jpg", 0, -1,
		core::vector3df(0, 0, 0), core::vector3df(0, 0, 0), core::vector3df(1, 1, 1));
	if (terrain)
	{
		terrain->setID(NotPickable);
		terrain->setScale(core::vector3df(1.0f, 0.5f, 1.0f));
		//terrain->setPosition(core::vector3df(0, 0, 0));
		terrain->setMaterialFlag(video::EMF_LIGHTING, false);
		terrain->setMaterialTexture(0, driver->getTexture("./media/terrainTexture2.jpg"));
	}

	// Box // ==================================================================
	scene::ISceneNode* box = smgr->addCubeSceneNode();
	box->setMaterialTexture(0, driver->getTexture("./media/redPixel.bmp"));
	box->setMaterialFlag(video::EMF_LIGHTING, false);
	box->setScale(core::vector3df(5, 5, 5));
	box->setPosition(core::vector3df(-500, 1000, -500));
	//box->setScale(core::vector3df(1, 2, 1));

	// light // ================================================================
	scene::ILightSceneNode* lightSun1 = smgr->addLightSceneNode(0,
		core::vector3df(812, terrain->getHeight(812, 812) + 812, 812),
		video::SColorf(1, 1, 0.94f, 1), 5000.0f);

	// =========================================================================
	device->getCursorControl()->setVisible(false);

	// display frames per second // ============================================
	int fps;
	int lastFPS = -1;
	gui::IGUIStaticText* guiText = guienv->addStaticText(L"Hello World! This is the Irrlicht Software renderer!",
		core::rect<s32>(10, 200, 100, 222), true);
	core::stringw str;

	// Frame rate independence // ==============================================
	u32 then = device->getTimer()->getTime();
	u32 now;
	f32 tempT;
	f32 deltaTime;
	ITimer* Timer = device->getTimer();

	// Action // ===============================================================
	Action Act(&receiver, device, terrain, &deltaTime);

	while (device->run())
	{
		if (device->isWindowActive())
		{
			// Frame rate independence // ==========================================
			now = Timer->getTime();
			tempT = (f32)(now - then);
			deltaTime = (f32)(((tempT) < 60) ? tempT : 60);
			then = now;

			// =====================================================================
			Act.update();

			// Draw Scene // =======================================================
			driver->beginScene(true, true, video::SColor(255, 100, 101, 140));
			smgr->drawAll();
			guienv->drawAll();

			driver->draw2DImage(controlsPic, core::rect<s32>(10, 10, 200, 200), core::rect<s32>(0,
				0, 256, 256), 0, 0, true);
			driver->draw2DImage(irrlichtLogo, core::rect<s32>(1100, 10, 1250, 150), core::rect<s32>(0,
				0, 128, 128), 0, 0, true);

			// Selection box (must be implemented elsewhere of course
			/*driver->draw2DRectangle(video::SColor(50, 3, 67, 67), core::rect<s32>(512,
				512, 680, 680));
			driver->draw2DLine(core::position2d<s32>(512, 512), core::position2d<s32>(512, 680),
				video::SColor(255, 11, 208, 194));
			driver->draw2DLine(core::position2d<s32>(512, 680), core::position2d<s32>(680, 680),
				video::SColor(255, 11, 208, 194));
			driver->draw2DLine(core::position2d<s32>(680, 680), core::position2d<s32>(680, 512),
				video::SColor(255, 11, 208, 194));
			driver->draw2DLine(core::position2d<s32>(680, 512), core::position2d<s32>(512, 512),
				video::SColor(255, 11, 208, 194));*/

			Act.DrawCursor();
			driver->endScene();
			// Type FPS //==========================================================
			fps = driver->getFPS();
			if (lastFPS != fps)
			{
				str = L"[";
				str += driver->getName();
				str += "] FPS:";
				str += fps;

				guiText->setText(str.c_str());
				lastFPS = fps;
			}
		}
		else
			device->yield();
	}

	// Drop Device //===========================================================
	device->drop();
	return 0;
}

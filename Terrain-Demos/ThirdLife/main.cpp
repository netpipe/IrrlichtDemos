#include "app.h"

int main()
{
	//ThirdLifeEventReceiver* er = new ThirdLifeEventReceiver();
	EventReceiver er;

	IrrlichtDevice *device =
		createDevice( video::EDT_OPENGL, dimension2d<u32>(1280, 960), 16, false, false, false, &er);
	device->setResizable(true);

	//Try to create an irrlicht device using OPENGL and crash if we fail
	if (!device)
		return 1;

	//Our window title
	device->setWindowCaption(L"Hello World! - Irrlicht Engine Demo");

	//Get the needed managers so we don't need to do device->something->next all the time
	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();
	IGUIEnvironment* guienv = device->getGUIEnvironment();
	IFileSystem* filesys = device->getFileSystem();

	device->getLogger()->setLogLevel(ELL_INFORMATION);
	device->getLogger()->log("Log level now set to ELL_INFORMATION");

	//This skybox will make stuff look pretty
	ISceneNode* skyBox = 0;
	skyBox = smgr->addSkyBoxSceneNode(
			driver->getTexture("up.jpg"),
			driver->getTexture("down.jpg"),
			driver->getTexture("front.jpg"),
			driver->getTexture("back.jpg"),
			driver->getTexture("right.jpg"),
			driver->getTexture("left.jpg"));

	// Terrain
	EditableTerrain terrain = EditableTerrain(smgr, "heightmap.bmp");
	terrain.GetTerrainNode()->setMaterialFlag(video::EMF_LIGHTING, false);
	terrain.GetTerrainNode()->setMaterialTexture(0, driver->getTexture("Grass.jpg"));
	//terrain.GetTerrainNode()->setScale(vector3df(0.5f,1.f,0.5f));
	//terrain.GetTerrainNode()->setMaterialTexture(1, driver->getTexture("detailmap3.jpg"));
	//terrain.GetTerrainNode()->setMaterialType(video::EMT_DETAIL_MAP);
	terrain.GetTerrainNode()->scaleTexture(5.0f, 1.0f);

	ITriangleSelector* terrainSelector = smgr->createTerrainTriangleSelector(terrain.GetTerrainNode(), 0);

	//Set up our input handler
	InputHandler* input = new InputHandler();
	er.add(input);

	//Set our player handler and subscribe to input events
	//Player* player = new Player(smgr, L"faerie.md2", L"Faerie5.BMP");
	//input->add(player);

	ICameraSceneNode* cam = smgr->addCameraSceneNodeFPS();

	// Arrow
	ISceneNode* arrow = smgr->addAnimatedMeshSceneNode(smgr->addArrowMesh("arrow",
			SColor(255, 255, 0, 0), SColor(255, 0, 255, 0)), NULL);
	arrow->setMaterialFlag(video::EMF_LIGHTING, false);
	arrow->setRotation(vector3df(0,0,180));

	//Set up a terraform window
	GUIManager* guiman = new GUIManager(guienv);
	guiman->getTerraformWindow()->setVisible(true);
	er.add(guiman);

	bool wireframe = false;

	u32 nextAction = 0;
	u32 now=0,last=0;
	f32 delta;
	int lastFPS=0;
	//While our device is still up and running
	while(device->run())
	{
		//For framerate independant camera movement
		now = device->getTimer()->getTime();    // Set the current time
		delta   = (f32)(now - last);
		//player->updateDelta(delta);

		//Lock event handler
		input->endEventProcess();

		if (!device->isWindowActive())
			device->yield();

		int fps = driver->getFPS();

        if (lastFPS != fps)
        {
                core::stringw str = L"[";
                str += driver->getName();
                str += "] FPS:";
                str += fps;

                device->setWindowCaption(str.c_str());
                lastFPS = fps;
        }

		//Start a new scene
		driver->beginScene(true, true, SColor(255,100,101,140));

		if (input->keyPressed(irr::KEY_ESCAPE)) device->closeDevice();

	    if (input->keyPressed(irr::KEY_KEY_W))
		{
			wireframe = !wireframe;
			terrain.GetTerrainNode()->setMaterialFlag(video::EMF_WIREFRAME, wireframe);
		}

		if(input->keyPressed(irr::KEY_KEY_S))
			terrain.Save(driver);


		// move the arrow to the nearest vertex ...
		const position2di clickPosition = device->getCursorControl()->getPosition();
		const line3df ray = smgr->getSceneCollisionManager()->getRayFromScreenCoordinates(clickPosition, cam);
		vector3df pos;
		triangle3df Tri;
		ISceneNode* node;
		if (smgr->getSceneCollisionManager()->getCollisionPoint(ray, terrainSelector, pos, Tri, (irr::scene::ISceneNode *&)node))
		{
			//arrow->setPosition(pos);
			static const f32 scale = 1.f; // terrain is scaled 1x
			static const s32 size = 256; // heightmap is 256X256 pixels
			f32 x = (f32)pos.X;
			f32 z = (f32)pos.Z;

			// ... Move it if clicked
			if((input->leftMouseDown() || input->rightMouseDown()) && nextAction < now)
			{
				nextAction = now+150; //Only do actions every x ms to avoid insta raising but allow keeping the mouse pressed
				terrain.RaiseVertexGrid((s32)round(x), (s32)round(z), size, guiman->getTerraformWindow()->getTerraformSizeScrollbar()->getPos(), (input->leftMouseDown() ? 1 : -1));
			}
			else if (input->middleMouseDown() && nextAction < now)
			{
				nextAction = now+150; //Only do actions every x ms to avoid insta raising but allow keeping the mouse pressed
				terrain.SmoothGrid((s32)round(x), (s32)round(z), size, guiman->getTerraformWindow()->getTerraformSizeScrollbar()->getPos());
			}

			terrain.DrawSelectionGrid((f32)round(x), (f32)round(z), guiman->getTerraformWindow()->getTerraformSizeScrollbar()->getPos());
		}



		//Draw everything
		smgr->drawAll();
		guienv->drawAll();

		//End the scene
		driver->endScene();

		//Unlock input handler
		input->startEventProcess();

		last=now;
	}

	device->drop();

	return 0;
}

// Vehement (C) Steven Dorman 2008
#define	 VEHEMENT_FACTORY_CPP

// Inlude the class definition
#include "factory.hpp"

// Using the vehe namespace
namespace vehe 
{
	//! Creates or returns the singleton instance
	dfactory * dfactory::instance()
	{
		if (!dinstance)
		  dinstance = new dfactory;

		return dinstance;
	}

	//! Creates the irrlicht window
	void dfactory::init (int width, int height, bool fullscr,  IEventReceiver*	recv)
	{
		video::E_DRIVER_TYPE driverType;

		printf("Please select the desired rendering driver:\n"\
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
			default:  driverType = video::EDT_SOFTWARE; break;
		}	

		//Set up the pointers
		device = createDevice(driverType,
				 core::dimension2d<s32>(width, height),
				 32,fullscr,false,false);

		guienv = device->getGUIEnvironment();
		driver = device->getVideoDriver();
		smgr   = device->getSceneManager();
		audio  = createIrrKlangDevice();	

		//Setup the keyboard event receiver
		device->setEventReceiver(recv);

		//Create the game state manager
		gamestate = new statemanager();

		//Create new map editor class
		editor = new mapeditor();

		//Create new player manager class
		tplayer = new playermanager();

		//Create new scripting manager class
		tscript = new scripting();
	
		dfactory::start();
	}

	//! Start the engine
	void dfactory::start ()
	{
		//Always use 32 bit textures
		driver->setTextureCreationFlag(video::ETCF_ALWAYS_32_BIT, true);
		
		//Create the meta triangle selector instance
		meta = smgr->createMetaTriangleSelector(); 

		//Set Fog
		driver->setFog(video::SColor(0,235,235,235), true, 75.0f, 3750.0f, 250.1000000f, true);

		//Load the font graphics
		IGUISkin* skin = guienv->getSkin();
		IGUIFont* font = guienv->getFont("./data/graphics/gui/font.bmp");
		if (font)
			skin->setFont(font);
		skin->setFont(guienv->getBuiltInFont(), EGDF_TOOLTIP);

	}


	/* Load the billboard for out target icon */
	void dfactory::createBillBoard()
	{
		bill = smgr->addBillboardSceneNode();
		bill->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR );
		bill->setMaterialTexture(0, driver->getTexture("./data/graphics/gui/targeticon.jpg"));
		bill->setMaterialFlag(video::EMF_LIGHTING, false);
		bill->setMaterialFlag(video::EMF_ZBUFFER, false);
		bill->setSize(core::dimension2d<f32>(31.0f, 31.0f));
	}

	//! Begin Drawing
	void dfactory::beginDraw() 
	{
		//Yeild the CPU
		device->sleep(1);

		//Begin drawing the scene
		driver->beginScene(true, true, SColor(0,200,200,200));

		//Advance the physics simulation
		if (!ipause)
			p_world->update();

		//Draw the entire scene
		smgr->drawAll();

		//Draw physics debug infos?
		if (idebug)
			p_world->drawAllDebugInfos();
	}

	//! Finish Drawing
	void dfactory::finishDraw() 
	{
		//Draw the Graphic User Interface
		guienv->drawAll();
		//Finish drawing the scene
		driver->endScene();
	}

	//! Setup the Camera
	void dfactory::createCamera(vector3df pos) 
	{
			//WASD Configuration
			SKeyMap keyMap[4]; 
			keyMap[0].Action=EKA_MOVE_FORWARD;   keyMap[0].KeyCode=KEY_KEY_W; 
			keyMap[1].Action=EKA_MOVE_BACKWARD;  keyMap[1].KeyCode=KEY_KEY_S; 
			keyMap[2].Action=EKA_STRAFE_LEFT;    keyMap[2].KeyCode=KEY_KEY_A; 
			keyMap[3].Action=EKA_STRAFE_RIGHT;   keyMap[3].KeyCode=KEY_KEY_D; 

			//Add the camera
			camera = smgr->addCameraSceneNodeFPS(0,100,380,-1,keyMap,4,false,0);
			camera->setPosition(pos);
			camera->setFarValue(3333.0f);

			//Add irrlicht standard collision camera animator
			ISceneNodeAnimator* anim = smgr->createCollisionResponseAnimator(
			meta, camera, core::vector3df(45,80,45),
				core::vector3df(0,-0.8f,0), 
				core::vector3df(0,6,0));
			camera->addAnimator(anim);
			anim->drop();

			//Second camera (free mouse)
			camera2 = smgr->addCameraSceneNode();
			camera2->setFarValue(3333.0f);

			//Third Camera (roming cam)
			camera3 = smgr->addCameraSceneNodeFPS(0,150.0f,800.0f,-1,keyMap,4);
			camera3->setFarValue(5000.0f);

			//Set active camera
			smgr->setActiveCamera(camera);
	}

	//! Switch the camera to free the mouse
	void dfactory::freeMouse()
	{
		if (activecamera==0) {
			activecamera=1;
			device->getCursorControl()->setVisible(true);
			camera->updateAbsolutePosition();
			camera2->setPosition(camera->getAbsolutePosition());
			camera2->setTarget(camera->getTarget());
			smgr->setActiveCamera(camera2);}
		else if (activecamera==1) {
			activecamera=0;
			device->getCursorControl()->setVisible(false);
			smgr->setActiveCamera(camera);}
		else if (activecamera==2){
			activecamera =0;
			camera->setPosition(camera3->getPosition());
			smgr->setActiveCamera(camera);
		}
	}

	//! Setup the SkyDome
	void dfactory::createSkyDome() 
	{
		//Create the SkyDome
		device->getFileSystem()->addZipFileArchive("./data/graphics/sky.dat");
		smgr->addSkyDomeSceneNode(driver->getTexture("mysky.jpg"),16,16,1.0f,2.0f);
	}

	//! Reset Camera
	void dfactory::resetCamera()
	{
		camera->remove();
		camera2->remove();
		camera3->remove();
		dfactory::createCamera(currentmap.playerstart);
	}

	//! Roaming Camera
	void dfactory::roamingCamera()
	{
		camera3->setPosition(camera->getPosition());
		smgr->setActiveCamera(camera3);
		activecamera = 2;
	}

	//! Push and object with your miiiiind
	void dfactory::pushObject(int myid)
	{
		if (myid != 666 && myid != -1 && !editor->getActive() &&
			!entities.at(myid).body->isAffectedByForce())
			entities.at(myid).body->addLocalForce(camera->getTarget(),camera->getTarget());
		
	}

	//! Delete a directory
	bool dfactory::deleteDirectory(LPCTSTR lpszDir, bool noRecycleBin)
	{
	  int len = _tcslen(lpszDir);
	  TCHAR *pszFrom = new TCHAR[len+2];
	  _tcscpy(pszFrom, lpszDir);
	  pszFrom[len] = 0;
	  pszFrom[len+1] = 0;
	  
	  SHFILEOPSTRUCT fileop;
	  fileop.hwnd   = NULL;    // no status display
	  fileop.wFunc  = FO_DELETE;  // delete operation
	  fileop.pFrom  = pszFrom;  // source file name as double null terminated string
	  fileop.pTo    = NULL;    // no destination needed
	  fileop.fFlags = FOF_NOCONFIRMATION|FOF_SILENT;  // do not prompt the user
	  
	  if(!noRecycleBin)
		fileop.fFlags |= FOF_ALLOWUNDO;

	  fileop.fAnyOperationsAborted = FALSE;
	  fileop.lpszProgressTitle     = NULL;
	  fileop.hNameMappings         = NULL;

	  int ret = SHFileOperation(&fileop);
	  delete [] pszFrom;  
	  return (ret == 0);
	}


	
};
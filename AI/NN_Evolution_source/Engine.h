
#define WINDOW_CAPTION	L"Vehicle Evolution"
#define MEDIA_DIR		"media"
#define FRAME_RATE		30.f

// This core Engine class contains the main loop and some handy functions to access
// the functionality of Irrlicht.
class Engine
{
	IrrlichtDevice			*device;
	video::IVideoDriver		*driver;
	gui::IGUIEnvironment	*guienv;
	scene::ISceneManager	*smgr;
	ITimer					*timer;
	gui::ICursorControl		*curcon;
	io::IFileSystem			*filesys;
	ILogger					*logger;

	GUI *gui;
	World *world;
	CameraController *cameraController;
	ScreenLog *screenLog;
	bool fastForward;

public:

	IrrlichtDevice			*getDevice()			{ return device; }
	video::IVideoDriver		*getVideoDriver()		{ return driver; }
	gui::IGUIEnvironment	*getGUIEnvironment()	{ return guienv; }
	scene::ISceneManager	*getSceneManager()		{ return smgr; }
	ITimer					*getTimer()				{ return timer; }
	gui::ICursorControl		*getCursorControl()		{ return curcon; }
	io::IFileSystem			*getFileSystem()		{ return filesys; }
	ILogger					*getLogger()			{ return logger; }

	GUI						*getGUI()				{ return gui; }
	World					*getWorld()				{ return world; }
	CameraController		*getCameraController()	{ return cameraController; }
	ScreenLog				*getScreenLog()			{ return screenLog; }

	void setFastForward(bool state)
	{
		fastForward = state;
	}

	void main()
	{
		// Before creating the window, find out the desktop resolution
		device = createDevice(video::EDT_NULL);
		core::dimension2d<s32> res = core::dimension2d<s32> (800,600);// device->getVideoModeList()->getDesktopResolution();
		device->drop();

		// Initialise the Irrlicht window at the desktop resolution
		device = createDevice(video::EDT_OPENGL,
			res,
			32,
			false,
			false,false);

		// Save Irrlicht pointers for easier access
		guienv	= device->getGUIEnvironment();
		driver	= device->getVideoDriver();
		smgr	= device->getSceneManager();
		timer	= device->getTimer();
		curcon	= device->getCursorControl();
		filesys	= device->getFileSystem();
		logger	= device->getLogger();

		filesys->addFolderFileArchive(MEDIA_DIR);

		device->setWindowCaption(WINDOW_CAPTION);

		fastForward = false;

		// These objects may call Irrlicht methods while destructing, so they need
		// to be deleted before Irrlicht shuts down.
		screenLog			= new ScreenLog();
		cameraController	= new CameraController;
		world				= new World();
		gui					= new GUI(world, cameraController);

		// The simulation is run with a fixed timestep. This is necessary so that the AI
		// performs consistently, with the ANN getting updating the same number of times each second.
		// It also allows for "fast forwarding" which is when, if the processor is fast enough, the
		// simulation can be run faster than real time by performing more than one update within the
		// length of the time step.

		// To make the visuals smoother it would be possible to run the graphics rendering at a higher
		// frame rate than the AI etc, but this would require the object transformations to be interpolated
		// between updates. The current method is really good enough for this application.

		float timeStep = 1.0 / FRAME_RATE;
		u32 lastUpdateTimeMs = timer->getTime();

		while (device->run())
		{
			if (true)//device->isWindowActive())
					// this "if" is disabled so that evolution occurs even when
					// the program is minimised. Useful if you want to let it evolve
					// in the background while doing other things...
			{
				bool readyForUpdate = timer->getTime() - lastUpdateTimeMs > timeStep*1000.0;

				if (readyForUpdate || fastForward)
				{
					// Update (AI, movement etc...)
					// When in "fast forward" mode, this occurs as fast as possible, irrespective of whether
					// the defined timestep has actually passed. Limited by the processor.
					world->update(timeStep);

					// but Drawing etc. is always done at the same, predefined, frame rate
					if (readyForUpdate)
					{
						cameraController->draw(timeStep);

						driver->beginScene(true, true, video::SColor(0,0,0,0));

						// Vehicle sensors are drawn manually as they are not Irrlicht scene nodes.
						world->draw(timeStep);

						// Draw all Irrlicht scene nodes
						smgr->drawAll();

						// Draw GUI
						guienv->drawAll();

						driver->endScene();

						// Save the time of the last, not-fast-forwarding, update.
						lastUpdateTimeMs = timer->getTime();
						device->sleep(20,0);
					}
					else
					{
						// Must update all scene nodes for collision detection etc...
						// (Irrlicht usually does this above in drawAll(), but in this case
						// drawAll has not been called so it must be done manually)
						smgr->getRootSceneNode()->OnAnimate(timer->getTime());
					}
				}
				else
				{
                    device->yield();
				}
			}
			else
			{
				device->yield();
			}
		}

		delete gui;
		delete world;
		delete cameraController;
		delete screenLog;

		// Shut down Irrlicht
		device->drop();
	}
};

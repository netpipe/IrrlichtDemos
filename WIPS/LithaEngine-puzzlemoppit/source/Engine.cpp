
#include "Engine.h"
#include "World.h"
#include "LogicTask.h"
#include "RenderTask.h"
#include "Kernel.h"
#include "OpenALSoundSystem.h"
#include "Updater.h"
#include "EventQueue.h"
#include "Event.h"
#include "IWantEvents.h"
#include "IInputProfile.h"

//#if defined(_IRR_COMPILE_WITH_X11_DEVICE_)
//#include <X11/Xlib.h> // for XMoveWindow
//#endif

IEngine *engineInstance = NULL;

IEngine *CreateEngine(int argc, const char **argv, const VariantMap *settings)
{
	ASSERT(!engineInstance);
	
	new Engine(argc, argv, settings);
	
	// Engine sets engineInstance in its constructor.
	ASSERT(engineInstance);
	
	return engineInstance;
}

IEngine *GetEngine()
{
	ASSERT(engineInstance);
	return engineInstance;
}

// Default engine settings are defined here.
VariantMap Engine::GetDefaultSettings()
{
	VariantMap defaultSettings;
	defaultSettings["appName"] = "Litha Engine";
	defaultSettings["screenWidth"] = 800;
	defaultSettings["screenHeight"] = 600;
	defaultSettings["fullScreen"] = false;
	defaultSettings["softwareMode"] = false;
	defaultSettings["shadersEnabled"] = true;
	defaultSettings["postProcessingEnabled"] = true;
	defaultSettings["vsync"] = true;
	defaultSettings["maxRenderFPS"] = 60;
	return defaultSettings;
}

VariantMap Engine::GetCreationSettings()
{
	ASSERT( initSettings.count("appName") );
	return initSettings;
}

io::path Engine::GetSettingsPath()
{
	// must not be called before initSettings has been set.
	ASSERT( initSettings.count("appName") );
	return os::path::concat(GetLocalSettingsDir(), initSettings["appName"] + ".ini");
}

Engine::Engine(int argc, const char **argv, const VariantMap *settings)
{
	restartOnExit = false;
	
	// Set this pointer first, as some objects below will want to call GetEngine
	// to access some Engine methods. (e.g. GetEngineTime).
	engineInstance = this;
	
	// Set up default engine init settings
	initSettings = GetDefaultSettings();
	
	// Now override those with project specific settings, if given.
	if (settings)
		override_variantmap(initSettings, *settings);
	
	// And then, any settings set in the config file will override everything else.
	// Since appName is used here, that is the one thing that cannot be specified by the
	// end user in the config file. (well it *can* be, but don't tell the user that..)
	
	io::path settingsFullPath = GetSettingsPath();
	VariantMap configFileSettings = file::loadsettings(settingsFullPath);
	
	if (configFileSettings.size())
		override_variantmap(initSettings, configFileSettings);
	
	// Cache the original starting directory before the engine changes elsewhere.
	startDir = os::getcwd();
	
	// Find the exe path
	// We use getexecutablepath instead of argv[0] as we want a full absolute path.
	// Also getexecutablepath resolves symbolic links on Linux.
	io::path exePath = os::getexecutablepath();
	io::path exeDir = os::path::dirname(exePath);
	
	
	// Initialise logging
	// We use the full path just in case the working directory is not the same
	// as the exe dir.
	
	io::path logName; // just the log name, not a full path.
	
	if (initSettings.count("logFile"))
		logName = initSettings["logFile"];
	else
		logName = initSettings["appName"] + ".log";
	
	io::path logPath = os::path::concat(exeDir, logName);
	io::path logPath2 = "";
	
#ifdef __APPLE__
	// Mustn't write to bundle directory...
	logPath = os::path::concat(GetLocalSettingsDir(), logName);
#endif
	
	utils::log::setfile(logPath);
	
#ifdef _IRR_LINUX_PLATFORM_
	
	// also log to /var/log on Linux?
	// (assuming we can write there of course...???
	logPath2 = os::path::concat("/var/log", logName);
	utils::log::setfile2(logPath2);
	
#endif
	
	
	// Some initial notes on the log
	
	NOTE << "Litha Game Engine";
	
	for (int i = 0; i < argc; i ++)
	{
		NOTE << "Arg: " << argv[i];
		
		if (i > 0) // don't save exe name
			args.push_back(argv[i]);
	}
	
	NOTE << "Actual executable path: " << exePath;
	NOTE << "Settings path: " << settingsFullPath;
	
	NOTE << "Logging to " << logPath;
	
	if (logPath2.size())
		NOTE << "Also logging to " << logPath2;
	
	NOTE << "Original starting path: " << startDir;
	
	
	// Query current desktop resolution...
	{
		NOTE << "Querying current desktop resolution...";
		
		IrrlichtDevice *device = createDevice(video::EDT_NULL);
		
		if (device)
		{
			cachedDesktopResolution = device->getVideoModeList()->getDesktopResolution();
			
			NOTE << "Cached original resolution: " << cachedDesktopResolution.Width
					<< "x" << cachedDesktopResolution.Height;
			
			device->drop();
		}
		else
			WARN << "Failed to create Irrlicht NULL Device.";
	}
	
	
	// Create the Irrlicht device...
	
	/*device = createDevice(initSettings["softwareMode"] ? video::EDT_BURNINGSVIDEO : video::EDT_OPENGL,
		core::dimension2du(initSettings["screenWidth"], initSettings["screenHeight"]),
		32,
		initSettings["fullScreen"],
		false,
		initSettings["vsync"],
		this);*/
	
	SIrrlichtCreationParameters deviceParams;
	
	deviceParams.DriverType = initSettings["softwareMode"] ? video::EDT_BURNINGSVIDEO : video::EDT_OPENGL;
	deviceParams.WindowSize = core::dimension2du(initSettings["screenWidth"], initSettings["screenHeight"]);
	deviceParams.Bits = 32;
	deviceParams.Fullscreen = initSettings["fullScreen"];
	deviceParams.Stencilbuffer = false;
	deviceParams.Vsync = initSettings["vsync"];
	deviceParams.EventReceiver = this;
	
#ifdef _IRR_LINUX_PLATFORM_
	// test SDL to see if it will fix screen mode / focus bug.
	// we don't use SDL with software mode so config app works properly... (SDL seemed to break video mode listing)
	//deviceParams.DeviceType = initSettings["softwareMode"] ? EIDT_BEST : EIDT_SDL;
#endif
	
	device = createDeviceEx(deviceParams);
	
	
	// Set working directory to the one the application resides in.
	// (since depending on how the application is launched the working directory may vary)
	// We call chdir here after logging has been set up, since chdir will output a log message
	// when the working directory is changed.
	// This is also moved below all calls to createDevice, since the Mac implementation
	// of the device calls chdir to the directory above the current one.
	if (exeDir.size())
		os::chdir(exeDir);
	
	NOTE << "Current directory is now " << os::getcwd();
	
	
	NOTE << "Fullscreen: " << initSettings["fullScreen"].To<bool>();
	NOTE << "VSync: " << initSettings["vsync"].To<bool>();
	NOTE << "Max render FPS: " << initSettings["maxRenderFPS"].To<f32>();
	
	if (!device)
		FAIL << "Could not create Irrlicht Device.";
	
	// Centre the app window, if not fullscreen.
	
	if (!initSettings["fullScreen"])
	{
#if defined(_IRR_WINDOWS_API_)
		// Windows seems to automatically centre the window?
#elif defined(__APPLE__)
		// Maybe Mac OS centres window automatically? Haven't tested...
#elif defined(_IRR_COMPILE_WITH_X11_DEVICE_)
		// Linux
		// PROBLEM: Obviously cannot get video data "OpenGLLinux" when using software renderer...
		// So cannot centre window...
		// --> But actually, should we even centre the window?
		// Or should we let X11 intelligently choose the position...?
		/*
		const video::SExposedVideoData &videoData = device->getVideoDriver()->getExposedVideoData();
		
		ASSERT( videoData.OpenGLLinux.X11Display );
		ASSERT( videoData.OpenGLLinux.X11Window );
		
		XMoveWindow((Display *)videoData.OpenGLLinux.X11Display, (Window)videoData.OpenGLLinux.X11Window,
				0, 0);
		*/
#endif
	}
	
	// Set window title
	
	if (initSettings.count("windowTitle"))
		SetWindowTitle(initSettings["windowTitle"].To<core::stringc>());
	else // Default to appName if no title is given in the settings
		SetWindowTitle(initSettings["appName"].To<core::stringc>());
	
	
	// Needed elsewhere
	filesys = device->getFileSystem();
	
	// Set warning level... (don't want *all* information even in debug mode)
	device->getLogger()->setLogLevel(ELL_WARNING);
	
	// Time
	timer = device->getTimer();
	engineStartTime = timer->getRealTime();
	
	// Random seed!
	srand( timer->getRealTime() );
	
	// Clear buttons to zero
	ClearButtonStates();
	
	// force b3d mipmap flag always enabled
	device->getSceneManager()->getParameters()->setAttribute(scene::B3D_LOADER_IGNORE_MIPMAP_FLAG, true);
	
	// this light and camera scene node is temporary!
	scene::ILightSceneNode *light = device->getSceneManager()->addLightSceneNode(NULL,
		core::vector3df(0,5,0),
		video::SColorf(), 6.0);
	video::SLight &slight = light->getLightData();
	slight.AmbientColor = video::SColorf(0.5,0.5,0.4);
	slight.DiffuseColor = video::SColorf(0.7,0.7,0.3);
	slight.SpecularColor = video::SColorf(0.5,0.5,0.5);
	
	// Use Irrrlicht 1.4.2 setting...
	slight.Attenuation.set(1,0,0);
	
	scene::ICameraSceneNode *camera = device->getSceneManager()->addCameraSceneNode();//FPS(0, 100.f, 50.f);
	camera->bindTargetAndRotation(true); // So setTarget is interpolated.
	
	//hacked in for PuzzleGame. This should be configurable.
	camera->setNearValue(0.1);
	
	// cursor control
	curcon = device->getCursorControl();
	curcon->setVisible(false);
	
	// centre the mouse once initially
	CentreMouse();
	
	// don't auto centre the mouse by default
	SetAutoMouseCentring(false,false);
	
	
	kernel = new Kernel();
	
	world = new World();
	soundSystem = new OpenALSoundSystem();
	
	logicTask = new LogicTask();
	kernel->AddTask(logicTask);
	
	renderTask = new RenderTask(world);
	kernel->AddTask(renderTask);
	
	// Set the time stepping methods for the tasks
	logicTask->SetSteppingParams(true, 0.01);
	renderTask->SetSteppingParams(false, 1.0 / initSettings["maxRenderFPS"].To<f32>());
	
	// Set world to be updated by Logic task
	logicTask->GetUpdater().AddUpdatable(world);
	
	
	// Now render system has been set up.
	
	// Shaders enabled?
	if (!initSettings["shadersEnabled"])
		GetRenderSystem()->ForceNoShaders(true);
	
	// Post processing?
	// Although this probably wouldn't work without shaders as PP is generally done with shaders.
	if (!initSettings["postProcessingEnabled"])
		GetRenderSystem()->ForceNoPostProcessing(true);
}

Engine::~Engine()
{
	// Tasks will already have been Removed from kernel, since engine.Run
	// (and thus Kernel.Run) must be over to reach here.
	// However the logic and render tasks are not destructed yet since we kept a reference to them.
	
	// Remove task IUpdatables first, since they are user implemented loops
	// that may make use of World, Sound System etc...
	logicTask->GetUpdater().RemoveAllUpdatablesRecursive();
	renderTask->GetUpdater().RemoveAllUpdatablesRecursive();
	
	// Remove World updatables for similar reason
	// (don't want them calling methods of World as world is destructing)
	world->GetUpdater().RemoveAllUpdatablesRecursive();
	
	// remove the world and all transformables etc
	// remove it before tasks as it may contain stuff that has IUpdatables
	// (and thus call task->RemoveUpdatable on destruction)
	world->drop();
	
	// drop sound system after world as world may contain sounds
	// probably ought to move this to below dropping of tasks as well (since they might have sounds...)
	soundSystem->drop();
	
	// Tasks destructed after everything else.
	// Since at least the World may contain a logic IUpdatable (Sound Source with a SoundQueue),
	// and so will call logicTask->RemoveUpdatable on destruction.
	logicTask->drop();
	renderTask->drop();
	
	kernel->drop();
	device->drop();
	
	NOTE << "Litha Engine appears to have shut down successfully.";
	
	if (onExitWebPage.size())
		os::openwebpage(onExitWebPage);
	
	if (restartOnExit)
	{
		NOTE << "Application restart requested...";
		
		io::path exePath = os::getexecutablepath();
		
		NOTE << "Executable path: " << exePath;
		
		for (u32 i = 0; i < args.size(); i ++)
			NOTE << "Arg: " << args[i];
		
		utils::log::backup(".old");
		
#ifdef __APPLE__
		// We want a delay to allow time for resolution to be reset.
		if (!os::run(exePath, args, 3))
			WARN << "Restart failed.";
#else
		if (!os::run(exePath, args))
			WARN << "Restart failed.";
#endif
	}
}

IrrlichtDevice *Engine::GetIrrlichtDevice()
{
	return device;
}

IWorld *Engine::GetWorld()
{
	return world;
}

IRenderSystem *Engine::GetRenderSystem()
{
	return renderTask;
}

ISoundSystem *Engine::GetSoundSystem()
{
	return soundSystem;
}

IUpdater *Engine::CreateUpdater()
{
	return new Updater();
}

IEventQueue *Engine::CreateEventQueue()
{
	return new EventQueue();
}

f32 Engine::GetEngineTime()
{
	// We use the *real* system time, and subtract the time when starting. This gives us a virtual
	// time that starts at zero when the Engine starts.
	// We don't use Irrlicht's virtual timer, because that needs to be stopped sometimes (e.g. to pause graphics).
	return (timer->getRealTime() - engineStartTime) * 0.001;
}

f32 Engine::GetLogicInterpolationAlpha()
{
	return logicTask->GetInterpolationAmount();
}

void Engine::Run()
{
	// Kernel deals with pausing.
	// (checks engine->IsPaused())
	kernel->Run();
}

void Engine::Exit(const core::stringc &pageURL)
{
	kernel->Exit();
	
	if (pageURL.size())
		onExitWebPage = pageURL;
}

void Engine::Restart()
{
	kernel->Exit();
	restartOnExit = true;
}

bool Engine::WillRestart()
{
	return restartOnExit;
}

void Engine::OnPause()
{
	pauseTime = timer->getRealTime();
	
	// Need to pause other stuff here..?
	// e.g. sound, world...
	
	world->Pause();
}

void Engine::OnResume()
{
	// Account for the length of time we were paused for.
	engineStartTime += timer->getRealTime() - pauseTime;
	
	// If there are any buttons down, we need to send fake button up events.
	// (since all button states are cleared below)
	for (u32 i = 0; i < KEY_KEY_CODES_COUNT; i ++)
	{
		if (buttonStates[i])
		{
			Event event("ButtonUp");
			event["button"] = i;
			PostEvent(event);
		}
	}
	
	// Clear button states
	// (since there may have been key up events while app was out of focus)
	ClearButtonStates();
	
	// Resume other stuff
	world->Resume();
}

void Engine::PushWorkingDirectory(const io::path &path)
{
	io::path oldWD = filesys->getWorkingDirectory();
	
	if (filesys->changeWorkingDirectoryTo(path))
	{
		NOTE << "Pushed working directory: " << path;
		pushedWDs.push(oldWD);
	}
	else
	{
		WARN << "Failed to push directory (" << path << ")";
	}
}

io::path Engine::GetStartingDirectory()
{
	return startDir;
}

void Engine::PopWorkingDirectory()
{
	if (pushedWDs.empty())
	{
		WARN << "Directory stack empty.";
		return;
	}
	
	if (!filesys->changeWorkingDirectoryTo(pushedWDs.top()))
		WARN << "Failed to pop directory (" << pushedWDs.top() << ")";
	else
		NOTE << "Popped working directory: " << pushedWDs.top();
	
	pushedWDs.pop();
}

io::path Engine::GetLocalSettingsDir()
{
	return os::getcustomappdata(initSettings["appName"]);
}

void Engine::RegisterEventInterest(IWantEvents *receiver, const core::stringc &eventName)
{
	specificEventInterest[eventName].Insert(receiver);
}

void Engine::UnregisterEventInterest(IWantEvents *receiver, const core::stringc &eventName)
{
	specificEventInterest[eventName].Remove(receiver);
}

void Engine::RegisterAllEventInterest(IWantEvents *receiver)
{
	allEventInterest.Insert(receiver);
}

void Engine::UnregisterAllEventInterest(IWantEvents *receiver)
{
	allEventInterest.Remove(receiver);
	
	// also remove from all specific event interest sets
	for (std::map<core::stringc, Set<IWantEvents *> >::iterator i = specificEventInterest.begin();
			i != specificEventInterest.end(); i ++)
	{
		i->second.Remove(receiver);
	}
}

void Engine::PostEvent(const Event &event)
{
	Set<IWantEvents *> recipients;
	
	// Add those interested in all events
	recipients.Union(allEventInterest);
	
	// Add those interested in just this event
	if (specificEventInterest.count(event.GetTypeName()))
		recipients.Union(specificEventInterest[event.GetTypeName()]);
	
	// Send the events!
	
	// POTENTIAL PROBLEM:
	// One OnEvent may delete a recipient later in the list.
	// So a pointer may become invalid.
	
	for (u32 i = 0; i < recipients.size(); i ++)
		recipients[i]->OnEvent(event);
}

void Engine::QueueEvent(const Event &event, f32 delay)
{
	eventQueue.push_back(event);
	eventQueue.back()["___PostTime___"] = GetEngineTime() + delay;
}

void Engine::ProcessEventQueue()
{
	f32 currentTime = GetEngineTime();
	
	std::vector<Event> readyEvents;
	
	// Find events that are ready for sending.
	for (u32 i = 0; i < eventQueue.size(); i ++)
	{
		if ( eventQueue[i]["___PostTime___"].To<f32>() <= currentTime )
		{
			readyEvents.push_back(eventQueue[i]);
			eventQueue.erase(eventQueue.begin()+i);
			i --;
		}
	}
	
	// And then send them...
	for (u32 i = 0; i < readyEvents.size(); i ++)
		PostEvent(readyEvents[i]);
}

void Engine::ClearButtonStates()
{
	buttonStates.Clear();
}

bool Engine::GetButtonState(s32 button)
{
	return buttonStates[button];
}

ButtonStates Engine::GetButtonStates()
{
	return buttonStates;
}

void Engine::SetButtonState(s32 button, bool state)
{
	buttonStates[button] = state;
}

bool Engine::OnEvent(const SEvent &irrEvent)
{
	// We're going to eat up Irrlicht's log events and output them ourselves.
	if (irrEvent.EventType == EET_LOG_TEXT_EVENT)
	{
		NOTE << "Irrlicht says: " << irrEvent.LogEvent.Text;
		return true;
	}
	
	if (IsPaused())
		return false;
	
	std::vector<Event> newEvents;
	
	ButtonStates lastButtonStates = buttonStates;
	
	// Maintain button states
	if (irrEvent.EventType == EET_MOUSE_INPUT_EVENT)
	{
		// Mouse buttons
		if (irrEvent.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN)
			buttonStates[KEY_LBUTTON] = true;
		else if (irrEvent.MouseInput.Event == EMIE_LMOUSE_LEFT_UP)
			buttonStates[KEY_LBUTTON] = false;
		else if (irrEvent.MouseInput.Event == EMIE_RMOUSE_PRESSED_DOWN)
			buttonStates[KEY_RBUTTON] = true;
		else if (irrEvent.MouseInput.Event == EMIE_RMOUSE_LEFT_UP)
			buttonStates[KEY_RBUTTON] = false;
		else if (irrEvent.MouseInput.Event == EMIE_MMOUSE_PRESSED_DOWN)
			buttonStates[KEY_MBUTTON] = true;
		else if (irrEvent.MouseInput.Event == EMIE_MMOUSE_LEFT_UP)
			buttonStates[KEY_MBUTTON] = false;
	}
	else if (irrEvent.EventType == EET_KEY_INPUT_EVENT)
	{
		if (irrEvent.KeyInput.Key < KEY_KEY_CODES_COUNT)
			buttonStates[irrEvent.KeyInput.Key] = irrEvent.KeyInput.PressedDown;
		else
			WARN << "Invalid key event received (" << irrEvent.KeyInput.Key << ")";
	}
	
	// Determine whether to send button up/down events
	// We don't send on every Irrlicht key event since Irrlicht uses key repeat.
	for (u32 i = 0; i < KEY_KEY_CODES_COUNT; i ++)
	{
		if (buttonStates[i] != lastButtonStates[i])
		{
			if (buttonStates[i])
			{
				Event event("ButtonDown");
				event["button"] = i;
				newEvents.push_back(event);
			}
			else
			{
				Event event("ButtonUp");
				event["button"] = i;
				newEvents.push_back(event);
			}
		}
	}
	
	// Generate mouse AxisMoved events
	if (irrEvent.EventType == EET_MOUSE_INPUT_EVENT)
	{
		if (irrEvent.MouseInput.Event == EMIE_MOUSE_WHEEL)
		{
			Event event("AxisMoved");
			event["axis"] = AXIS_MOUSE_WHEEL;
			event["delta"] = irrEvent.MouseInput.Wheel;
			newEvents.push_back(event);
		}
		else if (irrEvent.MouseInput.Event == EMIE_MOUSE_MOVED)
		{
			// I think there was some bug once with values from the irrEvent structure,
			// so we query the mouse ourselves.
			core::position2df newMousePos = curcon->getRelativePosition();
			
			f32 mouseDeltaX = newMousePos.X - lastMousePos.X;
			f32 mouseDeltaY = newMousePos.Y - lastMousePos.Y;
			
			bool xChanged = mouseDeltaX != 0.f;
			bool yChanged = mouseDeltaY != 0.f;
			
			if (xChanged)
			{
				Event event("AxisMoved");
				event["axis"] = AXIS_MOUSE_X;
				event["delta"] = mouseDeltaX;
				newEvents.push_back(event);
			}
			
			if (yChanged)
			{
				Event event("AxisMoved");
				event["axis"] = AXIS_MOUSE_Y;
				event["delta"] = mouseDeltaY;
				newEvents.push_back(event);
			}
			
#ifndef __APPLE__
			// Windows and Linux
			
			if (autoCentreMouseX && autoCentreMouseY && xChanged && yChanged)
				curcon->setPosition(0.5f,0.5f);
			else if (autoCentreMouseX && xChanged)
				curcon->setPosition(0.5f,newMousePos.Y);
			else if (autoCentreMouseY && yChanged)
				curcon->setPosition(newMousePos.X,0.5f);
			
			// we call getRelativePosition *again* here, in case any mouse axes were centred above.
			lastMousePos = curcon->getRelativePosition();
			
#else
			// On Mac OS we do this a bit differently.
			// It seems the cursor position isn't immediately updated or something, so lastMousePos
			// does not get correctly updated with the centred position.
			// This approach would probably work on Windows and Linux too, but I haven't tested yet...
			
			lastMousePos = curcon->getRelativePosition();
			
			if (autoCentreMouseX && autoCentreMouseY && xChanged && yChanged)
			{
				curcon->setPosition(0.5f,0.5f);
				lastMousePos = core::position2df(0.5f,0.5f);
			}
			else if (autoCentreMouseX && xChanged)
			{
				curcon->setPosition(0.5f,newMousePos.Y);
				lastMousePos.X = 0.5f;
			}
			else if (autoCentreMouseY && yChanged)
			{
				curcon->setPosition(newMousePos.X,0.5f);
				lastMousePos.Y = 0.5f;
			}
#endif
		}
	}
	
	// WARNING: This is a hack, until I have implemented all Irrlicht events in
	// this engine. Obviously this event cannot be stored or transmitted over a network
	// in case the pointer gets invalidated.
	// Also the pointer will probably not be valid on 64 bit systems.
	Event event("IrrlichtEvent");
	event["SEvent"] = (u32)&irrEvent;
	newEvents.push_back(event);
	
	// Send 'em all!
	for (u32 i = 0; i < newEvents.size(); i ++)
		PostEvent(newEvents[i]);
	
	return false;
}

void Engine::SetAutoMouseCentring(bool centreX, bool centreY)
{
	autoCentreMouseX = centreX;
	autoCentreMouseY = centreY;
}

void Engine::CentreMouse()
{
	curcon->setPosition(0.5f,0.5f);
	lastMousePos = curcon->getRelativePosition();
}

core::dimension2du Engine::GetDesktopResolution()
{
	return cachedDesktopResolution;
}

IUpdater &Engine::GetLogicUpdater()
{
	return logicTask->GetUpdater();
}

IUpdater &Engine::GetRenderUpdater()
{
	return renderTask->GetUpdater();
}

void Engine::InitUpdateTiming()
{
	logicTask->InitUpdateTime();
	renderTask->InitUpdateTime();
	
	timer->setTime(0);
}



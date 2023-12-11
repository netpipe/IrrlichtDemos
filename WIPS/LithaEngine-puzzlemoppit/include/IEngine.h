
#ifndef I_ENGINE_H
#define I_ENGINE_H

#include "litha_internal.h"
#include "IPausable.h"

class IWorld;
class IRenderSystem;
class ISoundSystem;
class IUpdater;
class ButtonStates;
class IWantEvents;
class IEventQueue;
class Event;

class IEngine : public virtual IReferenceCounted, public IPausable
{
public:
	virtual ~IEngine(){}
	
	virtual IrrlichtDevice *GetIrrlichtDevice() = 0;
	
	virtual IWorld *GetWorld() = 0;
	
	virtual IRenderSystem *GetRenderSystem() = 0;
	virtual ISoundSystem *GetSoundSystem() = 0;
	
	// Use to add IUpdatables to be updated at same FPS as logic.
	virtual IUpdater &GetLogicUpdater() = 0;
	virtual IUpdater &GetRenderUpdater() = 0;
	
	virtual IUpdater *CreateUpdater() = 0;
	
	// Create an event queue.
	// This is an IUpdatable that allows more complex queueing of multiple events
	// than simply calling IEngine.QueueEvent.
	virtual IEventQueue *CreateEventQueue() = 0;
	
	// Zero any time deltas. Useful after finishing some loading or other, to prevent
	// a logic hang or rendering jump when rendering or logic tries to catch up.
	// Calls InitUpdateTime on the internal logic and render updatables.
	virtual void InitUpdateTiming() = 0;
	
	// Query current state of a button. True == pressed down.
	// Button states are cleared if window loses then regains focus.
	// Currently a button is an Irrlicht EKEY_CODE.
	virtual bool GetButtonState(s32 button) = 0;
	virtual ButtonStates GetButtonStates() = 0;
	
	// Override the internal button states.
	// Will remain overridden until a new button input event is received that changes the state.
	// ButtonUp/Down events are generated if this is a change in state.
	virtual void SetButtonState(s32 button, bool state) = 0;
	
	// Gets engine absolute time. This starts from zero when the engine starts, and is paused when the window
	// is minimised etc. (unless multiplayer, in which case the engine will not pause when minimised)
	// This time is generally only used internally, for most purposes you should use GetTime of an IUpdatable.
	virtual f32 GetEngineTime() = 0;
	
	// Returns the fraction that the current time is between the last and the next logic steps.
	// Used internally for rendering interpolated states between logic steps.
	virtual f32 GetLogicInterpolationAlpha() = 0;
	
	virtual void Run() = 0;
	
	// Shut down the engine.
	// Optionally a web page may be given which will be opened on exit.
	virtual void Exit(const core::stringc &pageURL = "") = 0;
	
	// Shut down the engine, and then re-launch the application.
	// Will attempt to use any command line arguments originally passed to CreateEngine.
	virtual void Restart() = 0;
	
	// Has Restart been called? (i.e. the app will re-launch as soon as Engine destructs)
	virtual bool WillRestart() = 0;
	
	virtual void SetWindowTitle(const core::stringw &title) = 0;
	
	// Change current working directory.
	// Each change can be undone with PopWorkingDirectory.
	// Default working directory is the .exe location.
	// Error will be printed on failure.
	virtual void PushWorkingDirectory(const io::path &path) = 0;
	virtual void PopWorkingDirectory() = 0;
	
	// Return the directory the app was started from.
	// (i.e. before the engine has changed to the binary directory, which it does on init)
	virtual io::path GetStartingDirectory() = 0;
	
	// Cet a special directory this app can write in.
	// (e.g. for storing settings and other stuff).
	// Generally this will be somewhere in the user's home directory in a sub dir appName as set
	// in EngineSettings.
	// In Windows, this might be something like C:\Documents and Settings\User\Application Data\appName
	// In Linux, might be /home/user/.appName
	// WARNING: This probably doesn't work on non ASCII systems... :S
	virtual io::path GetLocalSettingsDir() = 0;
	
	// Get the path of the file used to store app settings.
	// (this will be within the settings directory)
	virtual io::path GetSettingsPath() = 0;
	
	// Perhaps event stuff can be moved to a separate class at some point.
	
	// Register interest in a named event. Must unregister with UnregisterEventInterest or
	// UnregisterAllEventInterest when events are no longer needed.
	virtual void RegisterEventInterest(IWantEvents *receiver, const core::stringc &eventName) = 0;
	
	// Unregister interest in a named event.
	virtual void UnregisterEventInterest(IWantEvents *receiver, const core::stringc &eventName) = 0;
	
	// Register to receive all events.
	// Must unregister with UnregisterAllEventInterest.
	virtual void RegisterAllEventInterest(IWantEvents *receiver) = 0;
	
	// Unregister from receiving all events. No further events will be received.
	// This also unregisters from all specific event interest.
	virtual void UnregisterAllEventInterest(IWantEvents *receiver) = 0;
	
	// Send an event immediately, with no delay.
	// This is more or less equivalant to a function call, so care must be taken
	// to avoid recursion etc.
	virtual void PostEvent(const Event &event) = 0;
	
	// Queue an event, to be processed when "delay" seconds have passed.
	// If delay is 0, then the event is still not sent immediately, but is sent
	// when all task updates have finished.
	virtual void QueueEvent(const Event &event, f32 delay = 0.f) = 0;
	
	// Used internally by Kernel.
	virtual void ProcessEventQueue() = 0;
	
	// If enabled for an axis, that axis is centred after each move of the mouse.
	// This is useful when the mouse is being used to control a camera, otherwise
	// the pointer would reach the edge of the screen and no further movement would be possible.
	// This is internally enabled and disabled by World.Resume and World.Pause respectively.
	virtual void SetAutoMouseCentring(bool centreX, bool centreY) = 0;
	
	// Centres the mouse without generating any mouse AxisMoved events.
	virtual void CentreMouse() = 0;
	
	// This is different from the Irrlicht getDesktopResolution, in that this function
	// returns the resolution that was set *before* this app was started.
	// (Irrlicht will return the new resolution if it is running full screen).
	virtual core::dimension2du GetDesktopResolution() = 0;
	
	// Get settings used for engine creation.
	// This will be settings loaded from file, and also will contain default settings.
	virtual VariantMap GetCreationSettings() = 0;
	
	// Get the built in default engine settings, those that are used by the engine
	// if none are passed to CreateEngine.
	// This is possibly useful for the config app but has little use elsewhere.
	virtual VariantMap GetDefaultSettings() = 0;
};

// Create the engine. Should be called, once, on application start.
// If settings is NULL default settings are used.
/*
Settings may have any of the following keys defined:
(if a key is not defined, a built in default value will be used)

	appName - Name of the application. Used in various places, such as the name of the user data directory.
				Defaults to "Litha Engine".
	
	windowTitle - Window title. If not set, will default to the appName.
	
	logFile - log file destination, considered relative to the exe directory.
				If not set, will default to the appName + ".log"

	screenWidth - screen width. If not set, defaults to 800.
	
	screenHeight - screen height. If not set, defaults to 600.
	
	fullScreen - boolean, should the app run in full screen? Otherwise app is run windowed.
				Defaults to false, with app running in windowed mode.
	
	softwareMode - should software rendering be used?
				Defaults to false, OpenGL is used.
*/
IEngine *CreateEngine(int argc, const char **argv, const VariantMap *settings = NULL);

// Get a pointer to the created engine. May only be called after engine creation.
// Useful to avoid passing pointers to the engine around everywhere.
IEngine *GetEngine();

#endif

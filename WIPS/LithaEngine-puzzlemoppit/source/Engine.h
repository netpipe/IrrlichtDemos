
#include "IEngine.h"
#include <stack>
#include "Buttons.h"
#include <map>

class Kernel;
class LogicTask;
class RenderTask;
class World;

class Engine : public IEngine, public IEventReceiver
{
	IrrlichtDevice *device;
	Kernel *kernel;
	World *world;
	ISoundSystem *soundSystem;
	io::IFileSystem *filesys;
	ITimer *timer;
	gui::ICursorControl *curcon;
	
	core::dimension2du cachedDesktopResolution;
	io::path startDir;
	
	// command line args used to launch the app.
	// (may be empty, exe name is not included)
	// useful if we want to restart the app.
	std::vector<io::path> args;
	
	// Engine init settings.
	VariantMap initSettings;
	
	u32 engineStartTime;
	u32 pauseTime;
	
	// push/pop working directory history.
	std::stack<io::path> pushedWDs;
	
	LogicTask *logicTask;
	RenderTask *renderTask;
	
	ButtonStates buttonStates;
	
	// for calculating mouse delta
	core::position2df lastMousePos;
	bool autoCentreMouseX;
	bool autoCentreMouseY;
	
	// event system
	std::map<core::stringc, Set<IWantEvents *> > specificEventInterest;
	Set<IWantEvents *> allEventInterest;
	
	// event queue
	std::vector<Event> eventQueue;
	
	// A web page to open when the engine has completely shut down.
	core::stringc onExitWebPage;
	
	// Should the engine re-launch the application on exiting?
	bool restartOnExit;
	
	void ClearButtonStates();
	
protected:
	void OnPause();
	void OnResume();
	
public:
	VariantMap GetDefaultSettings();
	VariantMap GetCreationSettings();
	
	Engine(int argc, const char **argv, const VariantMap *settings);
	~Engine();
	
	IrrlichtDevice *GetIrrlichtDevice();
	
	IWorld *GetWorld();
	IRenderSystem *GetRenderSystem();
	ISoundSystem *GetSoundSystem();
	
	IUpdater &GetLogicUpdater();
	IUpdater &GetRenderUpdater();
	
	IUpdater *CreateUpdater();
	IEventQueue *CreateEventQueue();
	
	void InitUpdateTiming();
	
	bool GetButtonState(s32 button);
	ButtonStates GetButtonStates();
	void SetButtonState(s32 button, bool state);
	
	f32 GetEngineTime();
	
	f32 GetLogicInterpolationAlpha();
	
	void Run();
	void Exit(const core::stringc &pageURL);
	void Restart();
	bool WillRestart();
	
	void SetWindowTitle(const core::stringw &title) { device->setWindowCaption(title.c_str()); }
	
	void PushWorkingDirectory(const io::path &path);
	void PopWorkingDirectory();
	io::path GetStartingDirectory();
	io::path GetLocalSettingsDir();
	io::path GetSettingsPath();
	
	void RegisterEventInterest(IWantEvents *receiver, const core::stringc &eventName);
	void UnregisterEventInterest(IWantEvents *receiver, const core::stringc &eventName);
	void RegisterAllEventInterest(IWantEvents *receiver);
	void UnregisterAllEventInterest(IWantEvents *receiver);
	void PostEvent(const Event &event);
	void QueueEvent(const Event &event, f32 delay);
	void ProcessEventQueue();
	
	void SetAutoMouseCentring(bool centreX, bool centreY);
	void CentreMouse();
	core::dimension2du GetDesktopResolution();
	
	bool OnEvent(const SEvent &irrEvent);
};


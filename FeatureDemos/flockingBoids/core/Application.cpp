#include "Application.h"
#include <irrlicht.h>
#ifdef _SOUND
#include <irrKlang.h>
#endif
#include "Configuration.h"
#include "Demo.h"
#include "../states/GlobalState.h"
#include "../states/IntroState.h"
#include "../states/RunningState.h"

Application::Application() :
	configuration(new Configuration),
	device(0)
#ifdef _SOUND
	,soundEngine(0)
#endif
{

}

Application::~Application()
{
	this->configuration->writeToFile("config.xml");
	delete this->configuration;
}

bool Application::init()
{
	//read the configuration file
	this->configuration->readFromFile("config.xml");

	//init Irrlicht
	this->device = irr::createDeviceEx(this->configuration->getIrrlichtParams());
	if (this->device == 0)
		return false;

	//hide mouse cursor
	this->device->getCursorControl()->setVisible(false);

#ifdef _SOUND
	//init irrklang
	this->soundEngine = irrklang::createIrrKlangDevice();
	if (!this->soundEngine)
	{
		// error starting up the engine
		this->device->getLogger()->log(L"Disabling sound.");
		this->configuration->setSoundEnabled(false);
	}
#endif

	return true;
}

void Application::run() const
{
	//create global game state
	GlobalState global;

	// create all other game states the game can be in
	IntroState intro;
	RunningState running;

	Demo demo(
		this->configuration,
		this->device
#ifdef _SOUND
		, this->soundEngine
#endif
	);

	this->device->drop(); //demo grabs them
#ifdef _SOUND
	this->soundEngine->drop();
#endif
	//register game states
	demo.addGameState("intro", &intro);
	demo.addGameState("running", &running);

	// setup the state machine
	demo.setGlobalState(&global);

	// start the game with an intro
	demo.setState(&intro);

	//main loop
	while (demo.isRunning())
	{
		//update demo timer
		demo.tick();

		//update the state machine (aka rendering and event receiving)
		demo.onUpdate();
	}
}

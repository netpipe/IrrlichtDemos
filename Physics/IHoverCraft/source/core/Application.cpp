#include "Application.h"

#include "Game.h"
#include "../statemanager/states/GlobalState.h"
#include "../statemanager/states/IntroState.h"
#include "../statemanager/states/OptionsState.h"
#include "../statemanager/states/RunningState.h"
#include "../statemanager/states/EditPlayerState.h"

Application::Application() :
	device(0)
#ifdef _SOUND
	, soundEngine(0)
#endif
{
	
}

const bool Application::init()
{
	//read the configuration file
	this->configuration.read();

	//init Irrlicht engine
	this->device = irr::createDeviceEx(this->configuration.getIrrlichtParams());
	if (!this->device)
	{
		printf("Could not startup Irrlicht.\n");
		return false;
	}

#ifdef _SOUND
	// init irrKlang engine
	this->soundEngine = irrklang::createIrrKlangDevice();
	if (!soundEngine)
	{
		printf("Could not startup irrKlang.\n");
		this->configuration.setSoundEnabled(false);
	}
#endif

	return true;
}

const void Application::run()
{	
	//create global game state
	GlobalState global;

	// create all other game states the game can be in
	IntroState intro;
	OptionsState options;
	RunningState running;
	EditPlayerState editPlayer;

	//create game (it's the state machine)
	Game game(
		this->configuration,
		this->device
#ifdef _SOUND
		, this->soundEngine
#endif
	);

	//drop irrlicht and irrklang device since game grabbed them
	this->device->drop();
#ifdef _SOUND
	this->soundEngine->drop();
#endif

	//register game states
	game.addGameState("intro", &intro);
	game.addGameState("options", &options);
	game.addGameState("running", &running);
	game.addGameState("editPlayer", &editPlayer);

	// setup the state machine
	game.setGlobalState(&global);

	// start the game with an intro
	game.setState(&intro);
	
	//main loop
	while (game.isRunning())
	{
		//update game timer
		game.tick();

		//update the state machine (aka rendering and event receiving)
		game.onUpdate();
	}
}

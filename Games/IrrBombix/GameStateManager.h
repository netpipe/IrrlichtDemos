//GameStateManager.h
// based on Irrlicht_-_from_Noob_to_Pro:_Gamestates
#if !defined __GameStateManager_h__
#define __GameStateManager_h__

#include "irrlicht.h"

class GameState;
class ControllerManager;

#define statemanager                      GameStateManager::getInstance()
#define statedevice                       GameStateManager::getInstance().getDevice()

class GameStateManager {
private:
	irr::IrrlichtDevice* irrlichtDevice;
	irr::core::array<GameState*> gameStates;

private:
	GameStateManager(void);
	GameStateManager(const GameStateManager&);
	~GameStateManager(void);

	GameStateManager operator =(const GameStateManager&);



public:
	static GameStateManager& getInstance(void) {
		static GameStateManager instance;
		return (instance);
	}

	ControllerManager* playerController;

public:
	GameState* oldGameState;
	bool initialize(int width, int height, bool fullscreen);
	bool finalize(void);

	irr::IrrlichtDevice* getDevice(void);

	bool isRunning(void);

	void addGameState(GameState* GameState, bool active = false);
	void changeGameState(const char* name);
	void changeGameStateBack();
	GameState* getActiveGameState(void);
	GameState* getGameState(const char* name);

	void beginRender(void);
	void endRender(void);

	void render(void);

	void updateState(void);
	int getUpdatesPerSecond();
};

#endif


/*
 * GameStateManger.h
 *
 *  Created on: Apr 11, 2011
 *      based on Irrlicht_-_from_Noob_to_Pro:_Gamestates
 */

#ifndef GAMESTATEMANAGER_H_
#define GAMESTATEMANAGER_H_

//GameStateManager.h
#include "GameStateManager.h"
#include "GameState.h"
#include <iostream>

GameStateManager::GameStateManager(void) {
}

GameStateManager::GameStateManager(const GameStateManager&) {
}

GameStateManager::~GameStateManager(void) {
}

bool GameStateManager::initialize(int windowWidth, int windowHeight,
		bool fullscreen) {
	irrlichtDevice = irr::createDevice(irr::video::EDT_OPENGL, irr::core::dimension2d<irr::u32> (windowWidth,
			windowHeight), 32, fullscreen);

	if (irrlichtDevice) {
		return (true);
	}

	return (false);
}

bool GameStateManager::finalize(void) {
	irrlichtDevice->closeDevice();
	irrlichtDevice->drop();

	return (true);
}

irr::IrrlichtDevice* GameStateManager::getDevice(void) {
	return (irrlichtDevice);
}


bool GameStateManager::isRunning(void) {
	return (irrlichtDevice->run());
}

void GameStateManager::addGameState(GameState* GameState, bool active) {
	gameStates.push_back(GameState);

	if (active) {
		GameState->setActive(true);
		//statedevice->setEventReceiver(GameState);
		GameState->initialize();
	}
}

void GameStateManager::changeGameStateBack() {
	changeGameState(oldGameState->getName().c_str());
}

void GameStateManager::changeGameState(const char* name) {
	oldGameState = getActiveGameState();
	for (unsigned int i = 0; i < gameStates.size(); i++) {
		//std::cout << "GameStateManager " << name  << "vs" << gameStates[i]->getName().c_str()<<"\n";
		if (irr::core::stringc(name) == irr::core::stringc(gameStates[i]->getName())) {
			if (getActiveGameState() != NULL) {
				std::cout << "GameStateManager change from" << getActiveGameState()->getName().c_str() <<" to "<< gameStates[i]->getName().c_str()<<"\n";
				getActiveGameState()->flush();


			}
			//statedevice->setEventReceiver(gameStates[i]);
			gameStates[i]->initialize();

			return;
		}
	}
	std::cout << "GameStateManager " << name  << "state not found\n";
}

GameState* GameStateManager::getActiveGameState(void) {
	for (unsigned int i = 0; i < gameStates.size(); i++) {
		if (gameStates[i]->isActive()) {
			return (gameStates[i]);
		}
	}

	return (NULL);
}

GameState* GameStateManager::getGameState(const char* name) {
	for (unsigned int i = 0; i < gameStates.size(); i++) {
		if (irr::core::stringc(name) == gameStates[i]->getName()) {
			return (gameStates[i]);
		}
	}
	return (NULL);
}

void GameStateManager::beginRender(void) {
	irrlichtDevice->getVideoDriver()->beginScene(true, true, irr::video::SColor(255, 255,
			255, 255));
}

void GameStateManager::endRender(void) {
	irrlichtDevice->getVideoDriver()->endScene();
}

void GameStateManager::render(void) {
	getActiveGameState()->render();
}

void GameStateManager::updateState() {
	getActiveGameState()->updateThisState();
}

int GameStateManager::getUpdatesPerSecond(){
	//getActiveGameState()->updatesPerSecond();
	return 25;
}

#endif /* GAMESTATEMANGER_H_ */

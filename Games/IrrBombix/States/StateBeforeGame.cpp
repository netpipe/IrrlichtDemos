 /*
 * Copyright 2011
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "StateBeforeGame.h"
#include "../GameIrrlicht.h"
#include "../GameItemIrrlicht.h"
#include "../GameStateManager.h"
#include "../ControllerManager.h"
#include "../Mainwindow.h"
#include "../SoundManager.h"

#include <iostream>

StateBeforeGame::StateBeforeGame() {
	// TODO Auto-generated constructor stub
	m_game = new GameIrrlicht();

}

StateBeforeGame::StateBeforeGame(irr::core::stringc newName) {
	// TODO Auto-generated constructor stub
	m_game = new GameIrrlicht();
	name = newName;

}

StateBeforeGame::~StateBeforeGame() {
	// TODO Auto-generated destructor stub
}

// state stuff

void StateBeforeGame::render() {
	GameIrrlicht::m_GameItemIrrlicht->update();
}

void StateBeforeGame::updateThisState() {
	if (!countdowntimer){
		countdowntimer = Mainwindow::device->getTimer()->getTime();
	}
	//m_game->update();
	// wait 3s before gamestart
	int prestart = 500;
	if (Mainwindow::device->getTimer()->getTime() - countdowntimer > 3000+prestart) {
		std::cout << "StateBeforeGame InGame \n";
		statemanager.changeGameState("InGame");
		//statemanager.getGameState("Pause")->setActive(true);


	}

	if (Mainwindow::device->getTimer()->getTime() - countdowntimer > 0+prestart
			&& !three) {
		// play a sound
		cAudio::IListener* listener = SoundManager::manager->getListener();
		cAudio::IAudioSource* mysound = SoundManager::manager->create("bling",
				"media/Sounds/3.wav", false);

		//Set the IAudio Sound to play3d and loop
		//play3d takes 4 arguments play3d(toloop,x,y,z,strength)
		if (mysound && listener) {
			listener->setPosition(cAudio::cVector3(0, 0, 0));
			mysound->setVolume(SoundManager::fxvolume);
			mysound->setMinDistance(1.0f);
			mysound->setMaxDistance(100.0f);
			mysound->play3d(cAudio::cVector3(0, 0, 0), 2.0f, false);
		}

		//statemanager.getGameState("Pause")->setActive(true);
		three = true;

	}
	if (Mainwindow::device->getTimer()->getTime() - countdowntimer > 1000+prestart
			&& !two) {
		// play a sound
		cAudio::IListener* listener = SoundManager::manager->getListener();
		cAudio::IAudioSource* mysound = SoundManager::manager->create("bling",
				"media/Sounds/2.wav", false);

		//Set the IAudio Sound to play3d and loop
		//play3d takes 4 arguments play3d(toloop,x,y,z,strength)
		if (mysound && listener) {
			listener->setPosition(cAudio::cVector3(0, 0, 0));
			mysound->setVolume(SoundManager::fxvolume);
			mysound->setMinDistance(1.0f);
			mysound->setMaxDistance(100.0f);
			mysound->play3d(cAudio::cVector3(0, 0, 0), 2.0f, false);
		}

		//statemanager.getGameState("Pause")->setActive(true);
		two = true;

	}
	if (Mainwindow::device->getTimer()->getTime() - countdowntimer > 2000+prestart
			&& !one) {
		// play a sound
		cAudio::IListener* listener = SoundManager::manager->getListener();
		cAudio::IAudioSource* mysound = SoundManager::manager->create("bling",
				"media/Sounds/1.wav", false);

		//Set the IAudio Sound to play3d and loop
		//play3d takes 4 arguments play3d(toloop,x,y,z,strength)
		if (mysound && listener) {
			listener->setPosition(cAudio::cVector3(0, 0, 0));
			mysound->setVolume(SoundManager::fxvolume);
			mysound->setMinDistance(1.0f);
			mysound->setMaxDistance(100.0f);
			mysound->play3d(cAudio::cVector3(0, 0, 0), 2.0f, false);
		}
		one = true;

		//statemanager.getGameState("Pause")->setActive(true);


	}
	if (Mainwindow::device->getTimer()->getTime() - countdowntimer > 3000+prestart
			&& !go) {
		// play a sound
		cAudio::IListener* listener = SoundManager::manager->getListener();
		cAudio::IAudioSource* mysound = SoundManager::manager->create("bling",
				"media/Sounds/go.wav", false);

		//Set the IAudio Sound to play3d and loop
		//play3d takes 4 arguments play3d(toloop,x,y,z,strength)
		if (mysound && listener) {
			listener->setPosition(cAudio::cVector3(0, 0, 0));
			mysound->setVolume(SoundManager::fxvolume);
			mysound->setMinDistance(1.0f);
			mysound->setMaxDistance(100.0f);
			mysound->play3d(cAudio::cVector3(0, 0, 0), 2.0f, false);
		}

		//statemanager.getGameState("Pause")->setActive(true);
		go = true;

	}

	if (statemanager.playerController->keyReleased(irr::KEY_F10)) {
		std::cout << "StateBeforeGame Pause \n";
		statemanager.changeGameState("Pause");
		//statemanager.getGameState("Pause")->setActive(true);
		return;
	}

}

void StateBeforeGame::OnEnter(void) {

	Mainwindow::device->setInputReceivingSceneManager(
			GameIrrlicht::m_GameItemIrrlicht->smgr);
	countdowntimer=0;
	one = false;
	two = false;
	three = false;
	go = false;

	// turn off menu music
	SoundManager::menumusic->stop();
}

//this is called everytime when another GameState is entered
//so first leave this (e.g. destroy the map)
void StateBeforeGame::OnLeave(void) {

}

//here we get the user input
//this method is derived by IEventReceiver
bool StateBeforeGame::OnEvent(const irr::SEvent &event) {
	return true;
}
;

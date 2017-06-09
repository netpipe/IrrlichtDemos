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

#include "StateInGame.h"
#include "../GameIrrlicht.h"
#include "../GameItemIrrlicht.h"
#include "../GameStateManager.h"
#include "../ControllerManager.h"
#include "../Mainwindow.h"
#include "../GameTimerIrrlicht.h"
#include "../SoundManager.h"
#include <iostream>


StateInGame::StateInGame() {
	// TODO Auto-generated constructor stub
	m_game= new GameIrrlicht();

}

StateInGame::StateInGame(irr::core::stringc newName) {
	// TODO Auto-generated constructor stub
	m_game= new GameIrrlicht();
	name=newName;

}

StateInGame::~StateInGame() {
	// TODO Auto-generated destructor stub
}


// state stuff

void StateInGame::render(){
	GameIrrlicht::m_GameItemIrrlicht->update();
}

void StateInGame::updateThisState(){
	m_game->update();

	if (statemanager.playerController->keyReleased(irr::KEY_F10)) {
			std::cout << "StateInGame Pause \n";
			statemanager.changeGameState("Pause");
			//statemanager.getGameState("Pause")->setActive(true);
			return;
	}
}


void  StateInGame::OnEnter(void){
	Mainwindow::device->setInputReceivingSceneManager(GameIrrlicht::m_GameItemIrrlicht->smgr);
	GameIrrlicht::m_gametimerIrrlicht->updateOldTime();


	// play a sound
	cAudio::IListener* listener = SoundManager::manager->getListener();
	//Set the IAudio Sound to play3d and loop
	//play3d takes 4 arguments play3d(toloop,x,y,z,strength)
	if (!SoundManager::gamemusic->isPlaying()) {
		if (SoundManager::gamemusic && listener) {
			listener->setPosition(cAudio::cVector3(0, 0, 0));
			SoundManager::gamemusic->setVolume(SoundManager::musicvolume);
			SoundManager::gamemusic->setMinDistance(1.0f);
			SoundManager::gamemusic->setMaxDistance(100.0f);
			SoundManager::gamemusic->play3d(cAudio::cVector3(0, 0, 0), 2.0f,
					true);
		}
	}
}

  //this is called everytime when another GameState is entered
  //so first leave this (e.g. destroy the map)
void  StateInGame::OnLeave(void) {

	// play a sound
	cAudio::IListener* listener = SoundManager::manager->getListener();
	//Set the IAudio Sound to play3d and loop
	//play3d takes 4 arguments play3d(toloop,x,y,z,strength)
	SoundManager::gamemusic->stop();

}

  //here we get the user input
  //this method is derived by IEventReceiver
bool  StateInGame::OnEvent(const irr::SEvent &event) {
	return true;
};

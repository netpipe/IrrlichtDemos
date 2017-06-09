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

#include "StateAfterGame.h"
#include "../GameIrrlicht.h"
#include "../GameItemIrrlicht.h"
#include "../GameStateManager.h"
#include "../ControllerManager.h"
#include "../Mainwindow.h"
#include "../SoundManager.h"
#include <iostream>


StateAfterGame::StateAfterGame() {
	// TODO Auto-generated constructor stub
	m_game= new GameIrrlicht();

}

StateAfterGame::StateAfterGame(irr::core::stringc newName) {
	// TODO Auto-generated constructor stub
	m_game= new GameIrrlicht();
	name=newName;

}

StateAfterGame::~StateAfterGame() {
	// TODO Auto-generated destructor stub
}


// state stuff

void StateAfterGame::render(){
	GameIrrlicht::m_GameItemIrrlicht->update();
}

void StateAfterGame::updateThisState(){
	//m_game->update();
	// wait 5s after end
	if (Mainwindow::device->getTimer()->getTime()-countdowntimer > 5000){
		std::cout << "StateAfterGame Scoreboard \n";
		statemanager.changeGameState("Scoreboard");
			//statemanager.getGameState("Pause")->setActive(true);
			return;

	}

	if (statemanager.playerController->keyReleased(irr::KEY_F10)) {
			std::cout << "StateAfterGame Pause \n";
			statemanager.changeGameState("Pause");
			//statemanager.getGameState("Pause")->setActive(true);
			return;
	}
}


void  StateAfterGame::OnEnter(void){
	Mainwindow::device->setInputReceivingSceneManager(GameIrrlicht::m_GameItemIrrlicht->smgr);
	countdowntimer=Mainwindow::device->getTimer()->getTime();


	cAudio::IListener* listener = SoundManager::manager->getListener();
	cAudio::IAudioSource* mysound = SoundManager::manager->create("bling",
			"media/Sounds/WinRound.wav", false);

	//Set the IAudio Sound to play3d and play
	//play3d takes 4 arguments play3d(toloop,x,y,z,strength)
	if (mysound && listener) {
		listener->setPosition(cAudio::cVector3(GameIrrlicht::m_GameItemIrrlicht->cam->getPosition().X,GameIrrlicht::m_GameItemIrrlicht->cam->getPosition().Y,GameIrrlicht::m_GameItemIrrlicht->cam->getPosition().Z));
		mysound->setVolume(SoundManager::fxvolume);
		mysound->setMinDistance(1.0f);
		mysound->setMaxDistance(100.0f);
		mysound->play3d(cAudio::cVector3(0, 0, 0), 2.0f, false);
	}

}

  //this is called everytime when another GameState is entered
  //so first leave this (e.g. destroy the map)
void  StateAfterGame::OnLeave(void) {

}

  //here we get the user input
  //this method is derived by IEventReceiver
bool  StateAfterGame::OnEvent(const irr::SEvent &event) {
	return true;
};

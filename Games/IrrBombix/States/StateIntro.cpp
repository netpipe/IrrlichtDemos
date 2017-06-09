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

#include "StateIntro.h"
#include "../GameIrrlicht.h"
#include "../GameItemIrrlicht.h"
#include "../GameStateManager.h"
#include "../ControllerManager.h"
#include "../Mainwindow.h"

#include "../SoundManager.h"

#include <iostream>




StateIntro::StateIntro() {
	// TODO Auto-generated constructor stub
	m_game = new GameIrrlicht();

}

StateIntro::StateIntro(irr::core::stringc newName) {
	// TODO Auto-generated constructor stub
	m_game = new GameIrrlicht();
	name = newName;


}

StateIntro::~StateIntro() {
	// TODO Auto-generated destructor stub
}

// state stuff

void StateIntro::render() {

	irr::core::dimension2d<unsigned int> size =
			Mainwindow::device->getVideoDriver()->getScreenSize();
	irr::video::ITexture* drawpic =
			Mainwindow::device->getVideoDriver()->getTexture(
					"./media/Screens/xbomblogo.bmp");
	irr::core::rect<irr::s32> picrect =
			irr::core::rect<irr::s32>(0, 0, drawpic->getOriginalSize().Width,
					drawpic->getOriginalSize().Height);
	//irr::core::rect<irr::s32> nrect = irr::core::rect<irr::s32>(0, 0, size.Width,size.Height);
	irr::core::rect<irr::s32> nrect = irr::core::rect<irr::s32>(0, 0, 0, 0);

	const irr::video::SColor col = irr::video::SColor(255, 255, 255, 255);
	//Mainwindow::device->getVideoDriver()->draw2DImage(drawpic	, irr::core::rect<irr::s32>(0, 0, size.Width, size.Height), picrect, &nrect, &col , false);
	Mainwindow::device->getVideoDriver()->draw2DImage(drawpic, irr::core::rect<
			irr::s32>(0, 0, size.Width, size.Height), picrect);
	//Mainwindow::device->getVideoDriver()->draw2DImage(drawpic	, irr::core::rect<irr::s32>(0, 0, size.Width, size.Height), picrect, 0,0, true);


}

void StateIntro::updateThisState() {
	//m_game->update();
	// wait 3s before gamestart
	if (Mainwindow::device->getTimer()->getTime() - countdowntimer > 3000) {
		std::cout << "StateIntro  \n";
		statemanager.changeGameState("PlayerSelectionboard");
		//statemanager.getGameState("Pause")->setActive(true);
		return;

	}

	/*
	if (statemanager.playerController->keyReleased(irr::KEY_KEY_S)) {
		//std::cout << "StateOptionsboard Pause \n";
		GameIrrlicht::game->clear();
		GameIrrlicht::game->initGame();
		statemanager.changeGameState("InGame");
		//statemanager.getGameState("Pause")->setActive(true);
		return;
	}
	*/


}

void StateIntro::OnEnter(void) {
	//Mainwindow::device->setInputReceivingSceneManager(GameIrrlicht::m_GameItemIrrlicht->smgr);
	countdowntimer = Mainwindow::device->getTimer()->getTime();

	// play a sound
	cAudio::IListener* listener = SoundManager::manager->getListener();
	cAudio::IAudioSource* mysound = SoundManager::manager->create("bling",
			"media/Sounds/XBomberBox2.wav", false);

	//Set the IAudio Sound to play3d and loop
	//play3d takes 4 arguments play3d(toloop,x,y,z,strength)
	if (mysound && listener) {
		listener->setPosition(cAudio::cVector3(0, 0, 0));
		mysound->setVolume(SoundManager::musicvolume);
		mysound->setMinDistance(1.0f);
		mysound->setMaxDistance(100.0f);
		mysound->play3d(cAudio::cVector3(0, 0, 0), 2.0f, false);
	}
}

//this is called everytime when another GameState is entered
//so first leave this (e.g. destroy the map)
void StateIntro::OnLeave(void) {


}

//here we get the user input
//this method is derived by IEventReceiver
bool StateIntro::OnEvent(const irr::SEvent &event) {
	return true;
}
;

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

#include "StateDrawGame.h"
#include "../GameIrrlicht.h"
#include "../GameItemIrrlicht.h"
#include "../GameStateManager.h"
#include "../ControllerManager.h"
#include "../Mainwindow.h"
#include <iostream>

StateDrawGame::StateDrawGame() {
	// TODO Auto-generated constructor stub
	//m_game = new GameIrrlicht();

}

StateDrawGame::StateDrawGame(irr::core::stringc newName) {
	// TODO Auto-generated constructor stub
	// m_game = new GameIrrlicht();
	name = newName;

	// TODO Auto-generated constructor stub
	smgr = Mainwindow::device->getSceneManager()->createNewSceneManager(false);

	name = newName;

	//camera = smgr->addCameraSceneNodeFPS(0, 100.0f, .3f, 0, 0, 0, true, 3.f);
	camera = smgr->addCameraSceneNode();
	camera->setPosition(irr::core::vector3df(0, 250, 0));
	camera->setTarget(irr::core::vector3df(0, 0, 0));

	readModell(std::string("./media/Screens/Scoreboard.xml"), smgr);
	node2->setVisible(true);
	node2->setPosition(irr::core::vector3df(0, 0, 0));
	node2->setMaterialFlag(irr::video::EMF_LIGHTING, true);
	node2->setMaterialFlag(irr::video::EMF_ANISOTROPIC_FILTER, true);
	//node2->setVisible(false);


	irr::scene::ILightSceneNode* light = smgr->addLightSceneNode(camera,
			irr::core::vector3df(0, 250, 0), irr::video::SColorf(1.0f, 0.6f,
					0.7f, 1.0f), 10.0f);
	//irr::scene::ISceneNode* const* node= node2->getChildren().getLast().operator ->();
	//			node2->getChildren().size();


	int matnr = animModel->getMaterialCount() - 1;

	animModel->getMaterial(matnr).setTexture(0,	Mainwindow::device->getVideoDriver()->getTexture("./media/Screens/draw.bmp"));

	smgr->setAmbientLight(irr::video::SColor(0, 200, 200, 200));
}

StateDrawGame::~StateDrawGame() {
	// TODO Auto-generated destructor stub
}

// state stuff

void StateDrawGame::render() {
	if (Mainwindow::device->getTimer()->getTime() - countdowntimer < 3000) {
		Mainwindow::device->setInputReceivingSceneManager(GameIrrlicht::m_GameItemIrrlicht->smgr);
		GameIrrlicht::m_GameItemIrrlicht->update();
	} else {
		Mainwindow::device->setInputReceivingSceneManager(smgr);
		smgr->drawAll();
	}

}

void StateDrawGame::updateThisState() {
	//m_game->update();
	// wait 3-5s before gamestart
	if (Mainwindow::device->getTimer()->getTime() - countdowntimer > 5000) {
		std::cout << "StateDrawGame Scoreboard \n";
		GameIrrlicht::game->clear();
		GameIrrlicht::game->initGame();
		statemanager.changeGameState("BeforeGame");
		//statemanager.getGameState("Pause")->setActive(true);
		return;

	}

	if (statemanager.playerController->keyReleased(irr::KEY_F10)) {
						//std::cout << "StateOptionsboard Pause \n";
						statemanager.changeGameState("Pause");
						//statemanager.getGameState("Pause")->setActive(true);
						return;
					}

	if (statemanager.playerController->keyReleased(irr::KEY_F10)) {
		std::cout << "StateDrawGame Pause \n";
		statemanager.changeGameState("Pause");
		//statemanager.getGameState("Pause")->setActive(true);
		return;
	}
}

void StateDrawGame::OnEnter(void) {


	countdowntimer = Mainwindow::device->getTimer()->getTime();
}

//this is called everytime when another GameState is entered
//so first leave this (e.g. destroy the map)
void StateDrawGame::OnLeave(void) {

}

//here we get the user input
//this method is derived by IEventReceiver
bool StateDrawGame::OnEvent(const irr::SEvent &event) {
	return true;
}
;

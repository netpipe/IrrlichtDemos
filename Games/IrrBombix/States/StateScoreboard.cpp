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

#include "StateScoreboard.h"
#include "../GameStateManager.h"
#include "../ControllerManager.h"
#include "../GameIrrlicht.h"
#include "../GameTimerIrrlicht.h"
#include "../GameItemIrrlicht.h"
#include "../PlayerIrrlicht.h"
#include "../Mainwindow.h"
#include "StateOptionsboard.h"


#include <iostream>
#include "irrlicht.h"
#include <sstream>

StateScoreboard::StateScoreboard() {
	// TODO Auto-generated constructor stub


}

StateScoreboard::StateScoreboard(irr::core::stringc newName) {
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

	//animModel->setMaterialTexture( matnr, Mainwindow::device->getVideoDriver()->getTexture("./media/Screens/fondg2.bmp") );
	// this should work
	animModel->getMaterial(matnr).setTexture(0,	Mainwindow::device->getVideoDriver()->getTexture("./media/Screens/fondg2.bmp"));


	smgr->setAmbientLight(irr::video::SColor(0, 200, 200, 200));

	//render to texture
	bool drt = Mainwindow::device->getVideoDriver()->queryFeature(
			irr::video::EVDF_RENDER_TO_TARGET);

	//drt =0;
	fixedCam = 0;

	if (drt) {
		rt = Mainwindow::device->getVideoDriver()->addRenderTargetTexture(
				irr::core::dimension2d<irr::u32>(512, 512), "RTT1");
		animModel->getMaterial(matnr).setTexture(0,	rt);
		// add fixed camera
		fixedCam = smgr->addCameraSceneNode(0,
				irr::core::vector3df(10, 10, -80), irr::core::vector3df(-10,
						10, -100));
		fixedCam->setPosition(irr::core::vector3df(0, 200, 0));
		fixedCam->setTarget(irr::core::vector3df(0, 210, 0));

	} else {
		animModel->getMaterial(matnr).setTexture(0,		Mainwindow::device->getVideoDriver()->getTexture("./media/Screens/fondg.bmp"));

		rt = 0;
	}

}

StateScoreboard::~StateScoreboard() {
	// TODO Auto-generated destructor stub
}

// state stuff

void StateScoreboard::render() {
	//GameIrrlicht::m_GameItemIrrlicht->update();

	//render to texture

	if (rt) {
		// draw scene into render target

		// set render target texture
		Mainwindow::device->getVideoDriver()->setRenderTarget(rt, true, true,
				irr::video::SColor(0, 0, 0, 255));

		// make cube invisible and set fixed camera as active camera
		//test->setVisible(false);
		smgr->setActiveCamera(fixedCam);

		// draw whole scene into render buffer

		smgr->drawAll();
		irr::core::dimension2d<unsigned int> size =
				Mainwindow::device->getVideoDriver()->getScreenSize();

		Mainwindow::device->getVideoDriver()->draw2DImage(
				Mainwindow::device->getVideoDriver()->getTexture(
						"./media/Screens/fondg2.bmp"), irr::core::position2d<
						irr::s32>(0, 0), irr::core::rect<irr::s32>(0, 0, 512,
						512), 0, irr::video::SColor(255, 255, 255, 255), true);

		irr::gui::IGUIFont* font =
				Mainwindow::device->getGUIEnvironment()->getFont(
						"./media/fontoffi.png");

		//int hstep = 32;

		bool tworows = false;
		int wstep = 0;
		int halfplayers = GameIrrlicht::players.size() / 2
				+ GameIrrlicht::players.size() % 2;

		int hstep;
		if (GameIrrlicht::players.size() > 6) {
			tworows = true;
			hstep = (512) / ((halfplayers) + 1);

		} else {
			hstep = (512) / (GameIrrlicht::players.size() + 1);
		}

		font->draw("Score Board", irr::core::rect<irr::s32>(200, 0.5 * hstep,
				400, 0.5 * hstep + hstep), irr::video::SColor(255, 255, 255,
				255));

		for (int i = 0; i < GameIrrlicht::players.size(); i++) {

			std::stringstream ss;

			//ss << GameIrrlicht::players[i]->getPlayerName().c_str() << "\t\t"<< GameIrrlicht::players[i]->points();

			ss << GameIrrlicht::players[i]->points();

			/* font->draw(ss.str().c_str(), irr::core::rect<irr::s32>(200, 200 + i
			 * 100, 300 + i * 100, 400), irr::video::SColor(255, 255,
			 255, 255));
			 */

			if (tworows && i >= halfplayers) {
				wstep = 256;
				font->draw(GameIrrlicht::players[i]->getPlayerName().c_str(),
						irr::core::rect<irr::s32>(wstep + 32, (i + 1.0
								- halfplayers) * hstep, wstep + 232, hstep + (i
								+ 1.0 - halfplayers) * hstep),
						irr::video::SColor(255, 255, 255, 255));

				font->draw(ss.str().c_str(), irr::core::rect<irr::s32>(wstep
						+ 232, (i + 1.0 - halfplayers) * hstep, 512, hstep + (i
						+ 1.0 - halfplayers) * hstep), irr::video::SColor(255,
						255, 255, 255));

			} else {
				wstep = 0;
				font->draw(GameIrrlicht::players[i]->getPlayerName().c_str(),
						irr::core::rect<irr::s32>(wstep + 32,
								(i + 1.0) * hstep, wstep + 232, hstep + (i
										+ 1.0) * hstep), irr::video::SColor(
								255, 255, 255, 255));
				font->draw(ss.str().c_str(), irr::core::rect<irr::s32>(wstep
						+ 232, (i + 1.0) * hstep, 512, hstep + (i + 1.0)
						* hstep), irr::video::SColor(255, 255, 255, 255));
			}

			//std::cout << GameIrrlicht::players[i]->getPlayerName().c_str() << "\t\t" << GameIrrlicht::players[i]->points()<< "\n";
		}

		// draw whole scene into render buffer
		//smgr->drawAll();

		// set back old render target
		// The buffer might have been distorted, so clear it
		Mainwindow::device->getVideoDriver()->setRenderTarget(0, true, true, 0);

		// make the cube visible and set the user controlled camera as active one
		smgr->setActiveCamera(camera);
		smgr->drawAll();
	} else {
		smgr->drawAll();

		irr::gui::IGUIFont* font =
				Mainwindow::device->getGUIEnvironment()->getFont(
						"./media/fontoffi.png");

		irr::core::dimension2d<unsigned int> size =
				Mainwindow::device->getVideoDriver()->getScreenSize();

		for (int i = 0; i < GameIrrlicht::players.size(); i++) {
			std::stringstream ss;

			ss << GameIrrlicht::players[i]->getPlayerName().c_str() << "\t\t"
					<< GameIrrlicht::players[i]->points();

			font->draw(ss.str().c_str(), irr::core::rect<irr::s32>(200, 200 + i
					* 100, 300 + i * 100, 400), irr::video::SColor(255, 255,
					255, 255));

			//std::cout << GameIrrlicht::players[i]->getPlayerName().c_str() << "\t\t" << GameIrrlicht::players[i]->points()<< "\n";
		}

		std::cout << "No Render to Texture\n";
	}

	// irr::gui::IGUIFont* font = Mainwindow::device->getGUIEnvironment()->getBuiltInFont();


	//std::cout <<"\n\n";

}

void StateScoreboard::updateThisState() {
	statemanager.playerController->update();
	if (statemanager.playerController->keyReleased(irr::KEY_F10)) {
		//std::cout << "StateOptionsboard Pause \n";
		statemanager.changeGameState("Pause");
		//statemanager.getGameState("Pause")->setActive(true);
		return;
	}

	//std::cout << "StateScoreboard update \n";
	if (statemanager.playerController->keyReleased(irr::KEY_KEY_S)) {
		//std::cout << "StateScoreboard Pause \n";
		GameIrrlicht::game->clear();
		GameIrrlicht::game->initGame();
		statemanager.changeGameState("BeforeGame");
		//statemanager.getGameState("Pause")->setActive(true);
		return;
	}

	if (statemanager.playerController->keyReleased(irr::KEY_KEY_B)) {
		//std::cout << "StateScoreboard Pause \n";
		GameIrrlicht::game->clear();
		statemanager.changeGameState("PlayerSelection");
		//statemanager.getGameState("Pause")->setActive(true);
		return;
	}

	bool endseries = false;

	std::stringstream ss1;
	ss1.str(StateOptionsboard::stateoptionsboard->getValue("Nb Victory"));


	int nbvictory;

	ss1 >> nbvictory;

	for (int j = 0; j < GameIrrlicht::game->players.size(); j++) {
		if (GameIrrlicht::game->players[j]->points() == nbvictory) {
			// std::cout << "StateScoreboard update endseries " << j << " " << endseries << "\n";
			endseries = true;
		}
	}
	if (ss1.str()== "nonstop"){
		endseries = false;
	}

	for (int i = 0; i < GameIrrlicht::game->players.size(); i++) {

		if ((GameIrrlicht::game->players[i]->getbutton1()
				== PlayerIrrlicht::DOWN
				|| GameIrrlicht::game->players[i]->getbutton1()
						== PlayerIrrlicht::PRESSED)) {
			std::cout << "StateScoreboard button pressed \n";

			if (endseries) {
				for (int j = 0; j < GameIrrlicht::game->players.size(); j++) {
					//GameIrrlicht::game->players[i]->setPoints(0);

				}
				GameIrrlicht::game->clear();
				statemanager.changeGameState("PlayerSelectionboard");
				return;

			} else {
				GameIrrlicht::game->clear();
				GameIrrlicht::game->initGame();
				statemanager.changeGameState("BeforeGame");
				return;
			}

		}
	}
}

void StateScoreboard::OnEnter(void) {
	std::cout << "StateScoreboard on enter \n";
	Mainwindow::device->setInputReceivingSceneManager(smgr);

}

//this is called everytime when another GameState is entered
//so first leave this (e.g. destroy the map)
void StateScoreboard::OnLeave(void) {
}

//here we get the user input
//this method is derived by IEventReceiver
bool StateScoreboard::OnEvent(const irr::SEvent &event) {
	return true;
}
;

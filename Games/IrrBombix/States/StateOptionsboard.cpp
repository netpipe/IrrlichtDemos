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

#include "StateOptionsboard.h"
#include "../GameStateManager.h"
#include "../ControllerManager.h"
#include "../GameIrrlicht.h"
#include "../GameTimerIrrlicht.h"
#include "../GameItemIrrlicht.h"
#include "../PlayerIrrlicht.h"
#include "../Mainwindow.h"
#include <iostream>
#include "irrlicht.h"
#include <sstream>

#include "../OptionsWidget.h"

StateOptionsboard* StateOptionsboard::stateoptionsboard = 0;

StateOptionsboard::StateOptionsboard() {
	// TODO Auto-generated constructor stub


}

StateOptionsboard::StateOptionsboard(irr::core::stringc newName) {
	// TODO Auto-generated constructor stub

	StateOptionsboard::stateoptionsboard = this;
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
	smgr->setAmbientLight(irr::video::SColorf(0.2, 0.2, 0.2, 1));
	//irr::scene::ISceneNode* const* node= node2->getChildren().getLast().operator ->();
	//			node2->getChildren().size();


	int matnr = animModel->getMaterialCount() - 1;

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
		animModel->getMaterial(matnr).setTexture(0,	Mainwindow::device->getVideoDriver()->getTexture("./media/Screens/fondg.bmp"));

		rt = 0;
	}

	// menu logik
	OptionsWidget* tmp;
	cam = new OptionsWidget("Camera");
	tmp=cam;
	tmp->addoptions("dynamic");
	tmp->addoptions("static");
	tmp->addoptions("overhead");
	tmp->addoptions("fly");
	tmp->addoptions("beauty");
	entries.addoptionsWidget(tmp);

	tmp = new OptionsWidget("Nb Victory");
	tmp->addoptions("1");
	tmp->addoptions("3");
	tmp->addoptions("5");
	tmp->addoptions("7");
	tmp->addoptions("10");
	tmp->addoptions("nonstop");
	entries.addoptionsWidget(tmp);


	tmp = new OptionsWidget("Rock Density");
	tmp->addoptions("1");
	tmp->addoptions("2");
	tmp->addoptions("3");
	tmp->addoptions("4");
	tmp->addoptions("5");
	tmp->addoptions("6");
	tmp->addoptions("7");
	tmp->addoptions("8");
	tmp->addoptions("9");
	tmp->selectprev();
	tmp->selectprev();
	tmp->selectprev();
	tmp->selectprev();

	entries.addoptionsWidget(tmp);

	tmp = new OptionsWidget("Bonus Density");
	tmp->addoptions("0");
	tmp->addoptions("3");
	tmp->addoptions("5");
	tmp->addoptions("8");

	tmp->selectprev();
	tmp->selectprev();
	entries.addoptionsWidget(tmp);

	tmp = new OptionsWidget("Bad Bonus");
	tmp->addoptions("on");
	tmp->addoptions("off");
	entries.addoptionsWidget(tmp);


	tmp = new OptionsWidget("Party Time");
	tmp->addoptions("60");
	tmp->addoptions("90");
	tmp->addoptions("120");
	tmp->addoptions("240");
	entries.addoptionsWidget(tmp);
	tmp->selectprev();

	tmp = new OptionsWidget("Ufo");
	tmp->addoptions("on");
	tmp->addoptions("off");
	entries.addoptionsWidget(tmp);

	tmp = new OptionsWidget("Next");
	tmp->addoptions("");
	entries.addoptionsWidget(tmp);

	entries.selectprev();

}

std::string StateOptionsboard::getValue(std::string aname) {
	return entries.getOptionsWidget(aname)->getValue();
}

StateOptionsboard::~StateOptionsboard() {
	// TODO Auto-generated destructor stub
}

// state stuff

void StateOptionsboard::render() {
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
		int hstep = (512) / (12);
		int line = 0;

		font->draw("Options", irr::core::rect<irr::s32>(200, (line + 0.5)
				* hstep, 400, (line + 0.5) * hstep + hstep),
				irr::video::SColor(255, 255, 255, 255));
		line = line + 1;

		for (int j = 0; j < entries.size(); j++) {
			line = j + 1;
			std::string sel;
			if (entries.getSelectedPos() == j) {
				sel = "=>";
				font->draw(sel.c_str(), irr::core::rect<irr::s32>(50, (line
						+ 0.5) * hstep, 100, (line + 0.5) * hstep + hstep),
						irr::video::SColor(255, 255, 255, 255));

			}
			font->draw(entries.getOptionsWidget(j)->getName().c_str(),
					irr::core::rect<irr::s32>(100, (line + 0.5) * hstep, 350,
							(line + 0.5) * hstep + hstep), irr::video::SColor(
							255, 255, 255, 255));
			font->draw(entries.getOptionsWidget(j)->getValue().c_str(),
					irr::core::rect<irr::s32>(350, (line + 0.5) * hstep, 500,
							(line + 0.5) * hstep + hstep), irr::video::SColor(
							255, 255, 255, 255));

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

void StateOptionsboard::updateThisState() {
	statemanager.playerController->update();
	//std::cout << "StateOptionsboard lastInterfaceaction "<<Mainwindow::device->getTimer()->getTime() - lastInterfaceAction<<"\n";

	// more actions arent human
	if (Mainwindow::device->getTimer()->getTime() - lastInterfaceAction > 150) {

		//std::cout << "StateOptionsboard update \n";
		if (statemanager.playerController->keyReleased(irr::KEY_KEY_S)) {
			//std::cout << "StateOptionsboard Pause \n";
			GameIrrlicht::game->clear();
			GameIrrlicht::game->initGame();
			statemanager.changeGameState("InGame");
			//statemanager.getGameState("Pause")->setActive(true);
			return;
		}

		if (statemanager.playerController->keyReleased(irr::KEY_KEY_B)) {
			//std::cout << "StateOptionsboard Pause \n";
			statemanager.changeGameState("PlayerSelection");
			//statemanager.getGameState("Pause")->setActive(true);
			return;
		}
		if (statemanager.playerController->keyReleased(irr::KEY_F10)) {
					//std::cout << "StateOptionsboard Pause \n";
					statemanager.changeGameState("Pause");
					//statemanager.getGameState("Pause")->setActive(true);
					return;
				}
		if (statemanager.playerController->keyReleased(irr::KEY_KEY_Y)) {
			//std::cout << "StateOptionsboard Pause \n";
			entries.selectnext();
			lastInterfaceAction = Mainwindow::device->getTimer()->getTime();
			//statemanager.getGameState("Pause")->setActive(true);
			return;
		}
		if (statemanager.playerController->keyReleased(irr::KEY_KEY_X)) {
			//std::cout << "StateOptionsboard Pause \n";
			entries.getSelectedOptionsWidget()->selectprev();

			lastInterfaceAction = Mainwindow::device->getTimer()->getTime();
			//statemanager.getGameState("Pause")->setActive(true);
			return;
		}

		for (int i = 0; i < GameIrrlicht::game->players.size(); i++) {
			if (GameIrrlicht::game->players[i]->getAskedYSpeed() < 0) {
				//std::cout << "StateOptionsboard Pause \n";
				entries.selectnext();
				lastInterfaceAction = Mainwindow::device->getTimer()->getTime();
				//statemanager.getGameState("Pause")->setActive(true);
				return;
			}
			if (GameIrrlicht::game->players[i]->getAskedYSpeed() > 0) {
				//std::cout << "StateOptionsboard Pause \n";
				entries.selectprev();

				lastInterfaceAction = Mainwindow::device->getTimer()->getTime();
				//statemanager.getGameState("Pause")->setActive(true);
				return;
			}

			if (GameIrrlicht::game->players[i]->getAskedXSpeed() > 0) {
				//std::cout << "StateOptionsboard Pause \n";
				entries.getSelectedOptionsWidget()->selectnext();
				lastInterfaceAction = Mainwindow::device->getTimer()->getTime();
				//statemanager.getGameState("Pause")->setActive(true);
				return;
			}
			if (GameIrrlicht::game->players[i]->getAskedXSpeed() < 0) {
				//std::cout << "StateOptionsboard Pause \n";
				entries.getSelectedOptionsWidget()->selectprev();
				lastInterfaceAction = Mainwindow::device->getTimer()->getTime();
				//statemanager.getGameState("Pause")->setActive(true);
				return;
			}

			if ((GameIrrlicht::game->players[i]->getbutton1()
					== PlayerIrrlicht::DOWN
					|| GameIrrlicht::game->players[i]->getbutton1()
							== PlayerIrrlicht::PRESSED)
					&& entries.getSelectedOptionsWidget()->getName() == "Next") {
				//std::cout << "StateOptionsboard Pause \n";

				statemanager.changeGameState("ArenaSelectboard");
				//statemanager.getGameState("Pause")->setActive(true);
				return;
			}
			if ((GameIrrlicht::game->players[i]->getbutton2()
					== PlayerIrrlicht::DOWN
					|| GameIrrlicht::game->players[i]->getbutton2()
							== PlayerIrrlicht::PRESSED)) {
				std::cout << "StateOptionsboard back \n";

				statemanager.changeGameState("PlayerSelectionboard");
				//statemanager.getGameState("Pause")->setActive(true);
				return;
			}
		}
	}

}

void StateOptionsboard::OnEnter(void) {
	std::cout << "StateOptionsboard on enter \n";
	lastInterfaceAction = Mainwindow::device->getTimer()->getTime();
	Mainwindow::device->setInputReceivingSceneManager(smgr);

}

//this is called everytime when another GameState is entered
//so first leave this (e.g. destroy the map)
void StateOptionsboard::OnLeave(void) {
}

//here we get the user input
//this method is derived by IEventReceiver
bool StateOptionsboard::OnEvent(const irr::SEvent &event) {
	return true;
}
;

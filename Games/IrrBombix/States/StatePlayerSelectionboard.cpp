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

#include "StatePlayerSelectionboard.h"
#include "../GameStateManager.h"
#include "../ControllerManager.h"
#include "../GameIrrlicht.h"
#include "../GameTimerIrrlicht.h"
#include "../GameItemIrrlicht.h"
#include "../PlayerIrrlicht.h"
#include "../Mainwindow.h"
#include "../PlayerItemIrrlicht.h"
#include "../IrrlichtEventReceiver.h"
#include "../SoundManager.h"


#include <iostream>
#include "irrlicht.h"
#include <sstream>

#include "../OptionsWidget.h"
#include "../OptionsWidgetObject.cpp"

StatePlayerSelectionboard
		* StatePlayerSelectionboard::stateplayerselectionboard = 0;

irr::core::array<PlayerIrrlicht*> StatePlayerSelectionboard::players = 0;


StatePlayerSelectionboard::StatePlayerSelectionboard() {
	// TODO Auto-generated constructor stub


}

StatePlayerSelectionboard::StatePlayerSelectionboard(irr::core::stringc newName) {
	// TODO Auto-generated constructor stub

	StatePlayerSelectionboard::stateplayerselectionboard = this;
	smgr = Mainwindow::device->getSceneManager()->createNewSceneManager(false);

	name = newName;

	//camera = smgr->addCameraSceneNodeFPS(0, 100.0f, .3f, 0, 0, 0, true, 3.f);
	//camera = smgr->addCameraSceneNodeFPS();
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


	//	// random character modell
	//	int plskin = 1 + (rand() % 12);
	//
	//	ss.str("");
	//	ss << plskin;
	//	if (plskin > 9) {
	//		characterfile = new std::string("./media/Characters/0" + ss.str()
	//				+ "2.xml");
	//	} else {
	//
	//		characterfile = new std::string("./media/Characters/00" + ss.str()
	//				+ "2.xml");
	//	}
	//	PlayerItemIrrlicht* tmp =
	//			dynamic_cast<PlayerItemIrrlicht*> (player->getView());
	//	tmp->setCharacterfile(characterfile);


}

std::string StatePlayerSelectionboard::getValue(std::string aname) {
	return entries.getOptionsWidget(aname)->getValue();
}

StatePlayerSelectionboard::~StatePlayerSelectionboard() {
	// TODO Auto-generated destructor stub
}

// state stuff

void StatePlayerSelectionboard::render() {
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
		int totallines;
		int wstep = 0;
		int halfplayers = StatePlayerSelectionboard::players.size() / 2
				+ StatePlayerSelectionboard::players.size() % 2;
		int tmphalfplayers = 0;
		int hstep;

		if (StatePlayerSelectionboard::players.size() > 6 || true) {
			tworows = true;
			totallines = ((halfplayers) + 2);
		} else {
			totallines = (StatePlayerSelectionboard::players.size() + 2);
		}
		hstep = (512) / totallines;

		//std::cout << "StatePlayerSelection totallines " << totallines << "\n";

		float line = 0;

		irr::core::dimension2d<irr::u32> cSize = font->getDimension(L"S");
		float fdown = 0;
		if (hstep > cSize.Height) {
			//fdown = (hstep - cSize.Height) * 0.5;
		}

		line = line - 1;
		for (int i = 0; i < entries.size(); i++) {
			line = line + 1;
			OptionsWidgetObject<std::string>
					* tmpowo =
							(dynamic_cast<OptionsWidgetObject<std::string>*> (entries.getOptionsWidget(
									i)));
			if (tworows) {
				if (i >= halfplayers) {
					wstep = 256;
					tmphalfplayers = halfplayers;
				} else {
					wstep = 0;
					tmphalfplayers = 0;
				}
			} else {
				wstep = 100;
			}

			if (entries.getOptionsWidget(i)->getName() == "Next") {
				wstep = 200;
				line = totallines - 2;
				tmphalfplayers = 0;
			}

			std::string sel;
			if (entries.getSelectedPos() == i) {
				sel = "=>";
				font->draw(sel.c_str(), irr::core::rect<irr::s32>(wstep - 25,
						(line + 1.0 - tmphalfplayers) * hstep + fdown, wstep
								+ 25, hstep + (line + 1.0 - tmphalfplayers)
								* hstep + fdown), irr::video::SColor(255, 255,
						255, 255), false, true, 0);
			}

			font->draw(tmpowo->getName().c_str(), irr::core::rect<irr::s32>(
					wstep + 20, (line + 1.0 - tmphalfplayers) * hstep + fdown,
					wstep + 190, hstep + (line + 1.0 - tmphalfplayers) * hstep
							+ fdown), irr::video::SColor(255, 255, 255, 255),
					false, true, 0);

			if (tmpowo->getIcon()->compare("")) {
				//std::cout << "StatePlayerSelection render loading "<< tmpowo->getIcon()->c_str()<< "\n";
				irr::video::ITexture* drawpic =
						Mainwindow::device->getVideoDriver()->getTexture(
								tmpowo->getIcon()->c_str());
				irr::core::rect<irr::s32> picrect = irr::core::rect<irr::s32>(
						0, 0, drawpic->getOriginalSize().Width,
						drawpic->getOriginalSize().Height);
				//irr::core::rect<irr::s32> nrect = irr::core::rect<irr::s32>(0, 0, size.Width,size.Height);

				//irr::core::rect<irr::s32> nrect = irr::core::rect<irr::s32>(350,(line + 0.5) * hstep, 350 + hstep, (line + 1.5) * hstep);
				irr::core::rect<irr::s32> nrect = irr::core::rect<irr::s32>(
						wstep + 220 - hstep / 2, (line + 1 - tmphalfplayers)
								* hstep, wstep + 220 + hstep / 2, hstep + (line
								+ 1 - tmphalfplayers) * hstep);

				if (hstep > 60) {
					int hhstep = (hstep - 60) / 2;
					nrect = irr::core::rect<irr::s32>(wstep + 220 - 30, (line
							+ 1 - tmphalfplayers) * hstep + hhstep,
							wstep + 250, hstep + (line + 1 - tmphalfplayers)
									* hstep - hhstep);
				}

				const irr::video::SColor col = irr::video::SColor(255, 255,
						255, 255);

				//irr::core::rect<irr::s32>(0, 0, drawpic->getSize().Width,	drawpic->getSize().Height)
				//Mainwindow::device->getVideoDriver()->draw2DImage(drawpic	, irr::core::rect<irr::s32>(0, 0, size.Width, size.Height), picrect, &nrect, &col , false);
				Mainwindow::device->getVideoDriver()->draw2DImage(drawpic,
						nrect, picrect);

			} else {
				font->draw(tmpowo->getValue().c_str(),
						irr::core::rect<irr::s32>(wstep + 190, (line + 1.0
								- tmphalfplayers) * hstep + fdown, wstep + 250,
								hstep + (line + 1.0 - tmphalfplayers) * hstep
										+ fdown), irr::video::SColor(255, 255,
								255, 255), true, true, 0);

			}

		}
		line = 0;
		font->draw("Select Players", irr::core::rect<irr::s32>(150, (line)
				* hstep + fdown, 400, (line) * hstep + hstep + fdown),
				irr::video::SColor(255, 255, 255, 255),false,true,0);

		// draw whole scene into render buffer
		//smgr->drawAll();

		// set back old render target
		// The buffer might have been distorted, so clear it
		Mainwindow::device->getVideoDriver()->setRenderTarget(0, true, true, 0);
		//Mainwindow::device->getVideoDriver()->setRenderTarget(0, false, false, 0);

		// make the cube visible and set the user controlled camera as active one
		smgr->setActiveCamera(camera);
		smgr->drawAll();
	} else {
		// TODO nice Fallback
		smgr->drawAll();

		irr::gui::IGUIFont* font =
				Mainwindow::device->getGUIEnvironment()->getFont(
						"./media/fontoffi.png");

		irr::core::dimension2d<unsigned int> size =
				Mainwindow::device->getVideoDriver()->getScreenSize();

		for (int i = 0; i < StatePlayerSelectionboard::players.size(); i++) {
			std::stringstream ss;

			ss
					<< StatePlayerSelectionboard::players[i]->getPlayerName().c_str()
					<< "\t\t"
					<< StatePlayerSelectionboard::players[i]->points();

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

void StatePlayerSelectionboard::updateThisState() {
	statemanager.playerController->update();
	//std::cout << "StatePlayerSelectionboard lastInterfaceaction "<<Mainwindow::device->getTimer()->getTime() - lastInterfaceAction<<"\n";

	if (statemanager.playerController->newcontroller){
		StatePlayerSelectionboard::players.clear();
		statemanager.playerController->newcontroller=false;
		statemanager.playerController->init();
		OnEnter();
	}


	// more actions arent human
	if (Mainwindow::device->getTimer()->getTime() - lastInterfaceAction > 150) {
		if (statemanager.playerController->keyReleased(irr::KEY_F10)) {
			//std::cout << "StateOptionsboard Pause \n";
			statemanager.changeGameState("Pause");
			//statemanager.getGameState("Pause")->setActive(true);
			return;
		}

		//std::cout << "StatePlayerSelectionboard update \n";
		if (statemanager.playerController->keyReleased(irr::KEY_KEY_G)) {
			//std::cout << "StatePlayerSelectionboard Pause \n";
			GameIrrlicht::game->clear();
			GameIrrlicht::game->initGame();
			statemanager.changeGameState("InGame");
			//statemanager.getGameState("Pause")->setActive(true);
			return;
		}

		if (statemanager.playerController->keyReleased(irr::KEY_KEY_B)) {
			//std::cout << "StatePlayerSelectionboard Pause \n";
			statemanager.changeGameState("PlayerSelection");
			//statemanager.getGameState("Pause")->setActive(true);
			return;
		}
		StatePlayerSelectionboard::players.size();
		GameIrrlicht::players.clear();
		for (int i = 0; i < StatePlayerSelectionboard::players.size(); i++) {
			if (StatePlayerSelectionboard::players[i]->getAskedYSpeed() < 0) {
				//std::cout << "StatePlayerSelectionboard Pause \n";
				//entries.selectnext();
				lastInterfaceAction = Mainwindow::device->getTimer()->getTime();

			}
			if (StatePlayerSelectionboard::players[i]->getAskedYSpeed() > 0) {
				//std::cout << "StatePlayerSelectionboard Pause \n";
				//entries.selectprev();

				lastInterfaceAction = Mainwindow::device->getTimer()->getTime();

			}

			if (StatePlayerSelectionboard::players[i]->getAskedXSpeed() > 0) {
				//std::cout << "StatePlayerSelectionboard " << i	<< "selctnext \n";
				//entries.getSelectedOptionsWidget()->selectnext();
				entries.getOptionsWidget(i)->selectnext();
				lastInterfaceAction = Mainwindow::device->getTimer()->getTime();

			}
			if (StatePlayerSelectionboard::players[i]->getAskedXSpeed() < 0) {
				//std::cout << "StatePlayerSelectionboard Pause \n";
				//entries.getSelectedOptionsWidget()->selectprev();
				entries.getOptionsWidget(i)->selectprev();
				lastInterfaceAction = Mainwindow::device->getTimer()->getTime();

			}

			if (entries.getOptionsWidget(i)->getValue() != "None") {
				GameIrrlicht::players.push_back(
						StatePlayerSelectionboard::players[i]);
			}
		}



		for (int i = 0; i < GameIrrlicht::players.size(); i++) {

			if ((GameIrrlicht::players[i]->getbutton1() == PlayerIrrlicht::DOWN
					|| GameIrrlicht::players[i]->getbutton1()
							== PlayerIrrlicht::PRESSED)
					&& entries.getSelectedOptionsWidget()->getName() == "Next") {
				//std::cout << "StatePlayerSelectionboard Pause \n";


				for (int j = 0; j < StatePlayerSelectionboard::players.size(); j++) {
					if (entries.getOptionsWidget(i)->getValue() != "None") {

					// set selected character modell

					//OptionsWidget* tmp = entries.getOptionsWidget(j);

					OptionsWidgetObject<std::string>
							* tmpw =
									(dynamic_cast<OptionsWidgetObject<
											std::string>*> (entries.getOptionsWidget(
											j)));
					std::string * characterfile = new std::string(tmpw->getObject());
					PlayerItemIrrlicht
							* tmp =
									dynamic_cast<PlayerItemIrrlicht*> (StatePlayerSelectionboard::players[j]->getView());
					// std::cout << "StatePlayerSelectionboard player[]" << j << " "<< characterfile << " \n";
					tmp->setCharacterfile(characterfile);


					// reset playersstats
					StatePlayerSelectionboard::players[j]->setpoints(0);
					}

				}

				statemanager.changeGameState("Optionsboard");
				return;
			}
		}
	}
}

void StatePlayerSelectionboard::OnEnter(void) {
	//std::cout << "StatePlayerSelectionboard on enter \n";
	lastInterfaceAction = Mainwindow::device->getTimer()->getTime();
	Mainwindow::device->setInputReceivingSceneManager(smgr);

	// create a player for each controller

	IrrlichtEventReceiver
			* tmp2 =
					static_cast<IrrlichtEventReceiver*> (Mainwindow::device->getEventReceiver());
	ControllerManager* playerController = tmp2->getPlayerController();

	// create Players for every available controller
	for (int j = 0; true; j++) {
		std::string* characterfile;
		std::stringstream ss;
		ss << j;
		characterfile = new std::string("Player " + ss.str());
		PlayerIrrlicht* player = new PlayerIrrlicht(-1, -1,
				characterfile->c_str(), 0);

		if (playerController->assignPlayer(player)) {

			StatePlayerSelectionboard::players.push_back(player);

			// random character modell gets overwritten in StatePlayerSelection
			int plskin = 1 + (rand() % 12);
			//std::stringstream ss;
			ss.str("");
			ss << plskin;
			if (plskin > 9) {
				characterfile = new std::string("./media/Characters/0"
						+ ss.str() + "2.xml");
			} else {

				characterfile = new std::string("./media/Characters/00"
						+ ss.str() + "2.xml");
			}
			PlayerItemIrrlicht* tmp =
					dynamic_cast<PlayerItemIrrlicht*> (player->getView());
			//tmp->setCharacterfile(characterfile);
		} else {
			//std::cout << "StatePlayerSelction created " << j - 1 << " players\n";
			break;
		}
	}

	//build the menu

	entries.clear();
	std::stringstream ss;

	PlayerItemIrrlicht* pltmp = new PlayerItemIrrlicht(0);

	for (int i = 0; i < StatePlayerSelectionboard::players.size(); i++) {
		ss.str("");
		ss << "Player " << i;
		OptionsWidgetObject<std::string>* tmp;
		tmp = new OptionsWidgetObject<std::string> (ss.str().c_str());

		tmp->addoptions("None", "", new std::string(""));
		for (int j = 1; true; j++) {
		//for (int j = 1; j < 13; j++) {
			std::stringstream ss2;
			ss2.str("");
			if (j < 10) {
				ss2 << "0";
			}
			ss2 << j;
			std::string characterfile = "./media/Characters/0" + ss2.str()
					+ "2.xml";
			if (pltmp->setCharacterfile(new std::string(characterfile))){
				tmp->addoptions(ss2.str(), characterfile, new std::string(pltmp->iconname));
			}else{
				//std::cout << "StatePlayerSelction added " << j << "\n";
				break;
			}
		}

		entries.addoptionsWidget(tmp);
		for (int j = 0; j < i + 1; j++) {
			tmp->selectnext();
		}
	}
	OptionsWidgetObject<std::string>* tmp;
	tmp = new OptionsWidgetObject<std::string> ("Next");
	tmp->addoptions("", "", new std::string(""));
	entries.addoptionsWidget(tmp);

	entries.selectprev();

	// play a sound
	cAudio::IListener* listener = SoundManager::manager->getListener();
	//Set the IAudio Sound to play3d and loop
	//play3d takes 4 arguments play3d(toloop,x,y,z,strength)
	if (!SoundManager::menumusic->isPlaying()) {
		if (SoundManager::menumusic && listener) {
			listener->setPosition(cAudio::cVector3(0, 0, 0));
			SoundManager::menumusic->setVolume(SoundManager::musicvolume);
			SoundManager::menumusic->setMinDistance(1.0f);
			SoundManager::menumusic->setMaxDistance(100.0f);
			SoundManager::menumusic->play3d(cAudio::cVector3(0, 0, 0), 2.0f,
					true);
		}
	}

	std::cout << "StatePlayerSelectionboard leave OnEnter \n";
}

//this is called everytime when another GameState is entered
//so first leave this (e.g. destroy the map)
void StatePlayerSelectionboard::OnLeave(void) {
}

//here we get the user input
//this method is derived by IEventReceiver
bool StatePlayerSelectionboard::OnEvent(const irr::SEvent &event) {
	return true;
}
;

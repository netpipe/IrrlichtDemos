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

#include "StatePause.h"
#include "../GameStateManager.h"
#include "../ControllerManager.h"
#include "../GameIrrlicht.h"
#include "../GameTimerIrrlicht.h"
#include "../GameItemIrrlicht.h"
#include "../OptionsWidget.h"
#include "../Mainwindow.h"
#include "StateOptionsboard.h"
#include "../PlayerIrrlicht.h"
#include "../SoundManager.h"

#include <iostream>

StatePause::StatePause() {
	// TODO Auto-generated constructor stub
	init();

}

StatePause::StatePause(irr::core::stringc newName) {
	// TODO Auto-generated constructor stub
	init();
	name = newName;

}

void StatePause::init() {
	smgr = Mainwindow::device->getSceneManager()->createNewSceneManager(false);

	camera = smgr->addCameraSceneNode();
	camera->setPosition(irr::core::vector3df(0, 250, 0));
	camera->setTarget(irr::core::vector3df(0, 0, 0));

	OptionsWidget* tmp;
	tmp = new OptionsWidget("Continue");
	tmp->addoptions("");
	entries.addoptionsWidget(tmp);

	entries.addoptionsWidget(StateOptionsboard::stateoptionsboard->cam);

	tmp = new OptionsWidget("Music Volume");
	tmp->addoptions("0");
	tmp->addoptions("1");
	tmp->addoptions("2");
	tmp->addoptions("3");
	tmp->addoptions("4");
	tmp->addoptions("5");
	tmp->addoptions("6");
	tmp->addoptions("7");
	tmp->addoptions("8");
	tmp->addoptions("9");
	tmp->addoptions("10");
	for (int i = 0; i < SoundManager::soundmanager->musicvolume * 10; i++) {
		tmp->selectnext();
	}

	entries.addoptionsWidget(tmp);

	tmp = new OptionsWidget("Effect Volume");
	tmp->addoptions("0");
	tmp->addoptions("1");
	tmp->addoptions("2");
	tmp->addoptions("3");
	tmp->addoptions("4");
	tmp->addoptions("5");
	tmp->addoptions("6");
	tmp->addoptions("7");
	tmp->addoptions("8");
	tmp->addoptions("9");
	tmp->addoptions("10");
	for (int i = 0; i < SoundManager::soundmanager->fxvolume * 10; i++) {
		tmp->selectnext();
	}

	entries.addoptionsWidget(tmp);

	tmp = new OptionsWidget("New Game");
	tmp->addoptions("");
	entries.addoptionsWidget(tmp);

	tmp = new OptionsWidget("Quit Game");
	tmp->addoptions("");
	entries.addoptionsWidget(tmp);
}

StatePause::~StatePause() {
	// TODO Auto-generated destructor stub
}

// state stuff

void StatePause::render() {
	//GameIrrlicht::m_GameItemIrrlicht->update();

	statemanager.oldGameState->render();

	// draw nothing (this seems to be somehow necessary)
	smgr->setActiveCamera(camera);
	smgr->drawAll();

	irr::gui::IGUIFont* font =
			Mainwindow::device->getGUIEnvironment()->getFont(
					"./media/fontoffi.png");
	//irr::gui::IGUIFont* font = statemanager.getDevice()->getGUIEnvironment()->getFont(	"./media/fontoffi.png");

	//int hstep = 32;

	bool tworows = false;
	int wstep = 0;
	int hstep = (512) / (12);
	int line = 0;

	irr::core::dimension2d<unsigned int> size =
			Mainwindow::device->getVideoDriver()->getScreenSize();

	//irr::core::dimension2d<unsigned int> size =	irr::core::dimension2d<unsigned int>(100,100);

	int startline = size.Height / hstep / 3;

	Mainwindow::device->getVideoDriver()->draw2DRectangle(irr::video::SColor(
			100, 100, 100, 255), irr::core::rect<irr::s32>(
			size.Width / 2 - 250, (startline) * hstep, size.Width / 2 + 350,
			(startline + 1 + entries.size()) * hstep + hstep));

	font->draw("Menu", irr::core::rect<irr::s32>(size.Width / 2 - 0, (startline
			+ 0.5) * hstep, size.Width / 2 + 100, (startline + 0.5) * hstep
			+ hstep), irr::video::SColor(255, 255, 255, 255));
	line = line + 1;
	//std::cout << "StatePause entires " << entries.size() << "\n";
	for (int j = 0; j < entries.size(); j++) {
		line = j + 1 + startline;
		std::string sel;
		if (entries.getSelectedPos() == j) {
			sel = "=>";
			font->draw(sel.c_str(), irr::core::rect<irr::s32>(size.Width / 2
					- 200, (line + 0.5) * hstep, size.Width / 2 + 200, (line
					+ 0.5) * hstep + hstep), irr::video::SColor(255, 255, 255,
					255));

		}
		font->draw(entries.getOptionsWidget(j)->getName().c_str(),
				irr::core::rect<irr::s32>(size.Width / 2 - 150, (line + 0.5)
						* hstep, size.Width / 2 + 50, (line + 0.5) * hstep
						+ hstep), irr::video::SColor(255, 255, 255, 255));
		font->draw(entries.getOptionsWidget(j)->getValue().c_str(),
				irr::core::rect<irr::s32>(size.Width / 2 + 100, (line + 0.5)
						* hstep, size.Width / 2 + 500, (line + 0.5) * hstep
						+ hstep), irr::video::SColor(255, 255, 255, 255));

	}

}

void StatePause::updateThisState() {
	statemanager.playerController->update();

	bool save = false;
	//std::cout << "StatePause update \n";
	if (statemanager.playerController->keyReleased(irr::KEY_F10)) {
		//std::cout << "StatePause Pause \n";
		statemanager.changeGameStateBack();
		//statemanager.changeGameState("InGame");
		//statemanager.getGameState("Pause")->setActive(true);
		return;
	}
	if (Mainwindow::device->getTimer()->getTime() - lastInterfaceAction > 150) {
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
				save = true;
				//statemanager.getGameState("Pause")->setActive(true);
				return;
			}
			if (GameIrrlicht::game->players[i]->getAskedXSpeed() < 0) {
				//std::cout << "StateOptionsboard Pause \n";
				entries.getSelectedOptionsWidget()->selectprev();
				lastInterfaceAction = Mainwindow::device->getTimer()->getTime();
				//statemanager.getGameState("Pause")->setActive(true);
				save = true;
				return;
			}

			if ((GameIrrlicht::game->players[i]->getbutton1()
					== PlayerIrrlicht::DOWN
					|| GameIrrlicht::game->players[i]->getbutton1()
							== PlayerIrrlicht::PRESSED)
					&& entries.getSelectedOptionsWidget()->getName()
							== "Continue") {
				statemanager.changeGameStateBack();
			}

			if ((GameIrrlicht::game->players[i]->getbutton1()
					== PlayerIrrlicht::DOWN
					|| GameIrrlicht::game->players[i]->getbutton1()
							== PlayerIrrlicht::PRESSED)
					&& entries.getSelectedOptionsWidget()->getName()
							== "Quit Game") {

				Mainwindow::device->closeDevice();
			}
			if ((GameIrrlicht::game->players[i]->getbutton1()
					== PlayerIrrlicht::DOWN
					|| GameIrrlicht::game->players[i]->getbutton1()
							== PlayerIrrlicht::PRESSED)
					&& entries.getSelectedOptionsWidget()->getName()
							== "New Game") {
				GameIrrlicht::game->clear();
				statemanager.changeGameState("PlayerSelectionboard");

			}
		}
	}
	// set volumes
	std::stringstream ss1;
	ss1.str(entries.getOptionsWidget("Effect Volume")->getValue());
	ss1 >> SoundManager::fxvolume;
	SoundManager::fxvolume = SoundManager::fxvolume * 0.1;
	std::stringstream ss2;
	ss2.str(entries.getOptionsWidget("Music Volume")->getValue());
	ss2 >> SoundManager::musicvolume;
	SoundManager::musicvolume = SoundManager::musicvolume * 0.1;

	SoundManager::menumusic->setVolume(SoundManager::musicvolume);
	SoundManager::gamemusic->setVolume(SoundManager::musicvolume);

	SoundManager::soundmanager->save();
	//std::cout << "StatePause musicvolume " << SoundManager::musicvolume << "\n";
	//std::cout << "StatePause fxvolume " << SoundManager::fxvolume << "\n";

}

void StatePause::OnEnter(void) {
	Mainwindow::device->setInputReceivingSceneManager(smgr);

	std::cout << "StatePause pausing \n";
	GameState* tmp = statemanager.getActiveGameState();
	while (tmp) {
		//std::cout <<"StatePause pausing " << tmp->getName().c_str() <<"\n";
		tmp->setActive(false);
		//pausedStates.push_back(tmp);
		tmp = statemanager.getActiveGameState();
	}

	setActive(true);

	lastInterfaceAction = Mainwindow::device->getTimer()->getTime();

}

//this is called everytime when another GameState is entered
//so first leave this (e.g. destroy the map)
void StatePause::OnLeave(void) {
	//statemanager.getGameState("InGame")->setActive(true);

	// reset Gametimer
	//GameIrrlicht::m_gametimerIrrlicht->updateOldTime();

}

//here we get the user input
//this method is derived by IEventReceiver
bool StatePause::OnEvent(const irr::SEvent &event) {
	return true;
}
;

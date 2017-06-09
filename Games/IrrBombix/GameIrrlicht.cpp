/*
 * Copyright 2011
 * Copyright 2008 Mathias Kraus <k.hias@gmx.de>
 * Copyright 2007-2008 Thomas Gallinari <tg8187@yahoo.fr>
 * Copyright 2007-2008 Pierre-Benoit Bessse <besse@gmail.com>
 *
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

#include "GameIrrlicht.h"
#include "GameItemIrrlicht.h"
#include "ArenaIrrlicht.h"
#include "MapParserIrrlicht.h"
#include "ControllerManager.h"
#include "IrrlichtEventReceiver.h"
#include "BombExplosionIrrlicht.h"
#include "BombIrrlicht.h"
#include "FlyBombIrrlicht.h"
#include "BombItemIrrlicht.h"
#include "GameTimerIrrlicht.h"

#include "Bonus/BonusIrrlicht.h"

#include "FallingBlockIrrlicht.h"

#include "GameStateManager.h"

#include "States/StateArenaSelectboard.h"
#include "States/StateOptionsboard.h"

#include "PlayerIrrlicht.h"
#include <iostream>

GameSceneIrrlicht* GameIrrlicht::m_GameItemIrrlicht = 0;
GameTimerIrrlicht* GameIrrlicht::m_gametimerIrrlicht = 0;
irr::core::array<PlayerIrrlicht*> GameIrrlicht::players = 0;
irr::core::array<BombIrrlicht*> GameIrrlicht::bombs = 0;
irr::core::array<BonusIrrlicht*> GameIrrlicht::Bonus = 0;
irr::core::array<FallingBlockIrrlicht*> GameIrrlicht::fallingblocks = 0;
;
irr::core::array<BombExplosionIrrlicht*> GameIrrlicht::bombexplosions = 0;
ArenaIrrlicht* GameIrrlicht::arena = 0;
GameIrrlicht* GameIrrlicht::game = 0;

GameIrrlicht::GameIrrlicht() {
	// Initialize the game

	// init game
	//initGame();
	// Setup the window
	// low budget singleton
	GameIrrlicht::game = this;
}

GameIrrlicht::~GameIrrlicht() {

}

irr::core::array<PlayerIrrlicht*> GameIrrlicht::getPlayersIrrlicht() {
	return players;
}

void GameIrrlicht::clear() {
	//GameIrrlicht::players.clear();
	GameIrrlicht::bombs.clear();
	GameIrrlicht::Bonus.clear();
	GameIrrlicht::bombexplosions.clear();
	GameIrrlicht::fallingblocks.clear();
}

void GameIrrlicht::initGame() {
		// Create IrrLichtGui
	// creating and drawing player in new Game() needs smgr..
	if (!m_GameItemIrrlicht) {
		m_GameItemIrrlicht = new GameSceneIrrlicht(this);
	} else {
		m_GameItemIrrlicht->smgr->clear();
	}

	// set old duplicate variables.. TODO
	m_GameItemIrrlicht->InitDriver();

	// Create a new Game instance

	MapparserIrrlicht* mapparser = new MapparserIrrlicht();
	GameIrrlicht::arena = mapparser->getArena();


	mapparser->parse(
			StateArenaSelectboard::statearenaselectboard->getValue("").c_str());

	// std::cout << "Map parsing done\n";

	ArenaIrrlicht* tmp = arena;
	m_GameItemIrrlicht->setArenaIrrlicht(tmp);

	// std::cout << "setArena done\n";

	tmp->sortPlayerPosition();

	// set playerpositions
	for (int j = 0; j < players.size(); j++) {
		//	for (int j = 0; j < tmp->getPlayerPositionNr(); j++) {
		int postmp = j % tmp->getPlayerPositionNr();
		irr::core::vector2di pos = tmp->getPlayerPosition(postmp);
		//irr::core::vector2di pos = tmp->getPlayerPosition(j);
		irr::core::vector2df posf = tmp->getCoords(pos.X, pos.Y);
		std::cout << "GameIrrlicht";
		std::cout << "pos.X " << pos.X << "pos.Y" << pos.Y << "\n";
		std::cout << "posf.X " << posf.X << "pos.Y" << posf.Y << "\n";
		players[j]->setArena(arena);
		players[j]->setX(posf.X);
		players[j]->setY(posf.Y);

		// set alive
		players[j]->resurrect();
		// update Cell
		players[j]->move(players[j]->getX(), players[j]->getY());

		// std::cout << "GameIrrlicht placing" << players[j]->getPlayerName().c_str()<<" done\n";

	}

	// init FallingBlocks
	FallingBlockIrrlicht::initAllFallingBlocksIrrlicht();

	//m_GameItemIrrlicht->m_game=m_game;

	//m_GameItemIrrlicht->setArenaIrrlicht(new ArenaIrrlicht());

	//m_GameItemIrrlicht->getArenaIrrlicht()->init(15,15);


	/* show the menu */
	//m_GameItemIrrlicht->CamStats();


	m_gametimerIrrlicht = new GameTimerIrrlicht(device);

	m_GameItemIrrlicht->init();



}

void GameIrrlicht::GameIrrlicht::update() {
	// update the time
	m_gametimerIrrlicht->update();
	// update the player
	playerController->update();

	updatePlayers();

	updateBombExplosions();

	// update the bombs
	updateBombs();

	// before bombs so dead people cant place bombs anymore
	//updateBombExplosions();

	updateBonus();

	updateFallingBlocks();

	checkend();
device->sleep(10,0);
	//	irr::core::vector3df tmpv = m_GameItemIrrlicht->cam->getAbsolutePosition();
	//	std::cout << "CamPos";
	//	std::cout << "Vector3df ("<< tmpv.X <<", "<< tmpv.Y <<", "<< tmpv.Z <<")\n";

}

void GameIrrlicht::updateFallingBlocks() {
	//std::cout << "updateFallingBlocks in GameIrrlicht  start\n";
	for (int j = 0; j < fallingblocks.size(); j++) {
		fallingblocks[j]->update();
	}
	//std::cout << "updateFallingBlocks in GameIrrlicht end\n";

}

void GameIrrlicht::GameIrrlicht::checkend() {
	int livingplayers = 0;
	PlayerIrrlicht* winner = 0;
	for (int j = 0; j < players.size(); j++) {
		if (players[j]->isAlive()) {
			livingplayers++;
			winner = players[j];
		}
	}

	if (livingplayers == 1) {
		std::cout << "Player " << winner->getPlayerName().c_str() << " won!\n";
		winner->addPoint();
		statemanager.changeGameState("AfterGame");
		state = 1;
	}
	if (livingplayers == 0) {
		std::cout << "Draw!\n";
		statemanager.changeGameState("DrawGame");
		state = 1;
	}

	std::stringstream ss1;
	ss1.str(StateOptionsboard::stateoptionsboard->getValue("Party Time"));
	// std::cout << "GameSceneIrrlicht drawclock " <<StateOptionsboard::stateoptionsboard->getValue("Party Time")<<"\n";
	int roundtime;
	ss1 >> roundtime;
	int displaytime = roundtime * 1000
			- GameIrrlicht::m_gametimerIrrlicht->getTime();

	if (displaytime < 0 and livingplayers > 1) {
		// std::cout << "Draw!\n";
		statemanager.changeGameState("DrawGame");
		state = 1;
	}

}

void GameIrrlicht::updateBonus() {
	// collect Bonus
	for (int j = 0; j < Bonus.size(); j++) {
		Bonus[j]->update();
		Bonus[j]->getView()->update();
		if (Bonus[j]->collected){
			Bonus[j]->destroy();
		}
	}

	// apply Bonus to player

	for (int i = 0; i < players.size(); i++) {
		//std::cout << "apply in GameIrrlicht getBonussize "<< GameIrrlicht::players[i]->getBonustaken()->size() << "\n";
		if (GameIrrlicht::players[i]->isAlive()) {
			for (int j = 0; j
					< GameIrrlicht::players[i]->getBonustaken()->size(); j++) {
				//std::cout << "apply in GameIrrlicht " << j << "\n";
				//(GameIrrlicht::players[i]->getBonustaken()[j])->apply(GameIrrlicht::players[i]);
				(GameIrrlicht::players[i]->getBonustaken())->operator [](j) ->apply(
						GameIrrlicht::players[i]);
			}
		}
	}
}

void GameIrrlicht::updateBombExplosions() {
	for (int j = 0; j < bombexplosions.size(); j++) {
		bombexplosions[j]->update();
	}
}

void GameIrrlicht::updateBombs() {
	// create Bombs
	for (int j = 0; j < players.size(); j++) {

		if (players[j]->isAlive()) {
			FlyBombIrrlicht* aspot =
					dynamic_cast<FlyBombIrrlicht*> (players[j]->getbombspot());

			//std::cout << "Bomb try aspot" << aspot	<< GameIrrlicht::m_gametimerIrrlicht->getTime() << "\n";
			if (aspot) {
				players[j]->clearbombspot();
				Cell* bombCell = arena->getCell(arena->getColFromX(
						aspot->getX()), arena->getRowFromY(aspot->getY()));
				//std::cout << "Bomb try placed at" << bombCell->getI() << " "<< bombCell->getJ() << "\n";
				FlyBombIrrlicht* bombonCell =
						dynamic_cast<FlyBombIrrlicht*> (bombCell->getBomb());
				if (!bombonCell) {
					//std::cout << "Bomb placed " << bombCell->getI() << " "<< bombCell->getJ() << "\n";
					aspot->setlaytime(m_gametimerIrrlicht->getTime());
					aspot->setarena(arena);
					//BombIrrlicht* tmpbomb = new BombIrrlicht(arena, m_gametimerIrrlicht->getTime(),players[j]->getBombPower());
					//std::cout<<"GameIrrlicht bom coord "<<(arena->getCoords(aspot->X,aspot->Y).Width)<<(arena->getCoords(aspot->X,aspot->Y).Height);
					irr::core::dimension2df pos = arena->getCoords(
							bombCell->getI(), bombCell->getJ());
					aspot->setX(pos.Width);
					aspot->setY(pos.Height);
					aspot->landed = true;
					// std::cout << "Bomb placed x,y" << pos.Width << " "	<< pos.Height << "\n";
					bombCell->setBomb(aspot);
					bombs.push_back(aspot);

					aspot->getView()->setVisible(true);
					aspot->getView()->update();

				} else {
					// std::cout << "Bomb returning bomb " << bombCell->getI() << " " << bombCell->getJ() << "\n";
					aspot->toArsenal();
					// return bomb;
				}
			}
		}
	}
	// update bombs

	int bombcount = bombs.size();
	//std::cout <<"GameIrrlicht bombcount"<<bombcount <<"\n";
	for (int j = 0; j < bombcount; j++) {
		//std::cout <<"im stuck here 1";
		bombs[j]->update();
		BombItemIrrlicht* tmp =
				dynamic_cast<BombItemIrrlicht*> (bombs[j]->getView());
		tmp->update();
	}

	//delete bombs
	for (int j = 0; j < bombs.size(); j++) {
		//std::cout <<"im stuck here 2";
		if (bombs[j]->getExploded()) {

			bombs[j]->remove();
			BombItemIrrlicht* tmp =
					dynamic_cast<BombItemIrrlicht*> (bombs[j]->getView());

			bombs.erase(j);
			j = j - 1;

		}

	}
	//std::cout <<"GameIrrlicht bombs.size() "<<bombs.size() <<"\n";
	//std::cout << "GameIrrlicht updateBombs done \n";
}

void GameIrrlicht::updatePlayers() {
	for (int j = 0; j < players.size(); j++) {
		players[j]->update();
	}

}

irr::IrrlichtDevice* GameIrrlicht::getDevice() {
	return this->device;
}

void GameIrrlicht::setDevice(irr::IrrlichtDevice * device) {
	this->device = device;
}

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

#include "BombExplosionIrrlicht.h"
#include "BombExplosionItemIrrlicht.h"
#include <iostream>
#include "GameIrrlicht.h"
#include "GameTimerIrrlicht.h"
#include "ArenaIrrlicht.h"
#include "BlockIrrlicht.h"
#include "PlayerIrrlicht.h"
#include "Cell.h"
#include "Bonus/BonusIrrlicht.h"


BombExplosionIrrlicht::BombExplosionIrrlicht(irr::u32 time) :
	IJElementIrrlicht() {
	// TODO Auto-generated constructor stub
	//this->setView(this);
	bombexplosionItemIrrlicht* tmp = new bombexplosionItemIrrlicht(this);
	this->setView(tmp);
	tmp->setModel(this);
	done = false;
	starttime = time;
	explodetime = 1000.0;
	exploded = false;
	bombspot = false;
	GameIrrlicht::bombexplosions.push_back(this);

	//std::cout << this->getI() << ", "<< this->getJ() <<" in BombExplosionIrrlicht\n";
}

BombExplosionIrrlicht::~BombExplosionIrrlicht() {
	// TODO Auto-generated destructor stub
}

void BombExplosionIrrlicht::update() {
	if (!done) {
		if (starttime < GameIrrlicht::m_gametimerIrrlicht->getTime()) {
			//std::cout << this->getI() << ", " << this->getJ()	<< " start in BombExplosionIrrlicht\n";
			explode();
		}
		if (starttime + explodetime
				< GameIrrlicht::m_gametimerIrrlicht->getTime()) {
			//std::cout << this->getI() << ", " << this->getJ()<< " end in BombExplosionIrrlicht\n";
			//bombexplosionItemIrrlicht* tmp =dynamic_cast<bombexplosionItemIrrlicht*> (getView());
			//tmp->setVisible(false);
			done = true;
		}
	}
}

void BombExplosionIrrlicht::explode() {
	if (!exploded) {
		exploded = true;

		Cell* leCell = GameIrrlicht::arena->getCell(getI(), getJ());
		leCell->setExplosion(this);

		if (leCell->getBonus() && !leCell->getBlock()) {
			leCell->getBonus()->destroy();
			//leCell->getBonus()->getView()->setVisible(false);
		}

		if (leCell->getBlock()) {
			leCell->getBlock()->destroy();

			if (leCell->getBonus()) {
				leCell->getBonus()->getView()->setVisible(true);
			}
		}

		irr::core::array<PlayerIrrlicht*>* players = leCell->getPlayers();
		irr::core::array<PlayerIrrlicht*>* deads = new irr::core::array<PlayerIrrlicht*>;
		//std::cout << players->size()<< " deads in BombExplosionIrrlicht\n";
		for (int i = 0; i < players->size(); i++) {
			deads->push_back(players->operator [](i));
		}
		for (int i = 0; i < deads->size(); i++) {
			players->operator [](i)->die();
		}

		// update view
		//bombexplosionItemIrrlicht* tmp =	dynamic_cast<bombexplosionItemIrrlicht*> (getView());
		//tmp->init(0);
		//tmp->setVisible(true);


	}
}
;

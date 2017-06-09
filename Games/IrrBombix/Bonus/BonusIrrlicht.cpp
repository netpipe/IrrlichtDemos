/*
 * Copyright 2011
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

#include "BonusIrrlicht.h"
#include "../Cell.h"
#include "../GameIrrlicht.h"
#include "../ArenaIrrlicht.h"
#include "../PlayerIrrlicht.h"
#include "BonusXMLitemIrrlicht.h"
#include <iostream>

BonusIrrlicht::BonusIrrlicht() {
	GameIrrlicht::Bonus.push_back(this);
	playtakesound=false;
	collected = false;
	//// TODO Auto-generated constructor stub

}

BonusIrrlicht::~BonusIrrlicht() {
	// TODO Auto-generated destructor stub
}

void BonusIrrlicht::destroy(){
	Cell* leCell = GameIrrlicht::arena->getCell(getI(), getJ());
	leCell->removeBonus();
	this->getView()->setVisible(false);
	GameIrrlicht::Bonus.erase(GameIrrlicht::Bonus.linear_reverse_search(this));
}

void BonusIrrlicht::update(){
	for (int i=0;i<GameIrrlicht::players.size();i++){
		// Collect Bonus
		 int curCellRow = GameIrrlicht::arena->getRowFromY(GameIrrlicht::players[i]->getX());
		 int curCellCol = GameIrrlicht::arena->getColFromX(GameIrrlicht::players[i]->getY());
		 if (getJ()== curCellCol && getI() ==curCellRow && GameIrrlicht::players[i]->isAlive()){
			 // std::cout << GameIrrlicht::players[i]->getBonustaken()->size() <<"collect in BonusIrrlicht\n";
			 GameIrrlicht::players[i]->getBonustaken()->push_back(this);


			 //apply(GameIrrlicht::players[i]);
			 collected=true;
			 //destroy();
			 playtakesound=true;
		 }


	}
}

void BonusIrrlicht::apply(PlayerIrrlicht* pl){
	std::cout << "appling Bonus\n";
}

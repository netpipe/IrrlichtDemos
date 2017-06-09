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

#include "FallingBlockIrrlicht.h"
#include "Cell.h"
#include "GameIrrlicht.h"
#include "ArenaIrrlicht.h"
#include "PlayerIrrlicht.h"
#include "GameTimerIrrlicht.h"
#include "BlockIrrlicht.h"
#include "BombIrrlicht.h"

#include "States/StateOptionsboard.h"

//TODO nice modell
#include "FallingBlockItemIrrlicht.h"

#include <iostream>

bool FallingBlockIrrlicht::ufoDisabled = false;

FallingBlockIrrlicht::FallingBlockIrrlicht() {
	FallingBlockItemIrrlicht* tmp = new FallingBlockItemIrrlicht(this,
			"./media/Items/FallingBlock.xml");
	this->setView(tmp);
	tmp->setModel(this);
	showtime = 1500;
	GameIrrlicht::fallingblocks.push_back(this);
	//// TODO Auto-generated constructor stub

}

FallingBlockIrrlicht::~FallingBlockIrrlicht() {
	// TODO Auto-generated destructor stub
}

void FallingBlockIrrlicht::initAllFallingBlocksIrrlicht() {
	int rightturns = 0;
	int rowstep = GameIrrlicht::arena->getNbColumns();
	int colstep = GameIrrlicht::arena->getNbRows();

	int mirror = GameIrrlicht::arena->getNbRows() - 1;

	int rowpos = 0;
	int colpos = 0;

	int startdietime = 1000;
	int deltatime = 200;
	int deltadie = 250;

	std::stringstream ss1;
	ss1.str(StateOptionsboard::stateoptionsboard->getValue("Party Time"));
	int tmp;
	ss1 >> tmp;

	/*startdietime = tmp * 0.75 * 1000;
	 deltatime = tmp * 0.25 * 1000 / (rowstep * colstep);
	 */

	int smallarenatime = tmp / 3;
	int gaptime = 0;

	deltatime = 15 * 1000 / (rowstep * colstep);

	startdietime = (tmp - smallarenatime) * 1000 - (rowstep * colstep)
			* deltatime;

	if (startdietime < 0)
		startdietime = 1000;

	while (rowstep > 0 && colstep > 0) {

		//std::cout << "FallingBlockIrrlicht increase rowstep: " << rowstep	<< " colstep: " << colstep << "\n";

		int cutoff = std::min(GameIrrlicht::arena->getNbColumns() / 4,
				GameIrrlicht::arena->getNbRows() / 4);
		if (GameIrrlicht::arena->getNbColumns() - 2 * cutoff == rowstep
				&& GameIrrlicht::arena->getNbRows() - 2 * cutoff == colstep) {
			// gaptime = smallarenatime*1000;
			//std::cout << "FallingBlockIrrlicht increase startdietime\n";
			startdietime = startdietime + smallarenatime * 1000;
		}

		//gaptime = smallarenatime*1000;
		//walking up
		for (int i = 0; i < rowstep; i++) {
			startdietime = startdietime + deltatime;
			if (GameIrrlicht::arena->getCell(rowpos, colpos)->getTerrain()->isWalkable()) {
				FallingBlockIrrlicht* tmp = new FallingBlockIrrlicht;
				tmp->setI(rowpos);
				tmp->setJ(mirror - colpos);
				tmp->dietime = startdietime + gaptime;
				tmp->landtime = tmp->dietime + deltadie;

			}
			rowpos = rowpos + 1;
		}
		rowpos = rowpos - 1;
		colpos = colpos + 1;
		colstep = colstep - 1;

		for (int j = 0; j < colstep; j++) {
			startdietime = startdietime + deltatime;
			if (GameIrrlicht::arena->getCell(rowpos, colpos)->getTerrain()->isWalkable()) {
				FallingBlockIrrlicht* tmp = new FallingBlockIrrlicht;
				tmp->setI(rowpos);
				tmp->setJ(mirror - colpos);

				tmp->dietime = startdietime + gaptime;
				tmp->landtime = tmp->dietime + deltadie;

			}
			colpos = colpos + 1;

		}
		colpos = colpos - 1;
		rowpos = rowpos - 1;
		rowstep = rowstep - 1;

		for (int i = 0; i < rowstep; i++) {
			startdietime = startdietime + deltatime;
			if (GameIrrlicht::arena->getCell(rowpos, colpos)->getTerrain()->isWalkable()) {
				FallingBlockIrrlicht* tmp = new FallingBlockIrrlicht;
				tmp->setI(rowpos);
				tmp->setJ(mirror - colpos);

				tmp->dietime = startdietime + gaptime;
				tmp->landtime = tmp->dietime + deltadie;
			}
			rowpos = rowpos - 1;
		}
		rowpos = rowpos + 1;
		colpos = colpos - 1;
		colstep = colstep - 1;
		for (int j = 0; j < colstep; j++) {
			startdietime = startdietime + deltatime;
			if (GameIrrlicht::arena->getCell(rowpos, colpos)->getTerrain()->isWalkable()) {
				FallingBlockIrrlicht* tmp = new FallingBlockIrrlicht;
				tmp->setI(rowpos);
				tmp->setJ(mirror - colpos);

				tmp->dietime = startdietime + gaptime;
				tmp->landtime = tmp->dietime + deltadie;
			}
			colpos = colpos - 1;
		}
		colpos = colpos + 1;
		rowpos = rowpos + 1;
		rowstep = rowstep - 1;
	}

}

void FallingBlockIrrlicht::destroy() {
	/*
	 leCell->removeBonus();
	 this->getView()->setVisible(false);
	 GameIrrlicht::Bonus.erase(GameIrrlicht::Bonus.linear_reverse_search(this));
	 */
}

void FallingBlockIrrlicht::update() {
	//this->getView()->setVisible(true);


	if (dietime < GameIrrlicht::m_gametimerIrrlicht->getTime()) {
		//this->getView()->setVisible(true);
		Cell* leCell = GameIrrlicht::arena->getCell(getI(), getJ());
		if (leCell->getBlock()) {
			leCell->getBlock()->destroy();
		}
		if (leCell->getBonus()) {
			leCell->getBonus()->destroy();
		}
		if (leCell->getBomb()) {
			//std::cout << "FallingBlockIrrlicht igniting bomb\n";
			leCell->getBomb()->explode();
		}
		//std::cout << "checking  in FallingBlockIrrlicht dietime vs gametime: "<< dietime << " vs  "<< GameIrrlicht::m_gametimerIrrlicht->getTime() << "\n";
		for (int i = 0; i < GameIrrlicht::players.size(); i++) {
			// Collect Bonus
			int curCellRow = GameIrrlicht::arena->getRowFromY(
					GameIrrlicht::players[i]->getX());
			int curCellCol = GameIrrlicht::arena->getColFromX(
					GameIrrlicht::players[i]->getY());
			if (this->getJ() == curCellCol && this->getI() == curCellRow) {
				//std::cout << "die in FallingBlockIrrlicht PLayer: " << i	<< "\n";
				GameIrrlicht::players[i]->die();

				//apply(GameIrrlicht::players[i]);
			}
		}
	}
	if (landtime < GameIrrlicht::m_gametimerIrrlicht->getTime()) {
		Cell* leCell = GameIrrlicht::arena->getCell(getI(), getJ());
		leCell->setTerrain(this);
		if (leCell->getBomb())
			leCell->getBomb()->update();

	}

	// Disable ufo mode
	if (!ufoDisabled) {
		if (dietime - showtime < GameIrrlicht::m_gametimerIrrlicht->getTime()) {
			for (int i = 0; i < GameIrrlicht::players.size(); i++) {
				GameIrrlicht::players[i]->isUfo = false;
			}
			ufoDisabled = true;
		}
	}
}

/*
 bool FallingBlockIrrlicht::isWalkable(){
 // if is not really needed because its only called after landtime at the moment
 if (landtime < GameIrrlicht::m_gametimerIrrlicht->getTime()) {
 return false;
 }
 return true;
 }
 */


/*
 * Copyright 2011
 * Copyright 2009 Mathias Kraus <k.hias@gmx.de>
 * Copyright 2007-2008 Thomas Gallinari <tg8187@yahoo.fr>
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

#include "ArenaIrrlicht.h"
#include "XYElement.h"
#include "PlayerIrrlicht.h"
#include "ElementItemIrrlicht.h"
#include "PlayerItemIrrlicht.h"
#include "GameIrrlicht.h"
#include <iostream>
#include <cmath>
#include "BombIrrlicht.h"
#include "FlyBombIrrlicht.h"
#include "Bonus/BonusIrrlicht.h"
#include "FallingBlockIrrlicht.h"

#include "States/StateOptionsboard.h"

#include "GameTimerIrrlicht.h"

const int onIceSpeedIncrease = 2;
const int badBonusTimerTimeout = 100;
const int badBonusCountdown = 10000;

PlayerIrrlicht::PlayerIrrlicht(irr::f32 p_x, irr::f32 p_y,
		const irr::core::stringc& p_playerID, ArenaIrrlicht* p_arena) :
	XYElement(p_x, p_y, p_arena) {
	PlayerItemIrrlicht* tmp = new PlayerItemIrrlicht(this);
	this->setView(tmp);
	tmp->setModel(this);

	m_playerName = p_playerID;

	m_points = 0;
	m_but1 = UP;
	m_but2 = UP;

	clearbombspot();

	resurrect();

}

PlayerIrrlicht::~PlayerIrrlicht() {
}

irr::core::stringc PlayerIrrlicht::getPlayerName() const {
	return m_playerName;
}

void PlayerIrrlicht::init() {
	updateDirection();
	stopMoving();
}

void PlayerIrrlicht::igniteBomb() {
	//if (!m_death) {
	if (!m_restrain) {
		// at most once per cycle
		if (!bombspot) {
			// std::cout << "igniteBomb try " << m_x << ", " << m_y << ", " << bombArsenal.size()	<< GameIrrlicht::m_gametimerIrrlicht->getTime() << "\n";
			if (bombArsenal.size() > 0) {
				BombIrrlicht* prepare = bombArsenal.getLast();
				prepare->setBombPower(m_bombPower);
				// std::cout << "igniteBomb " << prepare << "\n";

				bombArsenal.erase(bombArsenal.size() - 1);
				prepare->setX(m_x);
				prepare->setY(m_y);
				bombspot = prepare;
			}
		}
	}
}
//}

void PlayerIrrlicht::button1Pressed() {
	if (m_but1 == PRESSED)
		m_but1 = DOWN;
	if (m_but1 == UP)
		m_but1 = PRESSED;
}

void PlayerIrrlicht::button1Released() {
	if (m_but1 == RELEASED)
		m_but1 = UP;

	if (m_but1 == PRESSED)
		m_but1 = RELEASED;

	if (m_but1 == DOWN)
		m_but1 = RELEASED;

}

void PlayerIrrlicht::button2Pressed() {
	if (m_but2 == PRESSED)
		m_but2 = DOWN;
	if (m_but2 == UP)
		m_but2 = PRESSED;
}

void PlayerIrrlicht::button2Released() {
	if (m_but2 == RELEASED)
		m_but2 = UP;

	if (m_but2 == PRESSED)
		m_but2 = RELEASED;

	if (m_but2 == DOWN)
		m_but2 = RELEASED;

}

void PlayerIrrlicht::goUp() {
	m_askedXSpeed = 0;

	int nSpeed = m_speed;
	if (m_onIce) {
		nSpeed = m_speed + onIceSpeedIncrease;
	}
	m_askedYSpeed = +nSpeed;

}

void PlayerIrrlicht::goDown() {
	m_askedXSpeed = 0;

	int nSpeed = m_speed;
	if (m_onIce) {
		nSpeed = m_speed + onIceSpeedIncrease;
	}
	m_askedYSpeed = -nSpeed;

}

void PlayerIrrlicht::goRight() {
	int nSpeed = m_speed;
	if (m_onIce) {
		nSpeed = m_speed + onIceSpeedIncrease;
	}
	m_askedXSpeed = nSpeed;

	m_askedYSpeed = 0;

}

void PlayerIrrlicht::goLeft() {
	int nSpeed = m_speed;
	if (m_onIce) {
		nSpeed = m_speed + onIceSpeedIncrease;
	}
	m_askedXSpeed = -nSpeed;

	m_askedYSpeed = 0;

}

void PlayerIrrlicht::updateDirection() {
	// if (m_death) {
	// return;
	// }

	setXSpeed(m_askedXSpeed);
	setYSpeed(m_askedYSpeed);

	if (m_askedXSpeed != 0 || m_askedYSpeed != 0) {
		lastXSpeed = m_askedXSpeed;
		lastYSpeed = m_askedYSpeed;
	}

	m_askedXSpeed = 0;
	m_askedYSpeed = 0;

}

void PlayerIrrlicht::update() {
	if (!m_death) {

		int CellCol;
		int CellRow;

		// Get the current Cell coordinates from the character coordinates
		int curCellJ = m_arena->getRowFromY(m_y);
		int curCellI = m_arena->getColFromX(m_x);

		FlyBombIrrlicht* bombonCell =
				dynamic_cast<FlyBombIrrlicht*> (m_arena->getCell(curCellI,
						curCellJ)->getBomb());

		if (bombonCell) {
			if (m_but1 == PRESSED and hasThrowBomb()) {
				//std::cout << "playerirrlicht thowing " << bombonCell << "\n";
				bombonCell->throwBomb(lastXSpeed, lastYSpeed, 3);
			}
		} else {
			if (m_but1 == PRESSED) {
				igniteBomb();
			}
		}
		updateMove();
	} else {
		if (!isUfo) {
			if (deadtime == -1)
				deadtime = GameIrrlicht::m_gametimerIrrlicht->getTime();
			if (deadtime + 2000 < GameIrrlicht::m_gametimerIrrlicht->getTime()) {
				std::cout << "PlayerIrrlicht " << deadtime + 2000 << "<"
						<< GameIrrlicht::m_gametimerIrrlicht->getTime() << "\n";
				startUfo();
			}
		}
	}

	if (isUfo) {
		if (m_but1 == PRESSED) {
			if (bombArsenal.size() > 0) {
				FlyBombIrrlicht* bombonCell =
						dynamic_cast<FlyBombIrrlicht*> (bombArsenal.getLast());
				bombArsenal.erase(bombArsenal.size() - 1);
				if (bombonCell) {
					// std::cout << "playerirrlicht ufo thowing " << bombonCell<< "\n";
					bombonCell->setX(getX());
					bombonCell->setY(getY());
					// turn to middle
					int horizonly = GameIrrlicht::arena->getColFromX(getX())
							!= 0 && GameIrrlicht::arena->getColFromX(getX())
							!= GameIrrlicht::arena->getNbColumns() - 1;
					int verticonly = GameIrrlicht::arena->getRowFromY(getY())
							!= 0 && GameIrrlicht::arena->getRowFromY(getY())
							!= GameIrrlicht::arena->getNbRows() - 1;

					int dx = lastXSpeed == 0;
					int dy = lastYSpeed == 0;

					dx = dx && !horizonly;
					dy = dy || horizonly;

					dx = dx || verticonly;
					dy = dy && !verticonly;

					int dmx = (GameIrrlicht::arena->getNbColumns() * Cell::SIZE
							/ 2) - getX();
					int dmy = (GameIrrlicht::arena->getNbRows() * Cell::SIZE
							/ 2) - getY();

					bombonCell->throwBomb(dx * dmx, dy * dmy, 2);
					bombonCell->setBombPower(1);
					bombonCell->setarena(GameIrrlicht::arena);
					bombonCell->setlaytime(
							GameIrrlicht::m_gametimerIrrlicht->getTime());
					GameIrrlicht::bombs.push_back(bombonCell);
					//igniteBomb();
				}
			}
		}

		updateMove();

	}

}

void PlayerIrrlicht::updateMove() {
	if (m_death && !isUfo) {
		return;
	}
	int CellCol;
	int CellRow;

	// Get the current Cell coordinates from the character coordinates
	int curCellJ = m_arena->getRowFromY(m_y);
	int curCellI = m_arena->getColFromX(m_x);

	if (m_mirror) {
		m_xSpeed = -m_xSpeed;
		m_ySpeed = -m_ySpeed;
		m_askedXSpeed = -m_askedXSpeed;
		m_askedYSpeed = -m_askedYSpeed;
	}

	//check if there is a hurdle in the way
	if (m_askedXSpeed != 0 || m_xSpeed != 0 || m_askedYSpeed != 0 || m_ySpeed
			!= 0) {
		int xDirection = 0; //x-direction: -1: move left; 0: not moving; 1: move right
		int yDirection = 0; //y-direction: -1: move up; 0: not moving; 1: move down
		int straightDirection = 0; //straight direction: -1: backward; 1:foreward; while foreward is right for moving in x-direction and down for y-direction
		irr::f32 deltaStraightMove = 0; //the move in straight direction
		irr::f32 deltaPerpendicularMove = 0; //the move in perpendicular direction; e.g. the player is not in Cell center and will collide with a wall in the Cell above, so the player has to be moved to the Cell center
		irr::f32 deltaAskedMove; //how far to move; positive for right/down move and negative for left/up
		irr::f32 deltaStraightCellCorner; //move in x-direction: the x-pos from the top left Cell corner; move in y-direction: the y-pos from the top left Cell corner
		irr::f32 deltaPerpendicularCellCorner; //move in x-direction: the y-pos from the top left Cell corner; move in y-direction: the x-pos from the top left Cell corner
		irr::f32 deltaStraightCellCenter; //distance to the Cell center in moving direction; positive if left/up from Cell center, negative if right/down
		irr::f32 deltaPerpendicularCellCenter; //distance to the Cell center perpendicular to moving direction; positive if up/left from Cell center, negative if down/right
		bool bMoveWithinNextCellCenter = false; //move is completed without exceeding the Cell center


		//set variables for right/left move
		if (m_askedXSpeed != 0 || m_xSpeed != 0) {
			//how far to move
			deltaAskedMove = (m_askedXSpeed != 0 ? m_askedXSpeed : m_xSpeed);

			//direction to move
			xDirection = sign(deltaAskedMove);
			straightDirection = xDirection;

			deltaStraightCellCorner = m_x - curCellI * Cell::SIZE;
			deltaPerpendicularCellCorner = m_y - curCellJ * Cell::SIZE;
		} else //set variables for down/up move
		{
			//how far to move
			deltaAskedMove = (m_askedYSpeed != 0 ? m_askedYSpeed : m_ySpeed);

			//direction to move
			yDirection = sign(deltaAskedMove);
			straightDirection = yDirection;

			deltaStraightCellCorner = m_y - curCellJ * Cell::SIZE;
			deltaPerpendicularCellCorner = m_x - curCellI * Cell::SIZE;
		}

		//how far to current Cell center
		deltaStraightCellCenter = Cell::SIZE / 2 - deltaStraightCellCorner;
		deltaPerpendicularCellCenter = Cell::SIZE / 2
				- deltaPerpendicularCellCorner;

		//check if the move exceeds a Cell center
		if (straightDirection * deltaStraightCellCenter >= 0) {
			if (fabs(deltaAskedMove) <= fabs(deltaStraightCellCenter)) {
				bMoveWithinNextCellCenter = true;
			}
		} else if (fabs(deltaAskedMove) + fabs(deltaStraightCellCenter)
				<= Cell::SIZE) {
			bMoveWithinNextCellCenter = true;
		}

		//the move is within two Cell centers
		if (bMoveWithinNextCellCenter) {
			deltaStraightMove += deltaAskedMove;
			//move to perpendicular center if needed
			if (deltaPerpendicularCellCenter != 0 && (straightDirection
					* deltaStraightCellCenter) < 0) //not in perpendicular center and entering a new Cell
			{
				if (fabs(deltaPerpendicularCellCenter) > Cell::SIZE / 2 - fabs(
						deltaStraightMove - deltaStraightCellCenter)) //check if it already can collide with a hurdle
				{
					CellRow = curCellJ + yDirection - abs(xDirection)
							* signZeroPositive(deltaPerpendicularCellCenter);
					CellCol = curCellI + xDirection - abs(yDirection)
							* signZeroPositive(deltaPerpendicularCellCenter);

					//std::cout<< "Player i,j  iswalkable  "<<CellRow <<", "<<CellCol<<"\n";
					if (!isWalkable(m_arena->getCell(CellCol, CellRow))) {
						deltaPerpendicularMove
								= deltaPerpendicularCellCenter
										+ signZeroPositive(
												deltaPerpendicularCellCenter)
												* (fabs(
														deltaStraightMove
																- deltaStraightCellCenter)
														- Cell::SIZE / 2);
						if (fabs(deltaPerpendicularMove) > fabs(
								deltaPerpendicularCellCenter)) //check if moved over perpendicular center
						{
							deltaPerpendicularMove
									= deltaPerpendicularCellCenter;
						}
					}
				}
			}
		} else //the move exceeds a Cell center
		{
			//at first move to the Cell center
			deltaStraightMove += deltaStraightCellCenter;
			deltaAskedMove -= deltaStraightCellCenter;
			if (straightDirection * deltaStraightCellCenter < 0) //the Cell center to move is in the next Cell
			{
				deltaStraightMove += straightDirection * Cell::SIZE;
				deltaAskedMove -= straightDirection * Cell::SIZE;

				//move to perpendicular center if needed
				if (deltaPerpendicularCellCenter != 0) {
					CellRow = curCellJ + yDirection - abs(xDirection)
							* signZeroPositive(deltaPerpendicularCellCenter);
					CellCol = curCellI + xDirection - abs(yDirection)
							* signZeroPositive(deltaPerpendicularCellCenter);

					//std::cout<< "Player i,j  iswalkable  "<<CellRow <<", "<<CellCol<<"\n";
					if (!isWalkable(m_arena->getCell(CellCol, CellRow))) {
						deltaPerpendicularMove = deltaPerpendicularCellCenter;
					}
				}

				//update the current Cell and row
				curCellI += xDirection;
				curCellJ += yDirection;
			}
			while (fabs(deltaAskedMove) > 0) //complete the move
			{
				if (isWalkable(m_arena->getCell(curCellI + xDirection, curCellJ
						+ yDirection))) //check if next Cell is walkable
				{
					if (fabs(deltaAskedMove) > Cell::SIZE) //move to next Cell center if the reMaining move exceeds a Cell center
					{
						deltaStraightMove += straightDirection * Cell::SIZE;
						deltaAskedMove -= straightDirection * Cell::SIZE;
						//move to perpendicular center if needed
						if (deltaPerpendicularCellCenter != 0) {
							CellRow = curCellJ + yDirection - abs(xDirection)
									* signZeroPositive(
											deltaPerpendicularCellCenter);
							CellCol = curCellI + xDirection - abs(yDirection)
									* signZeroPositive(
											deltaPerpendicularCellCenter);

							//std::cout<< "Player i,j  iswalkable  "<<CellRow <<", "<<CellCol<<"\n";
							if (!isWalkable(m_arena->getCell(CellCol, CellRow))) {
								deltaPerpendicularMove
										= deltaPerpendicularCellCenter;
							}
						}
					} else {
						deltaStraightMove += deltaAskedMove;
						//move to perpendicular center if needed
						if (deltaPerpendicularMove
								!= deltaPerpendicularCellCenter && fabs(
								deltaPerpendicularCellCenter) > (Cell::SIZE / 2
								- fabs(deltaStraightMove
										- deltaStraightCellCenter))) //check if it is in or already moved to perpendicular center and if it already can collide with a hurdle ***TODO: it seems to be wrong to use deltaStraightMove here, because ist could be greater than Cell::SIZE
						{
							CellRow = curCellJ + yDirection - abs(xDirection)
									* signZeroPositive(
											deltaPerpendicularCellCenter);
							CellCol = curCellI + xDirection - abs(yDirection)
									* signZeroPositive(
											deltaPerpendicularCellCenter);
							//std::cout<< "Player i,j  iswalkable  "<<CellRow <<", "<<CellCol<<"\n";
							if (!isWalkable(m_arena->getCell(CellCol, CellRow))) {
								deltaPerpendicularMove = signZeroPositive(
										deltaPerpendicularCellCenter) * fabs(
										deltaAskedMove);
								if (fabs(deltaPerpendicularMove) > fabs(
										deltaPerpendicularCellCenter)) {
									deltaPerpendicularMove
											= deltaPerpendicularCellCenter; //check if moved over perpendicular center
								}
							}
						}
						deltaAskedMove = 0;
					}
					//update the current Cell and row
					curCellI += xDirection;
					curCellJ += yDirection;
				} else //there is a hurdle in the next Cell, so don't stop moving
				{
					deltaAskedMove = 0;
					CellRow = curCellJ + yDirection;
					CellCol = curCellI + xDirection;

					if (m_arena->getCell(CellCol, CellRow)->getBomb() != NULL
							&& m_kickBomb) {
						m_arena->getCell(CellCol, CellRow)->getBomb()->setXSpeed(
								m_xSpeed);
						m_arena->getCell(CellCol, CellRow)->getBomb()->setYSpeed(
								m_ySpeed);
					}
				}
			}
		}

		// Update the direction
		if (m_askedXSpeed != 0 || m_askedYSpeed != 0) {
			updateDirection();
		}

		// Move the player
		if (xDirection != 0) {
			move(m_x + deltaStraightMove, m_y + deltaPerpendicularMove);
		} else {
			move(m_x + deltaPerpendicularMove, m_y + deltaStraightMove);
		}

	}

}

void PlayerIrrlicht::move(irr::f32 x, irr::f32 y) {
	int curCellJ = m_arena->getRowFromY(m_y);
	int curCellI = m_arena->getColFromX(m_x);
	if (!isUfo) {
		GameIrrlicht::arena->getCell(curCellI, curCellJ)->removePlayer(this);
	}
	// Move the Character
	//std::cout<< "playerImove x,y "<< x<<", "<< y<<"\n";

	//std::cout<< "playerImove x,y "<< x<<", "<< y<<"\n";
	m_x = x;
	m_y = y;

	if (!isUfo) {
		curCellJ = m_arena->getRowFromY(m_y);
		curCellI = m_arena->getColFromX(m_x);
		GameIrrlicht::arena->getCell(curCellI, curCellJ)->addPlayer(this);

		//std::cout<< "playerI placed i,j " << getPlayerName().c_str() << " "<< curCellI<<", "<< curCellJ<<"\n";
		//std::cout<< "playerI Cell size this "<< getPlayerName().c_str() << " "<< GameIrrlicht::arena->getCell(curCellI, curCellJ)->getPlayers()->size() <<" "<<GameIrrlicht::arena->getCell(curCellI, curCellJ)<<"\n";

	}

}

// not used, unsure about how it should act
bool PlayerIrrlicht::shield(int nExplosionID) {
	for (int i = 0; i < m_shields.size(); i++) {
		if (m_shields[i] == nExplosionID) {
			return true;
		} else if (m_shields[i] == 0) {
			m_shields[i] = nExplosionID;
			if (i == m_shields.size() - 1) {
			}
			return true;
		}
	}
	return false;
}

bool PlayerIrrlicht::hasShield() {
	if (m_shields.size() > 0 && m_shields.getLast() == 0) {
		return true;
	}
	return false;
}

bool PlayerIrrlicht::hasThrowBomb() {
	return m_throwBomb;
}

bool PlayerIrrlicht::setThrowBomb() {
	m_throwBomb = true;
}

bool PlayerIrrlicht::hasKickBomb() {
	return m_kickBomb;
}

void PlayerIrrlicht::setKickBomb() {
	m_kickBomb = true;
}

bool PlayerIrrlicht::hasBadBonus() {
	return m_hyperactive || m_scatty || m_slow || m_restrain || m_mirror;
}

void PlayerIrrlicht::die() {
	if (!m_death) {
		m_death = true;

		m_xSpeed = 0;
		m_xSpeed = 0;

		int curCellJ = m_arena->getRowFromY(m_y);
		int curCellI = m_arena->getColFromX(m_x);
		GameIrrlicht::arena->getCell(curCellI, curCellJ)->removePlayer(this);

	}

}

void PlayerIrrlicht::startUfo() {
	if (StateOptionsboard::stateoptionsboard->getValue("Ufo") == "on"
			&& !FallingBlockIrrlicht::ufoDisabled) {
		isUfo = true;
		std::cout << "PlayerIrrlicht isUfo:" << isUfo << "\n";
		bombArsenal.clear();
		Bonustaken.clear();
		m_mirror = false;
		m_slow = false;
		m_scatty = false;
		m_mirror = false;
		m_restrain = false;
		m_bombArmory = 1;
		m_bombPower = 3;

		// move to the nearest side
		if (m_arena->getNbColumns() * Cell::SIZE / 2 - getX() > 0) {
			setX(0.5 * Cell::SIZE);
		} else {
			setX((m_arena->getNbColumns() - 0.5) * Cell::SIZE);
		}
		setZ(getZ() + Cell::SIZE * 2);

		for (int i = 0; i < m_bombArmory; i++) {
			//BombIrrlicht* tmp = new BombIrrlicht();
			FlyBombIrrlicht* tmp = new FlyBombIrrlicht();
			tmp->setowner(this);
			bombArsenal.push_back(tmp);
		}
	}

}

bool PlayerIrrlicht::isAlive() const {
	return !m_death;
}

void PlayerIrrlicht::resurrect() {
	m_onIce = false;
	m_falling = false;
	m_death = false;
	m_maxSpeed = 10;
	m_speed = 5;
	m_normalSpeed = m_speed;
	m_moveMirrored = false;
	m_bombPower = 1;
	m_maxBombArmory = 1;
	m_bombArmory = m_maxBombArmory;
	m_shields.clear();
	m_throwBomb = false;
	m_kickBomb = false;
	m_restrain = 0;
	m_mirror = 0;
	m_hyperactive = 0;
	m_slow = 0;
	m_scatty = 0;
	wonRound = false;
	bombspot = 0;
	isUfo = false;
	deadtime = -1;

	Bonustaken.clear();
	bombArsenal.clear();

	for (int i = 0; i < m_bombArmory; i++) {
		//BombIrrlicht* tmp = new BombIrrlicht();
		FlyBombIrrlicht* tmp = new FlyBombIrrlicht();
		tmp->setowner(this);
		bombArsenal.push_back(tmp);
	}

	if (m_arena) {

		//        int CellRow = m_arena->getRowFromY(m_y);
		//        int CellCol = m_arena->getColFromX(m_x);

		int CellI = m_arena->getColFromX(m_x);
		int CellJ = m_arena->getRowFromY(m_y);

		//if(m_arena->getCell(CellCol,CellRow)->getType() == Cell::HOLE)

		move(m_x, m_y);

		//turn to middle
		int horizonly = GameIrrlicht::arena->getColFromX(getX()) != 0
				&& GameIrrlicht::arena->getColFromX(getX())
						!= GameIrrlicht::arena->getNbColumns() - 1;
		int verticonly = GameIrrlicht::arena->getRowFromY(getY()) != 0
				&& GameIrrlicht::arena->getRowFromY(getY())
						!= GameIrrlicht::arena->getNbRows() - 1;

		// up down per default
		int dx = 0;
		int dy = 1;

		dx = dx && !horizonly;
		dy = dy || horizonly;

		dx = dx || verticonly;
		dy = dy && !verticonly;
		int dmx = (GameIrrlicht::arena->getNbColumns() * Cell::SIZE / 2)
				- getX();
		int dmy = (GameIrrlicht::arena->getNbRows() * Cell::SIZE / 2) - getY();

		lastXSpeed = dx * dmx;
		lastYSpeed = dy * dmy;

	}
	setZ(-1.5 * Cell::SIZE);

	//emit resurrected();
}

int PlayerIrrlicht::points() const {
	return m_points;
}

void PlayerIrrlicht::setpoints(int p) {
	m_points = p;
}

void PlayerIrrlicht::addPoint() {
	m_points++;
	wonRound = true;
}

void PlayerIrrlicht::emitGameUpdated() {
}

irr::f32 PlayerIrrlicht::getAskedXSpeed() const {
	return m_askedXSpeed;
}

void PlayerIrrlicht::setAskedXSpeed(irr::f32 x) {
	m_askedXSpeed = x;
}

irr::f32 PlayerIrrlicht::getAskedYSpeed() const {
	return m_askedYSpeed;
}

void PlayerIrrlicht::setAskedYSpeed(irr::f32 y) {
	m_askedYSpeed = y;
}

Cell* PlayerIrrlicht::getAskedNextCell() {
	// Get the current Cell coordinates from the character coordinates
	int curCellRow = m_arena->getRowFromY(m_y);
	int curCellCol = m_arena->getColFromX(m_x);
	Cell* nextCell;

	// Get the next Cell function of the character asked direction
	if (m_askedXSpeed > 0) {
		nextCell = m_arena->getCell(curCellRow, curCellCol + 1);
	} else if (m_askedXSpeed < 0) {
		nextCell = m_arena->getCell(curCellRow, curCellCol - 1);
	} else if (m_askedYSpeed > 0) {
		nextCell = m_arena->getCell(curCellRow + 1, curCellCol);
	} else if (m_askedYSpeed < 0) {
		nextCell = m_arena->getCell(curCellRow - 1, curCellCol);
	}

	return nextCell;
}

int PlayerIrrlicht::direction() {
	return m_direction;
}

int PlayerIrrlicht::getBombPower() const {
	return m_bombPower;
}

bool PlayerIrrlicht::getWonRound() const {
	return wonRound;
}

void PlayerIrrlicht::setBombPower(int b) {
	m_bombPower = b;
	//for (int i = 0;i<bombArsenal.size();i++){
	//	bombArsenal[i]->setBombPower(m_bombPower);
	//}
}

int PlayerIrrlicht::getSpeed() const {
	return m_speed;
}

void PlayerIrrlicht::setSpeed(int b) {
	m_speed = b;
}

void PlayerIrrlicht::decrementBombArmory() {
	m_bombArmory--;
	if (m_bombArmory < 0) {
		m_bombArmory = 0;
	}
}

int PlayerIrrlicht::getBombArmory() {
	return m_bombArmory;
}

void PlayerIrrlicht::stopMoving() {
	setXSpeed(0);
	setYSpeed(0);
	m_askedXSpeed = 0;
	m_askedYSpeed = 0;

}

BombIrrlicht* PlayerIrrlicht::getbombspot() {
	return bombspot;
}
void PlayerIrrlicht::clearbombspot() {
	bombspot = 0;
	//new irr::core::vector2d<int>(-1,-1);
}

void PlayerIrrlicht::incRestrain(int i) {
	m_restrain = m_restrain + i;
}

void PlayerIrrlicht::incMirror(int i) {
	m_mirror = m_mirror + i;
}

void PlayerIrrlicht::incScatty(int i) {
	m_scatty = m_scatty + i;
}

void PlayerIrrlicht::setArena(ArenaIrrlicht* aI) {
	m_arena = aI;
}

void PlayerIrrlicht::toArsenal(BombIrrlicht* abomb) {
	//abomb->setBombPower(m_bombPower);
	bombArsenal.push_back(abomb);
}

//keyStatesENUM
PlayerIrrlicht::keyStatesENUM PlayerIrrlicht::getbutton1() {
	return m_but1;
}

PlayerIrrlicht::keyStatesENUM PlayerIrrlicht::getbutton2() {
	return m_but2;
}

int PlayerIrrlicht::signZeroPositive(const irr::f32 value) {
	return (value >= 0 ? 1 : -1);
}

int PlayerIrrlicht::sign(const irr::f32 value) {
	if (value == 0) {
		return 0;
	}
	return (value > 0 ? 1 : -1);
}

bool PlayerIrrlicht::isWalkable(Cell* aCell) {
	if (isUfo) {
		return ((aCell->getJ() == 0 || aCell->getJ() == m_arena->getNbRows()
				- 1) && aCell->getI() >= 0 && aCell->getI()
				<= m_arena->getNbColumns()) || (aCell->getJ() >= 0
				&& aCell->getJ() <= m_arena->getNbRows()) && (aCell->getI()
				== 0 || aCell->getI() == m_arena->getNbColumns() - 1);
	} else {
		return aCell->isWalkable();
	}
	return false;
}

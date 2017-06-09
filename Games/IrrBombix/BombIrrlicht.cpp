/*
 * Copyright 2011
 * Copyright 2009 Mathias Kraus <k.hias@gmx.de>
 * Copyright 2007-2008 Thomas Gallinari <tg8187@yahoo.fr>
 * Copyright 2007-2008 Pierre-Benoît Besse <besse.pb@gmail.com>
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

/* No direct use anymore!
 * erverybomb is a flybomb now because every bomb can be kicked.
 * but flybomb uses methods of this class.
 *
 * */


#include "BombIrrlicht.h"
#include "BombItemIrrlicht.h"
#include "BombExplosionIrrlicht.h"
#include "BombExplosionItemIrrlicht.h"
#include "XYElement.h"
#include "ArenaIrrlicht.h"
#include "GameIrrlicht.h"
#include "GameTimerIrrlicht.h"
#include "Cell.h"
#include "BlockIrrlicht.h"
#include "PlayerIrrlicht.h"
#include "GameItemIrrlicht.h"
#include <iostream>

BombIrrlicht::BombIrrlicht() :
	XYElement() {
	BombItemIrrlicht* tmp = new BombItemIrrlicht(this);
	this->setView(tmp);
	tmp->setModel(this);
	this->ignitetime = 3000.0f;
	exploded = false;
	//tmp->init(0);
	bombpower = 1;
	m_xSpeed = 0;
	m_ySpeed = 0;
	playsound =false;
}

BombIrrlicht::BombIrrlicht(ArenaIrrlicht* arena, irr::u32 laytime, int bp) :
	XYElement() {
	// TODO Auto-generated constructor stub
	//this->setView(this);
	BombItemIrrlicht* tmp = new BombItemIrrlicht(this);
	this->setView(tmp);
	tmp->setModel(this);
	//std::cout << this->getI() << ", "<< this->getJ() <<" inBombIrrlicht\n";
	this->laytime = laytime;
	this->ignitetime = 3000.0f;
	m_arena = arena;
	bombpower = bp;
	tmp->init();
	exploded = false;
	bombpower = 1;
	m_xSpeed = 0;
	m_ySpeed = 0;
	playsound =false;

	//tmp->setVisible(true);
}

BombIrrlicht::~BombIrrlicht() {
	// TODO Auto-generated destructor stub
}

void BombIrrlicht::reset() {
	ignitetime = 3000.0f;
	exploded = false;
	bombpower = 3;
	m_xSpeed = 0;
	m_ySpeed = 0;
	playsound =false;
}

void BombIrrlicht::explode() {
	//remove();
	exploded = true;
	// std::cout << "BombIrrlicht explode " << laytime << ", " << ignitetime	<< ", " << GameIrrlicht::m_gametimerIrrlicht->getTime()	<< "bombid " << this << "\n";
	int thisj = m_arena->getRowFromY(this->getY());
	int thisi = m_arena->getColFromX(this->getX());
	//bombpower=7;
	lineExplosion(thisi, thisj, 1, 0, bombpower, 0);
	lineExplosion(thisi, thisj, -1, 0, bombpower, 1);
	lineExplosion(thisi, thisj, 0, 1, bombpower, 1);
	lineExplosion(thisi, thisj, 0, -1, bombpower, 1);

	playsound=true;

}

void BombIrrlicht::update() {
	//	std::cout<<"BombIrrlicht "<<laytime<<", "<<ignitetime <<", "<<GameIrrlicht::m_gametimerIrrlicht->getTime()<<"\n";

	if (laytime + ignitetime < GameIrrlicht::m_gametimerIrrlicht->getTime()
			&& !this->exploded) {
		explode();

	}
	if (!exploded) {
		BombIrrlicht::updatemove();
	}
	//arena->getCell(thisi,thisj)->removeBomb();
	return;
}

bool BombIrrlicht::getExploded() {
	return this->exploded;
}

void BombIrrlicht::setlaytime(irr::u32 lt) {
	laytime = lt;

}
PlayerIrrlicht* BombIrrlicht::getowner() {
	return owner;
}
void BombIrrlicht::setowner(PlayerIrrlicht* po) {
	owner = po;

}

void BombIrrlicht::setarena(ArenaIrrlicht* ar) {
	m_arena = ar;
}

void BombIrrlicht::toArsenal() {
	//int pos = GameIrrlicht::bombs.linear_search(this);
	//if (!pos == -1) {
	//GameIrrlicht::bombs.erase(pos);
	//}
	owner->toArsenal(this);
	reset();
}

void BombIrrlicht::setBombPower(int i) {
	bombpower = i;
}

bool BombIrrlicht::remove() {
	// clear the Cell
	int thisj = m_arena->getRowFromY(this->getY());
	int thisi = m_arena->getColFromX(this->getX());
	Cell* tmp = m_arena->getCell(thisi, thisj);
	// std::cout << "bomb throwBomb removing bomb before " << tmp->getBomb() << "\n";
	tmp->removeBomb();
	//std::cout << "bomb throwBomb removing bomb after " << tmp->getBomb() << "\n";
	/*
	 tmp = m_arena->getCell(thisi, thisj);
	 std::cout << "bomb throwBomb removing bomb before " << tmp->getBomb()
	 << "\n";

	 tmp->removeBomb();


	 std::cout << "bomb throwBomb removing bomb after " << tmp->getBomb()
	 << "\n";
	 */


	toArsenal();

	// set exploded
	getView()->setVisible(false);
	getView()->update();
	//(dynamic_cast<BombItemIrrlicht*> (getView()))->update();
	return this->exploded;
}

void BombIrrlicht::lineExplosion(int thisi, int thisj, int di, int dj,
		int bombpower, int s) {
	Cell* tmp;
	for (int i = 0 + s; i < bombpower + 1; i++) {
		tmp = m_arena->getCell(thisi + i * di, thisj + i * dj);
		// ignite all other bombs
		if (tmp->getBomb() && !tmp->getBomb()->exploded) {
			tmp->getBomb()->explode();
			//std::cout << "bombirrlicht bomb ignits bomb\n";
		}

		if (tmp->getTerrain()->isWalkable()) {
			if (tmp->getBlock()) {
				BombExplosionIrrlicht* extmp = new BombExplosionIrrlicht(
						GameIrrlicht::m_gametimerIrrlicht->getTime());
				tmp->setExplosion(extmp);
				if (i == 0) {
					extmp->bombspot = true;
				}
				break;
			}
			if (tmp->getBonus()) {
				BombExplosionIrrlicht* extmp = new BombExplosionIrrlicht(
						GameIrrlicht::m_gametimerIrrlicht->getTime());
				tmp->setExplosion(extmp);
				if (i == 0) {
					extmp->bombspot = true;
				}
				// bomb on Bonus
				if (i+s != 0)
					break;
			}
			BombExplosionIrrlicht* extmp = new BombExplosionIrrlicht(
					GameIrrlicht::m_gametimerIrrlicht->getTime());
			tmp->setExplosion(extmp);
			if (i == 0) {
				extmp->bombspot = true;
			}
		} else {
			break;
		}

	}
	return;
}

void BombIrrlicht::updatemove() {
	//std::cout << "bomb move enter " << this	<< "\n";
	int thisj = m_arena->getRowFromY(this->getY());
	int thisi = m_arena->getColFromX(this->getX());
	Cell* tmp = m_arena->getCell(thisi, thisj);

	int currentRow = m_arena->getRowFromY(m_y);
	int currentCol = m_arena->getColFromX(m_x);

	if (m_xSpeed != 0 || m_ySpeed != 0) {
		bool bOnCenter = false;
		int xDirection = 0;
		int xDeltaCenter = Cell::SIZE / 2 - (m_x - currentCol * Cell::SIZE);
		if (m_xSpeed > 0) {
			xDirection = 1;
		} else if (m_xSpeed < 0) {
			xDirection = -1;
		}
		int yDirection = 0;
		int yDeltaCenter = Cell::SIZE / 2 - (m_y - currentRow * Cell::SIZE);
		if (m_ySpeed > 0) {
			yDirection = 1;
		} else if (m_ySpeed < 0) {
			yDirection = -1;
		}

		if ((xDirection != 0 && xDeltaCenter == 0) || (yDirection != 0
				&& yDeltaCenter == 0)) {
			bOnCenter = true;
		}

		int newRow = m_arena->getRowFromY(m_y + m_ySpeed);
		int newCol = m_arena->getColFromX(m_x + m_xSpeed);
		int nextRow = m_arena->getRowFromY(m_y + m_ySpeed + yDirection
				* Cell::SIZE / 2);
		int nextCol = m_arena->getColFromX(m_x + m_xSpeed + xDirection
				* Cell::SIZE / 2);

		//at first, check if move over Cell center or currently on Cell center
		if ((bOnCenter || (xDirection * xDeltaCenter < 0 && xDirection
				* (m_xSpeed + xDeltaCenter >= 0)) || (yDirection * yDeltaCenter
				< 0 && yDirection * (m_ySpeed + yDeltaCenter >= 0)))) {
			bool bIsMortar = false;
			bool bIsNewDirection = false;
			bool bIsHurdle = false;

			if (!(m_arena->getCell(nextCol, nextRow)->isWalkable())) {
				bIsHurdle = true;
			}
			if ((m_arena->getCell(nextCol, nextRow)->getPlayers()->size() > 0)) {
				bIsHurdle = true;
			}

			//stop at Cell center if hurdle in next Cell, direction change or bomb mortar in current Cell
			if (bIsMortar || bIsNewDirection || bIsHurdle) {
				//move((currentCol + 0.5) * Cell::SIZE, (currentRow + 0.5)* Cell::SIZE);
				m_x = (currentCol + 0.5) * Cell::SIZE;
				m_y = (currentRow + 0.5) * Cell::SIZE;
				setXSpeed(0);
				setYSpeed(0);
			} else {
				if ((newRow != currentRow || newCol != currentCol)) {
					m_arena->getCell(currentCol, currentRow)->removeBomb();
					//m_arena->getCell(currentCol,currentRow)->removeBomb();
					m_arena->getCell(newCol, newRow)->setBomb(this);
					std::cout << "bomb updating Cell->bomb " << this << "\n";
				}
				//move(m_x + m_xSpeed, m_y + m_ySpeed);
				m_x = m_x + m_xSpeed;
				m_y = m_y + m_ySpeed;
			}
		} else {
			if ((newRow != currentRow || newCol != currentCol)) {
				m_arena->getCell(currentCol, currentRow)->removeBomb();
				m_arena->getCell(newCol, newRow)->setBomb(this);
				std::cout << "bomb updating Cell->bomb " << this << "\n";
			}
			//move(m_x + m_xSpeed, m_y + m_ySpeed);
			m_x = m_x + m_xSpeed;
			m_y = m_y + m_ySpeed;
		}
	}
}


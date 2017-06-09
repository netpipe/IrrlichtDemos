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

#include "FlyBombIrrlicht.h"
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
#include <iostream>

FlyBombIrrlicht::FlyBombIrrlicht() :
	BombIrrlicht() {
	landed = false;
	flybombpower = 2;
	ignitetime = 3000.0f;
	setZ(-Cell::SIZE);
	//reset();
}

FlyBombIrrlicht::FlyBombIrrlicht(ArenaIrrlicht* arena, irr::u32 laytime, int bp) :
	BombIrrlicht() {
	landed = false;
	flybombpower = 2;
	ignitetime = 3000.0f;
	setZ(-Cell::SIZE);
	//reset();

	//tmp->setVisible(true);
}

FlyBombIrrlicht::~FlyBombIrrlicht() {
	// TODO Auto-generated destructor stub
}

void FlyBombIrrlicht::update() {
	//std::cout<<"FlyBombIrrlicht "<<laytime<<", "<<ignitetime <<", "<<GameIrrlicht::m_gametimerIrrlicht->getTime()<<"\n";
	//std::cout<<"FlyBombIrrlicht landed:"<<landed<<", exploded:"<<exploded <<", "<<flybombpower<<"\n";
	if (landed == false) {
		if (flybombpower < 1) {
			land();
		}
		updatemove();
	} else {
		//std::cout << "flybomb: update -> bombirrlicht->update" << "\n";
		BombIrrlicht::update();
	}
	//std::cout << "flybomb: flybomb flybombpower" << flybombpower << "\n";
	//arena->getCell(thisi,thisj)->removeflybomb();
	return;
}

void FlyBombIrrlicht::land() {
	//std::cout << "flybomb: land" << this << "\n";
	//int thisj = m_arena->getRowFromY(this->getY());
	//int thisi = m_arena->getColFromX(this->getX());
	int thisj = m_arena->getRowFromY(m_y);
	int thisi = m_arena->getColFromX(m_x);
	Cell* tmp = m_arena->getCell(thisi, thisj);
	//std::cout << "flybomb: try land at " << thisi << ", " << thisj << "| "	<< m_x << ", " << m_y << "\n";
	if (tmp->getBlock()) {
		// std::cout << "flybomb: flybomb cant land block " << this << "\n";
		//flybombpower = 1;
		throwBomb(dirx,diry,1.5);
		return;
	}

	if (tmp->getBomb()) {
		if (tmp->getBomb() != this) {
			//std::cout << "flybomb: flybomb cant land bomb " << this << " vs "<< tmp->getBomb() << "\n";
			//flybombpower = 1;
			throwBomb(dirx,diry,1.5);
			return;
		}
	}

	if (!tmp->getTerrain()->isWalkable()) {
		// std::cout << "flybomb: flybomb cant land terrain " << this << "\n";
		throwBomb(dirx,diry,1.5);
		//flybombpower = 1;
		return;
	}
	tmp->setBomb(this);
	//std::cout << "flybomb: flybomb landing \n";
}

void FlyBombIrrlicht::reset() {
	//int pos = GameIrrlicht::flybombs.linear_search(this);
	//if (!pos == -1) {
	//GameIrrlicht::flybombs.erase(pos);
	//}
	//owner->toArsenal(this);
	exploded = false;
	m_xSpeed = 0;
	m_ySpeed = 0;
	landed = true;
	flybombpower = 2;
	ignitetime = 3000.0f;
	setZ(-Cell::SIZE);
	//setZ(0.5*-Cell::SIZE);

}

void FlyBombIrrlicht::setflybombPower(int i) {
	flybombpower = i;
}

void FlyBombIrrlicht::throwBomb(int dx, int dy, float distance) {
	// std::cout << "flybomb enter throwBomb \n";
	int thisj = m_arena->getRowFromY(this->getY());
	int thisi = m_arena->getColFromX(this->getX());
	//Cell* tmp = m_arena->getCell(thisi, thisj);


	dirx = dx;
	diry = dy;

	Cell* tmp = GameIrrlicht::arena->getCell(GameIrrlicht::arena->getColFromX(
			getX()), GameIrrlicht::arena->getRowFromY(getY()));

	//std::cout << "Bomb tfrom place at" << tmp->getI() << " " << tmp->getJ()<< "\n";

	//std::cout << "flybomb throwBomb removing bomb before " << tmp->getBomb()<< "\n";
	if (tmp->getBomb() != this) {
		//std::cout << "flybomb throwBomb removing failed\n";
	} else {
		tmp->removeBomb();
	}
	//std::cout << "flybomb throwBomb removing bomb after  " << tmp->getBomb() << "\n";
	// sign
	m_xSpeed = ((dx > 0) - (dx < 0)) * Cell::SIZE / 3;
	m_ySpeed = ((dy > 0) - (dy < 0)) * Cell::SIZE / 3;

	// computing the flyway (gui only)
	flystartx = getX();
	flystarty = getY();
	flystartz = getZ();
	flylength = (distance)*Cell::SIZE;
	flydonex=0;
	flydoney=0;
	flydonez=0;

	flybombpower = distance;
	landed = false;
	exploded = false;
	getView()->setVisible(true);
	getView()->update();
}

void FlyBombIrrlicht::move() {
	m_x = m_x + m_xSpeed;
	m_y = m_y + m_ySpeed;

	flydonex=flydonex+fabs(m_xSpeed);
	flydoney=flydoney+fabs(m_ySpeed);


	//flydonez=flydonez+m_zSpeed;
	//std::cout << "flybomb: move enter arenaCols " << m_arena->getNbColumns()<< " arenaRows" << m_arena->getNbRows() << "\n";
	if (!landed) {
		//		if (m_x > (m_arena->getNbColumns() - 0.5) * Cell::SIZE) {
		//			m_x = 0;
		//		} else {
		//			if (m_x < 0) {
		//				m_x = (m_arena->getNbColumns() - 1) * Cell::SIZE;
		//			}
		//		}
		//		if (m_y > (m_arena->getNbRows()) * Cell::SIZE) {
		//			m_y = 0;
		//		} else {
		//			if (m_y < 0) {
		//				m_y = (m_arena->getNbRows() - 1) * Cell::SIZE;
		//			}
		//		}
		m_y = mymodulo(m_y, (m_arena->getNbRows()) * Cell::SIZE);
		m_x = mymodulo(m_x, (m_arena->getNbColumns()) * Cell::SIZE);

	}
}

float FlyBombIrrlicht::mymodulo(float a, float b) {

	return a - b * floor(a / b);
	/*if (a >= b){
	 return mymodulo(a-b,b);
	 }
	 if (a < 0){
	 return mymodulo(a+b,b);
	 }
	 return a;*/
}

void FlyBombIrrlicht::updatemove() {
	//std::cout << "flybomb: updatemove enter" << "\n";
	int thisj = m_arena->getRowFromY(this->getY());
	int thisi = m_arena->getColFromX(this->getX());
	Cell* tmp = m_arena->getCell(thisi, thisj);

	int currentRow = m_arena->getRowFromY(mymodulo(m_y, (m_arena->getNbRows())
			* Cell::SIZE));
	int currentCol = m_arena->getColFromX(mymodulo(m_x,
			(m_arena->getNbColumns()) * Cell::SIZE));

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

		if (!landed) {
			newRow = m_arena->getRowFromY(mymodulo(m_y + m_ySpeed,
					(m_arena->getNbRows()) * Cell::SIZE));
			newCol = m_arena->getColFromX(mymodulo(m_x + m_xSpeed,
					(m_arena->getNbColumns()) * Cell::SIZE));
			nextRow = m_arena->getRowFromY(mymodulo(m_y + m_ySpeed + yDirection
					* Cell::SIZE / 2, (m_arena->getNbRows()) * Cell::SIZE));
			nextCol = m_arena->getColFromX(mymodulo(m_x + m_xSpeed + xDirection
					* Cell::SIZE / 2, (m_arena->getNbColumns()) * Cell::SIZE));
			/*
			 std::cout << "flybomb compute x y"
			 <<" m_x " << m_x+m_xSpeed << ", fmod(m_x,..)" <<mymodulo(m_x + m_xSpeed,	(m_arena->getNbColumns()) * Cell::SIZE)
			 <<" m_y " << m_y+m_ySpeed << ", fmod(m_y,..)" <<mymodulo(m_y + m_ySpeed,	(m_arena->getNbRows()) * Cell::SIZE)
			 << " fp " << flybombpower << "\n";
			 */

		}

		/*std::cout << "flybomb ecompute i,j "<< currentCol	<< ", " << currentRow
		 <<" new  Cell " << newCol 	<< ", " << newRow
		 << " fp " << flybombpower << "\n";*/

		//at first, check if move over Cell center or currently on Cell center
		if ((bOnCenter || (xDirection * xDeltaCenter < 0 && xDirection
				* (m_xSpeed + xDeltaCenter >= 0)) || (yDirection * yDeltaCenter
				< 0 && yDirection * (m_ySpeed + yDeltaCenter >= 0)))) {
			bool bIsMortar = false;
			bool bIsNewDirection = false;
			bool bIsHurdle = false;

			if (landed) {

				if (!(m_arena->getCell(nextCol, nextRow)->isWalkable())) {
					bIsHurdle = true;
				}
				if ((m_arena->getCell(nextCol, nextRow)->getPlayers()->size()
						> 0)) {
					bIsHurdle = true;
				}
			} else {
				if (flybombpower < 1) {
					// force a stop in Cell middle
					bIsHurdle = true;
				}
			}
			//stop at Cell center if hurdle in next Cell, direction change or flybomb mortar in current Cell
			if (bIsMortar || bIsNewDirection || bIsHurdle) {
				//move((currentCol + 0.5) * Cell::SIZE, (currentRow + 0.5)* Cell::SIZE);
				m_x = (currentCol + 0.5) * Cell::SIZE;
				m_y = (currentRow + 0.5) * Cell::SIZE;
				setXSpeed(0);
				setYSpeed(0);
				tmp->setBomb(this);
				landed = true;
				//std::cout << "flybomb landed at "<< m_x <<", "<< m_y <<" \n";
			} else {
				if ((newRow != currentRow || newCol != currentCol)) {
					flybombpower = flybombpower - 1;
					/*
					 std::cout << "flybomb enter" <<" new  Cell->bomb " << newCol
					 << ", " << newRow << " old  Cell->bomb " << currentCol
					 << ", " << currentRow << " fp " << flybombpower << "\n";
					 */
					//m_arena->removeCellElement(currentRow, currentCol, this);
					//m_arena->setCellElement(newRow, newCol, this);
					if (landed) {
						//std::cout << "flybomb updating Cell->bomb " << this	<< "\n";

						m_arena->getCell(currentCol, currentRow)->removeBomb();
						//m_arena->getCell(currentCol,currentRow)->removeflybomb();
						m_arena->getCell(newCol, newRow)->setBomb(this);
					}
				}
				//move(m_x + m_xSpeed, m_y + m_ySpeed);
				move();

			}
		} else {
			if ((newRow != currentRow || newCol != currentCol)) {
				flybombpower = flybombpower - 1;
				/*
				std::cout << "flybomb enter" << " new  Cell->bomb " << newCol
						<< ", " << newRow << " old  Cell->bomb " << currentCol
						<< ", " << currentRow << " fp " << flybombpower << "\n";
						*/
				if (landed) {
					//std::cout << "flybomb updating Cell->bomb " << this << "\n";
					m_arena->getCell(currentCol, currentRow)->removeBomb();
					//m_arena->getCell(currentCol,currentRow)->removeflybomb();
					m_arena->getCell(newCol, newRow)->setBomb(this);
				}
			}
			//move(m_x + m_xSpeed, m_y + m_ySpeed);
			move();

			//			m_x = m_x + m_xSpeed;
			//			m_y = m_y + m_ySpeed;
			//
			//			if (!landed) {
			//				if (m_x > m_arena->getNbColumns() * Cell::SIZE) {
			//					m_x = 0;
			//				}
			//				if (m_x < 0) {
			//					m_x = m_arena->getNbColumns() * Cell::SIZE;
			//				}
			//				if (m_y > m_arena->getNbRows() * Cell::SIZE) {
			//					m_y = 0;
			//				}
			//				if (m_y < 0) {
			//					m_y = m_arena->getNbRows() * Cell::SIZE;
			//				}
			//
			//			}

		}
	}
}


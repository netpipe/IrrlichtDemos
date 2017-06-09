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

#include "ArenaIrrlicht.h"
#include "ArenaItemIrrlicht.h"
#include "TerrainIrrlicht.h"
#include "WallIrrlicht.h"
#include <cmath>
#include <iostream>

ArenaIrrlicht::ArenaIrrlicht() :
	ElementIrrlicht() {
	this->setView(new ArenaItemIrrlicht(this));
	// m_playerPositions = new irr::core::array<irr::core::vector2di>:
}

ArenaIrrlicht::~ArenaIrrlicht() {
	for (int i = 0; i < m_nbRows; ++i) {
		delete[] m_Cells[i];
	}
	delete[] m_Cells;
}

void ArenaIrrlicht::init(int p_nbRows, int p_nbColumns) {
	m_nbRows = p_nbRows;
	m_nbColumns = p_nbColumns;
	m_Cells = new Cell*[m_nbRows * m_nbColumns];
	std::cout << "arenaI init rows, cols " << m_nbRows << ", " << m_nbColumns
			<< "\n";

	/*
	 *  (x,y) x=(5,1)
	 *  **********
	 *  **********
	 *  *****x****
	 *  **********
	 *  (0,0) left down corner
	 *  nbRows=3
	 *  nbCols=9
	 */

	for (int i = 0; i < m_nbRows; ++i) {
		for (int j = 0; j < m_nbColumns; ++j) {
			int index = j + i * m_nbColumns;
			m_Cells[index] = new Cell;
			m_Cells[index]->setI(j);
			m_Cells[index]->setJ(i);
			//std::cout <<"ArenaIrrlicht " << m_Cells[i][j].getTerrain()<<"\n";
			//std::cout << " " <<m_Cells[i][j].getTerrain()->getView();
			std::cout << "(i,j)" << j << "," << i << " " << m_Cells[index];
		}

	}
	std::cout << "arenaI init done\n";

}

irr::core::stringc ArenaIrrlicht::getName() {
	return m_strArenaIrrlichtName;
}

void ArenaIrrlicht::setName(irr::core::stringc &p_strArenaIrrlichtName) {
	m_strArenaIrrlichtName = p_strArenaIrrlichtName;
}

void ArenaIrrlicht::sortPlayerPosition() {
	irr::core::array<irr::core::dimension2di> newm_playerPositions;

	int min;
	// most lower left postion
	while (m_playerPositions.size() > 0) {
		min = 0;
		for (int i = 0; i < m_playerPositions.size(); i++) {
			if (m_playerPositions.operator [](i).Height
					+ m_playerPositions.operator [](i).Width
					< m_playerPositions.operator [](min).Height
							+ m_playerPositions.operator [](min).Width) {
				min = i;
			}
		}
		newm_playerPositions.push_back(m_playerPositions[min]);
		m_playerPositions.erase(min);

		if (m_playerPositions.size() == 0) {break;}

		min = 0;
		// most upper right postion
		for (int i = 0; i < m_playerPositions.size(); i++) {
			if (m_playerPositions.operator [](i).Height
					+ m_playerPositions.operator [](i).Width
					> m_playerPositions.operator [](min).Height
							+ m_playerPositions.operator [](min).Width) {
				min = i;
			}
		}

		newm_playerPositions.push_back(m_playerPositions[min]);
		m_playerPositions.erase(min);

		if (m_playerPositions.size() == 0) {break;}

		min = 0;
		// most upper left postion
		for (int i = 0; i < m_playerPositions.size(); i++) {
			if (m_nbRows - m_playerPositions.operator [](i).Height
					+ m_playerPositions.operator [](i).Width < m_nbRows
					- m_playerPositions.operator [](min).Height
					+ m_playerPositions.operator [](min).Width) {
				min = i;
			}
		}
		newm_playerPositions.push_back(m_playerPositions[min]);
		m_playerPositions.erase(min);

		if (m_playerPositions.size() == 0) {break;}

		min = 0;
		// most lower right postion
		for (int i = 0; i < m_playerPositions.size(); i++) {
			if (m_nbColumns - m_playerPositions.operator [](i).Height
					+ m_playerPositions.operator [](i).Width > m_nbRows
					- m_playerPositions.operator [](min).Height
					+ m_playerPositions.operator [](min).Width) {
				min = i;
			}
		}
		newm_playerPositions.push_back(m_playerPositions[min]);
		m_playerPositions.erase(min);
	}
	m_playerPositions = newm_playerPositions;

}

irr::core::dimension2di ArenaIrrlicht::getPlayerPosition(int p_player) {
	//    if(m_playerPosition.count() > p_player)
	//    {
	//        return m_playerPosition.at(p_player);
	//    }
	//    return m_playerPosition.at(0);    //to have a valid position
	//
	return m_playerPositions.operator [](p_player);
}

int ArenaIrrlicht::getPlayerPositionNr() {
	//    if(m_playerPosition.count() > p_player)
	//    {
	//        return m_playerPosition.at(p_player);
	//    }
	//    return m_playerPosition.at(0);    //to have a valid position
	//
	return m_playerPositions.size();
}

void ArenaIrrlicht::addPlayerPosition(int x, int y) {
	m_playerPositions.push_back(irr::core::dimension2di(x, y));
}

Cell* ArenaIrrlicht::getCell(int p_row, int p_column) {
	//a_Cell.setI(5);
	//a_Cell.setJ(5);
	//std::cout <<" a_Cell.getCost() " << a_Cell->getCost() <<"\n";
	//return a_Cell;
	if (p_row < 0 || p_row >= m_nbColumns || p_column < 0 || p_column
			>= m_nbRows) {
		std::cout << "Cell.getCell() not on map! " << p_row << ", " << p_column
				<< "\n";
		Cell* tmp = new Cell;
		tmp->init();
		tmp->setTerrain(new WallIrrlicht());
		return tmp;
	}
	int index = p_row + p_column * m_nbColumns;
	return m_Cells[index];
}

//irr::core::dimension2df ArenaIrrlicht::getCoords(Cell* p_Cell)
//{
//    for (int i = 0; i < m_nbRows; ++i)
//    {
//        for (int j = 0; j < m_nbColumns; ++j)
//        {
//        	int index = i+j*m_nbRows;
//
//            if (m_Cells[index] == p_Cell)
//            {
//                return irr::core::dimension2df(j, i);
//            }
//        }
//    }
//    return irr::core::dimension2df();
//}

irr::core::dimension2df ArenaIrrlicht::getCoords(int i, int j) {
	return irr::core::dimension2df((i + 0.5) * Cell::SIZE, (j + 0.5)
			* Cell::SIZE);

}

int ArenaIrrlicht::getRowFromY(irr::f32 p_y) {
	int nRow = (p_y / Cell::SIZE);
	if (p_y < 0) {
		nRow -= 1;
	}
	return nRow;
}

int ArenaIrrlicht::getColFromX(irr::f32 p_x) {
	int nCol = (p_x / Cell::SIZE);
	if (p_x < 0) {
		nCol -= 1;
	}
	return nCol;
}

int ArenaIrrlicht::getNbColumns() {
	return m_nbColumns;
}

int ArenaIrrlicht::getNbRows() {
	return m_nbRows;
}

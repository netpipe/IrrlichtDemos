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

#ifndef ArenaIrrlicht_H
#define ArenaIrrlicht_H


#include "Cell.h"
#include <irrlicht.h>
#include "ElementIrrlicht.h"

/**
 * @brief This class represents the ArenaIrrlicht of the game.
 */
class ArenaIrrlicht: public ElementIrrlicht
{

private:
    /** The number of rows of the ArenaIrrlicht */
    //stringc m_strArenaIrrlichtName;
	irr::core::stringc m_strArenaIrrlichtName;

    
    /** The number of rows of the ArenaIrrlicht */
    int m_nbRows;
    
    /** The number of columns of the ArenaIrrlicht */
    int m_nbColumns;
    
    /** The ArenaIrrlicht Cells */
    Cell** m_Cells;
    
    /** The ArenaIrrlicht Cells */
    Cell m_emptyCell;
    
    Cell* a_Cell;

    /** The Player position on the ArenaIrrlicht */
    irr::core::array<irr::core::dimension2di> m_playerPositions;

public:

    /**
      * Creates a new ArenaIrrlicht instance.
      */
    ArenaIrrlicht();

    /**
      * Deletes the ArenaIrrlicht instance.
      */
    ~ArenaIrrlicht();
    
    /**
      * Creates the ArenaIrrlicht matrix.
      * @param p_nbRows the number of rows
      * @param p_nbColumns the number of columns
      */
    void init( int p_nbRows,  int p_nbColumns);
    
    /**
      * Returns the Arean name.
      * @return the ArenaIrrlicht name
      */
    irr::core::stringc getName () ;
    
    /**
      * Sets the ArenaIrrlicht name
      * @param p_strArenaIrrlichtName the ArenaIrrlicht name
      */
    void setName ( irr::core::stringc &p_strArenaIrrlichtName);


    /**
      * Sets the Element that is on the Cell whose coordinates are given in parameters.
      * @param p_row the Cell row
      * @param p_column the Cell column
      * @param p_element the Element that is on the Cell
      */
    void setCellElement( int p_row,  int p_column, Element* p_element);

    /**
      * Removes the Element that is on the Cell whose coordinates are given in parameters.
      * @param p_row the Cell row
      * @param p_column the Cell column
      * @param p_element the Element that is on the Cell
      */
    void removeCellElement( int p_row,  int p_column, Element* p_element);
    
    /**
      * Sets a player position on the ArenaIrrlicht.
      * @param p_position the player position
      */
    void addPlayerPosition(int x, int y) ;
    
    /**
      * Gets the player position on the ArenaIrrlicht.
      * @param p_player the player number
      * @return p_position the player position
      */
    irr::core::dimension2di getPlayerPosition(int p_player) ;
    int getPlayerPositionNr() ;

    

    /**
      * Gets the Cell at the given coordinates.
      * @param p_row the row index
      * @param p_column the column index
      * @return the Cell at the given row and column
      */
    Cell* getCell(int p_row, int p_column);

    /**
      * Gets the coordinates of the given Cell as a QPoint.
      * @param p_Cell the searched Cell
      * @return the row and column of the given Cell
      */
    //irr::core::dimension2df getCoords(Cell* p_Cell) ;
    irr::core::dimension2df getCoords(int i,int j);

    /**
      * Gets the row index corresponding to the given y-coordinate.
      * @param p_y the y-coordinate to convert into row index
      * @return the row index corresponding to the given y-coordinate
      */
    int getRowFromY( irr::f32 p_y) ;

    /**
      * Gets the column index corresponding to the given x-coordinate.
      * @param p_x the x-coordinate to convert into column index
      * @return the column index corresponding to the given x-coordinate
      */
    int getColFromX( irr::f32 p_x) ;
    
    /**
      * Gets the number of columns of the ArenaIrrlicht.
      * @return the number of columns
      */
    int getNbColumns() ;
      
    /**
      * Gets the number of rows of the ArenaIrrlicht.
      * @return the number of rows
      */
    int getNbRows() ;

    void sortPlayerPosition();
};

#endif

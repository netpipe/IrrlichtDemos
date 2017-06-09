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

#ifndef CELL_H
#define CELL_H


#include "irrlicht.h"
#include "CellPos.h"

class Element;
class Character;
class Bomb;
class Bonus;
class BlockIrrlicht;
class Bombexplosion;
class TerrainIrrlicht;
class BombExplosionIrrlicht;
class BombIrrlicht;
class IJElementIrrlicht;
class PlayerIrrlicht;
class BonusIrrlicht;

//class CellPos;

/**
 * @brief This class represents a Cell of the Arena.
 */
class Cell: public CellPos {
public:

	/** The Cell side size */
	static irr::f32 SIZE;

	void init();


private:
	/** split Element up in bomb, Bonus, characters, Block, Terrain ,..*/

	/** A reference on the bomb that is on the Cell */
	BombIrrlicht* m_bomb;

	/** A reference on the Bonus that is on the Cell */
	BonusIrrlicht* m_Bonus;

	/** A reference on the block that is on the Cell */
	BlockIrrlicht* m_block;

	/** A reference on the characters that are on the Cell */
	irr::core::array<PlayerIrrlicht*>* m_players;

	/** A reference on the bombexplosions that are on the Cell */
	//irr::core::array<Bombexplosion*> bombexplosions;

	//	/** A reference on the Terrain that is on the Cell */
	TerrainIrrlicht* m_terrain;

	//	/** A reference on the Terrain that is on the Cell */
	BombExplosionIrrlicht* m_explosion;

public:

	/**
	 * Creates a new Cell instance.
	 */
	Cell();

	/**
	 * Deletes the Cell instance.
	 */
	~Cell();

	/**
	 * Returns if it is possible to move into the Cell or not, because of a wall, bomb, etc.
	 * @return true if it is possible to move into the Cell
	 */
	bool isWalkable();


	/* Bomb */
	/**
	 * Gets the Bomb that is on the Cell.
	 * @return the Bomb that is on the Cell
	 */
	BombIrrlicht* getBomb();

	/**
	 * Sets the Bomb that is on the Cell.
	 * @param p_bomb the Bomb to set on the Cell
	 */
	void setBomb(BombIrrlicht* p_bomb);

	/**
	 * Removes the Bomb that is on the Cell.
	 * @param p_bomb the Bomb to remove from the Cell
	 */
	void removeBomb();

	//  not needed??
	//	/* Bombexplosions */
	//	/**
	//	 * Gets the Bombexplosions that are/was on the Cell.
	//	 * @return the Bombexplosions that are on the Cell
	//	 */
	//	irr::core::array<Bombexplosion*> getBombexplosions() ;

	/* Bonus */
	/**
	 * Gets the Bonus that is on the Cell.
	 * @return the Bonus that is on the Cell
	 */
	BonusIrrlicht* getBonus();

	/**
	 * Sets the Bonus that is on the Cell.
	 * @param p_Bonus the Bonus to set on the Cell
	 */
	void setBonus(BonusIrrlicht* p_Bonus);

	/**
	 * Removes the Bonus that is on the Cell.
	 * @param p_Bonus the Bonus to remove from the Cell
	 */
	void removeBonus();

	/* Block */
	/**
	 * Gets the Block that is on the Cell.
	 * @return the Block that is on the Cell
	 */
	BlockIrrlicht* getBlock();

	/**
	 * Sets the Block that is on the Cell.
	 * @param p_block the Block to set on the Cell
	 */
	void setBlock(BlockIrrlicht* p_block);

	/**
	 * Removes the Block that is on the Cell.
	 * @param p_block the Block to remove from the Cell
	 */
	void removeBlock();

	/* Terrain */
	/**
	 * Gets the Terrain that is on the Cell.
	 * @return the Terrain that is on the Cell
	 */
	TerrainIrrlicht* getTerrain();

	/**
	 * Sets the Terrain that is on the Cell.
	 * @param p_Terrain the Terrain to set on the Cell
	 */
	void setTerrain(TerrainIrrlicht* p_Terrain);

	/**
	 * Removes the Terrain that is on the Cell.
	 * @param p_Terrain the Terrain to remove from the Cell
	 */
	void removeTerrain();


	/* Explosion */
	/**
	 * Gets the Explosion that is on the Cell.
	 * @return the Explosion that is on the Cell
	 */
	BombExplosionIrrlicht* getExplosion();

	/**
	 * Sets the Explosion that is on the Cell.
	 * @param p_Explosion the Explosion to set on the Cell
	 */
	void setExplosion(BombExplosionIrrlicht* p_Explosion);

	/**
	 * Removes the Explosion that is on the Cell.
	 * @param p_Explosion the Explosion to remove from the Cell
	 */
	void removeExplosion();




	irr::core::array<PlayerIrrlicht*>* getPlayers();

	void addPlayer(PlayerIrrlicht* p);


	void removePlayer(PlayerIrrlicht* p);

	bool isExplodable();

};

#endif


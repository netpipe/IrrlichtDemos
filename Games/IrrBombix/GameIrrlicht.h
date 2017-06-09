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

#ifndef GAMEIRRLICHT_H
#define GAMEIRRLICHT_H

#include "irrlicht.h"
#include "Serializable.h"

class PlayerIrrlicht;
class ArenaIrrlicht;
class ControllerManager;
class GameSceneIrrlicht;
class BombIrrlicht;
class GameTimerIrrlicht;
class BombExplosionIrrlicht;
class BonusIrrlicht;
class FallingBlockIrrlicht;

class GameIrrlicht
{
public:
	GameIrrlicht();

	~GameIrrlicht();


	void initGame();
	void clear();
	irr::core::array<PlayerIrrlicht*> getPlayersIrrlicht();

	void setDevice(	irr::IrrlichtDevice * device);
	irr::IrrlichtDevice* getDevice();
	void updatePlayers();

	static GameTimerIrrlicht* m_gametimerIrrlicht;
	static GameSceneIrrlicht* m_GameItemIrrlicht;

	static irr::core::array<PlayerIrrlicht*> players;
	static irr::core::array<BombIrrlicht*> bombs;
	static irr::core::array<BombExplosionIrrlicht*> bombexplosions;
	static irr::core::array<BonusIrrlicht*> Bonus;
	static irr::core::array<FallingBlockIrrlicht*> fallingblocks;
	static ArenaIrrlicht* arena;
	static GameIrrlicht* game;

	void update();
	ControllerManager* playerController;
private:

	void updateBombs();
	void updateBombExplosions();
	void updateBonus();
	void checkend();
	void updateFallingBlocks();
	int state;
	irr::IrrlichtDevice * device;



};

#endif

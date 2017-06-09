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

#include "ElementItemIrrlicht.h"
#include "TerrainIrrlicht.h"
#include "irrlicht.h"
#include "Serializable.h"
class PlayerIrrlicht;
class GameSceneIrrlicht;
//class TerrainIrrlicht;

#ifndef bombitemIRRLICHT_H_
#define bombitemIRRLICHT_H_

class BombItemIrrlicht: public ElementItemIrrlicht {
public:
	BombItemIrrlicht();
	virtual ~BombItemIrrlicht();
	BombItemIrrlicht(BombIrrlicht* p_model);
	BombItemIrrlicht(BombIrrlicht* p_model,  GameSceneIrrlicht* g_scene);

	void init();

	void update();

	void setVisible(bool yorn);

private:
	//irr::scene::IAnimatedMeshSceneNode *node;
	irr::scene::ISceneNode *node;
	irr::u32 lastInterfaceAction;
	irr::core::vector3df normalscale;
};

#endif /* bombitemIRRLICHT_H_ */


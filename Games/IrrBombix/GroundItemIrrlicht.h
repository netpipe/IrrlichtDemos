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
class GroundIrrlicht;
class GameSceneIrrlicht;
//class TerrainIrrlicht;

#ifndef GroundITEMIRRLICHT_H_
#define GroundITEMIRRLICHT_H_

class GroundItemIrrlicht: public ElementItemIrrlicht {
public:
	GroundItemIrrlicht();
	virtual ~GroundItemIrrlicht();
	GroundItemIrrlicht(GroundIrrlicht* p_model);
	GroundItemIrrlicht(GroundIrrlicht* p_model,  GameSceneIrrlicht* g_scene);

	void init();
};

#endif /* GroundITEMIRRLICHT_H_ */


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

#ifndef ARENAITEMIRRLICHT_H_
#define ARENAITEMIRRLICHT_H_
#include <irrlicht.h>
#include "ElementItemIrrlicht.h"
#include <string>

class ArenaIrrlicht;
class GameSceneIrrlicht;

class ArenaItemIrrlicht: public ElementItemIrrlicht {
public:
	ArenaItemIrrlicht(ArenaIrrlicht* p_model);
	virtual ~ArenaItemIrrlicht();
	static irr::f32 getCellSize();

	irr::core::vector3d<irr::f32> getDrawPos() const;

	void setCellSize(irr::f32 CellSize);

	void setDrawPos(irr::core::vector3d<irr::f32> drawPos);

	void setCamera(GameSceneIrrlicht* g_scene);

	void init();

	void UpdateDynamicCam();

	std::string meshname;
	// initial position
	float ipx;
	float ipy;
	float ipz;
	// initial rotation
	float irx;
	float iry;
	float irz;
	float modellfieldsize;
	// use lightning
	int lightning;
private:
	irr::core::vector3d<irr::f32> drawPos;
	static irr::f32 CellSize;
	irr::core::vector3d<irr::f32> oldcameratarget;
	irr::core::vector3d<irr::f32> oldcamerapos;
	bool now;

};

#endif /* ARENAITEMIRRLICHT_H_ */

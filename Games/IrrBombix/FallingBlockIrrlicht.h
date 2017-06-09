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

#ifndef FallingBlockIrrlicht_H_
#define FallingBlockIrrlicht_H_
#include "IJElementIrrlicht.h"
#include "TerrainIrrlicht.h"

class PlayerIrrlicht;

class FallingBlockIrrlicht: public TerrainIrrlicht{
public:
	FallingBlockIrrlicht();
	virtual ~FallingBlockIrrlicht();
	static void initAllFallingBlocksIrrlicht();
	void update();
	void destroy();
	float dietime;
	float landtime;
	int showtime;

	static bool ufoDisabled;
};

#endif /* FallingBlockIrrlicht_H_ */

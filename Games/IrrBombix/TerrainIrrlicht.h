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


#ifndef TERRAINIRRLICHT_H_
#define TERRAINIRRLICHT_H_

#include "IJElementIrrlicht.h"

class TerrainIrrlicht: public IJElementIrrlicht {
public:
	TerrainIrrlicht();
	virtual ~TerrainIrrlicht();
	virtual bool isWalkable();
};

#endif /* TERRAINIRRLICHT_H_ */

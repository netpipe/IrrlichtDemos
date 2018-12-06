/* A Gibson room, including the towers */

/*
    Copyright © 2011 John Serafino
    This file is part of The Gibson Screensaver.

    The Gibson Screensaver is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Gibson Screensaver is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with The Gibson Screensaver If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef ROOM_H_
#define ROOM_H_
#include <ray3d.h>
#include <time.h>
#include <stdlib.h>

#define TOWER_COUNT 3600
#define TOWER_XSIZE 60
#define TOWER_YSIZE 60


// number of milliseconds to wait before moving to next texture on tower text
#define TEXT_ANIM_WAIT_TIME 200
#define TOWER_TEXTURE_COUNT 8

#define TOWER_DIST  30

extern f32 then, delta;

f32 getRand(f32 low, f32 high);

class Room
{
public:

	Entity mesh;

	Entity towers[TOWER_COUNT];
	int    type[TOWER_COUNT];		// stores the type of tower, dark or light as 1 or 2 respectively
	ITexture *towerTex[TOWER_TEXTURE_COUNT * 2];

	Entity light;

	int currentTowerTexture;
	f32 timeSinceTowerUpdate;

	void init();
	void update();
};

#endif /* ROOM_H_ */

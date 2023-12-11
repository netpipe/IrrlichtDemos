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

#ifndef PULSE_H_
#define PULSE_H_
#include <ray3d.h>
#include "room.h"

#define MAX_PULSE_SPEED  200
#define MIN_PULSE_SPEED  120

#define MAX_PULSE_HEIGHT 15
#define MAX_PULSE_X		 ((TOWER_XSIZE/2) * TOWER_DIST)
#define MAX_PULSE_Z		 ((TOWER_YSIZE/2) * TOWER_DIST)

extern f32 then, delta;

class PulseSet
{
public:

	Entity *pulse;
	f32    *speed;
	int pulseCount;

	void init(int number);
	void update();
};


#endif /* PULSE_H_ */

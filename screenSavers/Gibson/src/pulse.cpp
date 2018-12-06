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

#include "pulse.h"

f32 getRand(f32 low, f32 high)
{
	return ((f32(rand()) / f32(RAND_MAX)) * (high - low)) + low;
}

void PulseSet::init(int number)
{
	pulse = new Entity[number];
	speed = new f32[number];

	pulseCount = number;

	int i;
	for(i=0; i < pulseCount; i++)
	{
		pulse[i].createMesh();
		pulse[i].loadMesh("pulse.obj", false, false);
		pulse[i].loadTex("pulse.png");
		pulse[i].setLit(false);
		pulse[i].setPosition(TOWER_DIST * int(getRand(-TOWER_XSIZE, TOWER_XSIZE)), getRand(1,MAX_PULSE_HEIGHT), \
				(TOWER_DIST * int(getRand(-TOWER_YSIZE, TOWER_YSIZE)) + TOWER_DIST/2));
		pulse[i].translateGlobal(getRand(-TOWER_DIST/4, TOWER_DIST/4),0,getRand(-TOWER_DIST/4, TOWER_DIST/4));

		pulse[i].sceneNode->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL);
		pulse[i].sceneNode->setMaterialFlag(EMF_TRILINEAR_FILTER, true);
		pulse[i].sceneNode->setMaterialFlag(EMF_ANISOTROPIC_FILTER, true);
		pulse[i].setScale(2,2,4);
		pulse[i].setRotation(0,90 * int(getRand(0,4)),0);

		speed[i] = getRand(MIN_PULSE_SPEED, MAX_PULSE_SPEED);
		//speed[i] = 0;
	}
}

void PulseSet::update()
{
	int i;
	for(i=0; i < pulseCount; i++)
	{
		pulse[i].translate(0,0,-delta * speed[i]);

		if(pulse[i].getPosition().X >= MAX_PULSE_X || pulse[i].getPosition().Z >= MAX_PULSE_Z || \
				pulse[i].getPosition().X <= -MAX_PULSE_X || pulse[i].getPosition().Z <= -MAX_PULSE_Z)
		{

			pulse[i].setPosition(TOWER_DIST * int(getRand(-TOWER_XSIZE, TOWER_XSIZE)), getRand(1,MAX_PULSE_HEIGHT), \
										(TOWER_DIST * int(getRand(-TOWER_YSIZE, TOWER_YSIZE)) + TOWER_DIST/2));
			pulse[i].translateGlobal(getRand(-TOWER_DIST/4, TOWER_DIST/4),0,getRand(-TOWER_DIST/4, TOWER_DIST/4));

			pulse[i].setRotation(0,90 * int(getRand(0,4)),0);
			speed[i] = getRand(MIN_PULSE_SPEED, MAX_PULSE_SPEED);
			//speed[i] = 0;
		}

	}
}

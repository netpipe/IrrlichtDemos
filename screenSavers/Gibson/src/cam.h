/* A flythrough camera with a predefined path */

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

#ifndef CAM_H_
#define CAM_H_
#include <ray3d.h>
#include <Effects/CRendererPostProc.h>
#include <Effects/CEffectPostProc.h>
#include <Effects/CTransitionPostProc.h>
#include <Effects/CSplitPostProc.h>
#include <Effects/CLensFlarePostProc.h>
#include <Effects/CWaterPostProc.h>
#include "room.h"

extern int CAM_MOVEMENT_SPEED;
#define INDEX_LENGTH		11
#define CAM_ROTATION_SPEED  120
#define TOWER_HEIGHT 80
#define SPEEDUP_DAMP	3
#define INDEX_PRECISION 0.2

extern f32 then, delta;

class FlyCam
{
public:

	Camera cam;

	//vector3df movement[INDEX_LENGTH];
	//vector3df rotation[INDEX_LENGTH];
	f32 velocity[INDEX_LENGTH];

	ISceneNodeAnimator *anim;
	core::array<core::vector3df> camPath;

	ISceneNode *empty;

	vector3df prevPos, prevRot;
	vector3di setRot;

	array<vector3df> camSpline;

	f32 prevVel;

	int movementIndex;

	void init();
	void update();

};

#endif /* CAM_H_ */

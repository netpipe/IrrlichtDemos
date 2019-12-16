/*
Copyright (C) 2008  Mathias Gottschlag

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in the
Software without restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the
Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "graphics/FollowingController.h"
#include "graphics/SceneNode.h"

#include <math.h>

namespace peak
{
	FollowingController::FollowingController() : SceneNodeController()
	{
		mode = (FollowingControllerMode)(EFCM_PositionApplyAll | EFCM_RotationApplyAll);
	}
	FollowingController::~FollowingController()
	{
	}
	
	void FollowingController::setTarget(SceneNode *target)
	{
		this->target = target;
	}
	SceneNode *FollowingController::getTarget(void)
	{
		return target;
	}
	
	void FollowingController::setDistance(Vector3D distance)
	{
		this->distance = distance;
	}
	Vector3D FollowingController::getDistance(void)
	{
		return distance;
	}
	void FollowingController::setRotation(Vector3D rotation)
	{
		this->rotation = rotation;
	}
	Vector3D FollowingController::getRotation(void)
	{
		return rotation;
	}
	
	void FollowingController::setMode(FollowingControllerMode mode)
	{
		this->mode = mode;
	}
	FollowingControllerMode FollowingController::getMode(void)
	{
		return mode;
	}
	
	void FollowingController::doWork(float msecs)
	{
		if (node && target)
		{
			// TODO: X/Z axis
			// Set position
			Vector3D reldistance = distance;
			Quaternion rotation = target->getRotation();
			Vector3D axis(0, 0, 1);
			axis = rotation.rotateVector(axis);
			Vector3D euler = axis.getRotation();
			if (mode & EFCM_PositionApplyY)
			{
				reldistance.rotateXZ(-euler.y);
			}
			node->setPosition(target->getPosition() + reldistance);
			// Set rotation
			Vector3D noderotation;
			if (mode & EFCM_RotationApplyY)
			{
				noderotation.y = euler.y;
			}
			node->setRotation(noderotation + this->rotation);
		}
	}
}


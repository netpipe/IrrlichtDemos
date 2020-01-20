// Copyright (C) 2002-2009 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#include "CSceneNodeAnimatorCameraTouch.h"
#include "IVideoDriver.h"
#include "ISceneManager.h"
#include "Keycodes.h"
#include "ITouchControl.h"
#include "ICameraSceneNode.h"
#include "ISceneNodeAnimatorCollisionResponse.h"

namespace irr
{
namespace scene
{

//! constructor
CSceneNodeAnimatorCameraTouch::CSceneNodeAnimatorCameraTouch
	(f32 rotateSpeed, 
	 f32 moveSpeed, 
	 f32 jumpSpeed,
	 bool noVerticalMovement, 
	 bool invertY)
		:	MaxVerticalAngle(88.0f),
			MoveSpeed(moveSpeed), 
			RotateSpeed(rotateSpeed), 
			JumpSpeed(jumpSpeed), 
			MouseYDirection(invertY ? -1.0f : 1.0f),
			LastAnimationTime(0), 
			firstUpdate(true), 
			NoVerticalMovement(noVerticalMovement),
			touchActive(false)
{
	#ifdef _DEBUG
	setDebugName("CCameraSceneNodeAnimatorTouch");
	#endif

}


//! destructor
CSceneNodeAnimatorCameraTouch::~CSceneNodeAnimatorCameraTouch()
{
	if (TouchControl)
		TouchControl->drop();
}


//! It is possible to send mouse and key events to the camera. Most cameras
//! may ignore this input, but camera scene nodes which are created for
//! example with scene::ISceneManager::addMayaCameraSceneNode or
//! scene::ISceneManager::addTouchCameraSceneNode, may want to get this input
//! for changing their position, look at target or whatever.
//
bool CSceneNodeAnimatorCameraTouch::OnEvent(const SEvent& evt)
{
	switch(evt.EventType)
	{
	case EET_KEY_INPUT_EVENT:
		for (u32 i=0; i<KeyMap.size(); ++i)
		{
			if (KeyMap[i].keycode == evt.KeyInput.Key)
			{
				TouchKeys[KeyMap[i].action] = evt.KeyInput.PressedDown;
				return true;
			}
		}
		break;

	case EET_MOUSE_INPUT_EVENT:
		if (evt.MouseInput.Event == EMIE_MOUSE_MOVED)
		{
			TouchPos = TouchControl->getRelativePosition();
			return true;
		}
		break;
	
	case EET_TOUCH_INPUT_EVENT:
		if (evt.TouchInput.Event == ETOUCH_BEGAN || evt.TouchInput.Event == ETOUCH_MOVED)
		{
			TouchPos.X = evt.TouchInput.X/320.0f;
			TouchPos.Y = evt.TouchInput.Y/480.0f;
			touchActive = true;
		}
		else
		{
			touchActive = false;
		}
		return true;
		break;
			
	default:
		break;
	}

	return false;
}

// Time to animate the node
void CSceneNodeAnimatorCameraTouch::animateNode(ISceneNode* node, u32 timeMs)
{
	if (!node || node->getType() != ESNT_CAMERA)
		return;

	ICameraSceneNode* camera = static_cast<ICameraSceneNode*>(node);

	if (firstUpdate)
	{
		camera->updateAbsolutePosition();
		
		if (TouchControl && camera)
		{
			// "Zeroing" the Touch so we know what the center is
			//TouchControl->setPosition(0.5f, 0.5f);
			TouchPos = CenterTouch = core::position2d<f32>(0,0); //  TouchControl->getRelativePosition();
		}

		LastAnimationTime = timeMs;

		firstUpdate = false;
	}

	// If the camera isn't the active camera, and receiving input, then don't process it.
	if(!camera->isInputReceiverEnabled())
		return;

	scene::ISceneManager * smgr = camera->getSceneManager();
	
	if(smgr && smgr->getActiveCamera() != camera)
		return;

	// get time
	f32 timeDiff = (f32) ( timeMs - LastAnimationTime );
	LastAnimationTime = timeMs;

	// update position
	core::vector3df pos = camera->getPosition();

	// Update rotation
	core::vector3df target = (camera->getTarget() - camera->getAbsolutePosition());
	core::vector3df relativeRotation = target.getHorizontalAngle();

	if (touchActive)
	{		
		if (TouchPos != CenterTouch)
		{
			relativeRotation.Y -= (0.5f - TouchPos.X) * RotateSpeed;
			relativeRotation.X -= (0.5f - TouchPos.Y) * RotateSpeed * MouseYDirection;

			// X < MaxVerticalAngle or X > 360-MaxVerticalAngle

			if (relativeRotation.X > MaxVerticalAngle*2 &&
				relativeRotation.X < 360.0f-MaxVerticalAngle)
			{
				relativeRotation.X = 360.0f-MaxVerticalAngle;
			}
			else
			if (relativeRotation.X > MaxVerticalAngle &&
				relativeRotation.X < 360.0f-MaxVerticalAngle)
			{
				relativeRotation.X = MaxVerticalAngle;
			}
		}
	}

	// set target

	target.set(0,0, core::max_(1.f, pos.getLength()));
	core::vector3df movedir = target;

	core::matrix4 mat;
	mat.setRotationDegrees(core::vector3df(relativeRotation.X, relativeRotation.Y, 0));
	mat.transformVect(target);

	if (NoVerticalMovement)
	{
		mat.setRotationDegrees(core::vector3df(0, relativeRotation.Y, 0));
		mat.transformVect(movedir);
	}
	else
	{
		movedir = target;
	}

	movedir.normalize();

	// Move forward when touch is active
	if (touchActive) // TouchKeys[EKA_MOVE_FORWARD])
		pos += movedir * timeDiff * MoveSpeed;

	if (TouchKeys[EKA_MOVE_BACKWARD])
		pos -= movedir * timeDiff * MoveSpeed;

	// strafing

	core::vector3df strafevect = target;
	strafevect = strafevect.crossProduct(camera->getUpVector());

	if (NoVerticalMovement)
		strafevect.Y = 0.0f;

	strafevect.normalize();

	if (TouchKeys[EKA_STRAFE_LEFT])
		pos += strafevect * timeDiff * MoveSpeed;

	if (TouchKeys[EKA_STRAFE_RIGHT])
		pos -= strafevect * timeDiff * MoveSpeed;

	// For jumping, we find the collision response animator attached to our camera
	// and if it's not falling, we tell it to jump.
	if (TouchKeys[EKA_JUMP_UP])
	{
		const core::list<ISceneNodeAnimator*> & animators = camera->getAnimators();
		core::list<ISceneNodeAnimator*>::ConstIterator it = animators.begin();
		while(it != animators.end())
		{
			if(ESNAT_COLLISION_RESPONSE == (*it)->getType())
			{
				ISceneNodeAnimatorCollisionResponse * collisionResponse = 
					static_cast<ISceneNodeAnimatorCollisionResponse *>(*it);

				if(!collisionResponse->isFalling())
					collisionResponse->jump(JumpSpeed);
			}

			it++;
		}
	}

	// write translation
	camera->setPosition(pos);

	// write right target
	target += pos;
	camera->setTarget(target);
}


//! Sets the rotation speed
void CSceneNodeAnimatorCameraTouch::setRotateSpeed(f32 speed)
{
	RotateSpeed = speed;
}


//! Sets the movement speed
void CSceneNodeAnimatorCameraTouch::setMoveSpeed(f32 speed)
{
	MoveSpeed = speed;
}


//! Gets the rotation speed
f32 CSceneNodeAnimatorCameraTouch::getRotateSpeed() const
{
	return RotateSpeed;
}


// Gets the movement speed
f32 CSceneNodeAnimatorCameraTouch::getMoveSpeed() const
{
	return MoveSpeed;
}


//! Sets whether vertical movement should be allowed.
void CSceneNodeAnimatorCameraTouch::setVerticalMovement(bool allow)
{
	NoVerticalMovement = !allow;
}

//! Sets whether the Y axis of the mouse should be inverted.
/*
void CSceneNodeAnimatorCameraTouch::setInvertMouse(bool invert)
{
	if (invert)
		MouseYDirection = -1.0f;
	else
		MouseYDirection = 1.0f;
}
*/

ISceneNodeAnimator* CSceneNodeAnimatorCameraTouch::createClone(ISceneNode* node, ISceneManager* newManager)
{
	CSceneNodeAnimatorCameraTouch * newAnimator = new CSceneNodeAnimatorCameraTouch(RotateSpeed, MoveSpeed, JumpSpeed, NoVerticalMovement);
	return newAnimator;
}

} // namespace scene
} // namespace irr


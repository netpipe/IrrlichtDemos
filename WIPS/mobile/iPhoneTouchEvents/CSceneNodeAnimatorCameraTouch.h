// Copyright (C) 2002-2009 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __C_SCENE_NODE_ANIMATOR_CAMERA_TOUCH_H_INCLUDED__
#define __C_SCENE_NODE_ANIMATOR_CAMERA_TOUCH_H_INCLUDED__

#include "ISceneNodeAnimatorCameraTouch.h"
#include "vector2d.h"
#include "SKeyMap.h"
#include "irrArray.h"

namespace irr
{
namespace gui
{
	class ITouchControl;
}

namespace scene
{

	//! Special scene node animator for Touch cameras
	class CSceneNodeAnimatorCameraTouch : public ISceneNodeAnimatorCameraTouch 
	{
	public:

		//! Constructor
		CSceneNodeAnimatorCameraTouch(f32 rotateSpeed = 20.0f, 
									  f32 moveSpeed = .2f, 
									  f32 jumpSpeed=0.f,
									  bool noVerticalMovement=false, 
									  bool invertY=false);
			
		//! Destructor
		virtual ~CSceneNodeAnimatorCameraTouch();

		//! Animates the scene node, currently only works on cameras
		virtual void animateNode(ISceneNode* node, u32 timeMs);

		//! Event receiver
		virtual bool OnEvent(const SEvent& event);

		//! Returns the speed of movement in units per second
		virtual f32 getMoveSpeed() const;

		//! Sets the speed of movement in units per second
		virtual void setMoveSpeed(f32 moveSpeed);

		//! Returns the rotation speed
		virtual f32 getRotateSpeed() const;

		//! Set the rotation speed
		virtual void setRotateSpeed(f32 rotateSpeed);

		//! Sets the keyboard mapping for this animator
		//! \param keymap: an array of keyboard mappings, see SKeyMap
		//! \param count: the size of the keyboard map array
		//virtual void setKeyMap(SKeyMap *map, u32 count);

		//! Sets whether vertical movement should be allowed.
		virtual void setVerticalMovement(bool allow);

		//! Sets whether the Y axis of the mouse should be inverted.
		/** If enabled then moving the mouse down will cause
		the camera to look up. It is disabled by default. */
		//virtual void setInvertMouse(bool invert);

		//! This animator will receive events when attached to the active camera
		virtual bool isEventReceiverEnabled() const
		{
			return true;
		}

		//! Returns the type of this animator
		virtual ESCENE_NODE_ANIMATOR_TYPE getType() const
		{
			return (ESCENE_NODE_ANIMATOR_TYPE) 11;
		}

		//! Creates a clone of this animator.
		/** Please note that you will have to drop
		(IReferenceCounted::drop()) the returned pointer once you're
		done with it. */
		virtual ISceneNodeAnimator* createClone(ISceneNode* node, ISceneManager* newManager=0);

		struct SCamKeyMap
		{
			SCamKeyMap() {};
			SCamKeyMap(s32 a, EKEY_CODE k) : action(a), keycode(k) {}

			s32 action;
			EKEY_CODE keycode;
		};

		//! Sets the keyboard mapping for this animator
		/** Helper function for the clone method.
		\param keymap the new keymap array */
		void setKeyMap(const core::array<SCamKeyMap>& keymap);

	private:

		gui::ITouchControl *TouchControl;
		bool touchActive;

		f32 MaxVerticalAngle;

		f32 MoveSpeed;
		f32 RotateSpeed;
		f32 JumpSpeed;
		// -1.0f for inverted mouse, defaults to 1.0f
		f32 MouseYDirection;

		s32 LastAnimationTime;

		core::array<SCamKeyMap> KeyMap;
		core::position2d<f32> CenterTouch, TouchPos;

		bool TouchKeys[6];

		bool firstUpdate;
		bool NoVerticalMovement;
	};

} // end namespace scene
} // end namespace irr

#endif // __C_SCENE_NODE_ANIMATOR_CAMERA_TOUCH_H_INCLUDED__


// This file is a modified version of the original CSceneNodeAnimatorCameraFPS from Irrlicht

#ifndef __CIRRBP_SCENE_NODE_ANIMATOR_CAMERA_FPS_H_INCLUDED__
#define __CIRRBP_SCENE_NODE_ANIMATOR_CAMERA_FPS_H_INCLUDED__

#include "ISceneNodeAnimatorCameraFPS.h"
#include "vector2d.h"
#include "position2d.h"
#include "SKeyMap.h"
#include "irrArray.h"

#include "ICursorControl.h"
#include "body/CIrrBPRigidBody.h"



//! Special scene node animator for FPS cameras
class CSceneNodeAnimatorCameraFPS : public irr::scene::ISceneNodeAnimatorCameraFPS
{
public:

	//! Constructor
	CSceneNodeAnimatorCameraFPS(CIrrBPRigidBody * cambody,irr::gui::ICursorControl* cursorControl,
		irr::f32 rotateSpeed = 100.0f, irr::f32 moveSpeed = .5f, irr::f32 jumpSpeed=0.f,
		irr::SKeyMap* keyMapArray=0, irr::u32 keyMapSize=0, bool noVerticalMovement=false,
		bool invertY=false);

	//! Destructor
	virtual ~CSceneNodeAnimatorCameraFPS();

	//! Animates the scene node, currently only works on cameras
	virtual void animateNode(irr::scene::ISceneNode* node, irr::u32 timeMs);

	//! Event receiver
	virtual bool OnEvent(const irr::SEvent& event);

	//! Returns the speed of movement in units per second
	virtual irr::f32 getMoveSpeed() const;

	//! Sets the speed of movement in units per second
	virtual void setMoveSpeed(irr::f32 moveSpeed);

	//! Returns the rotation speed
	virtual irr::f32 getRotateSpeed() const;

	virtual irr::f32 getRotateSpeedKeyboard() const;
			virtual void setRotateSpeedKeyboard(irr::f32 rotateSpeed) {};

virtual void setKeyMap(const irr::core::array<irr::SKeyMap>& keymap);

virtual const irr::core::array<irr::SKeyMap>& getKeyMap() const;

	//! Set the rotation speed
	virtual void setRotateSpeed(irr::f32 rotateSpeed);

	//! Sets the keyboard mapping for this animator
	//! \param keymap: an array of keyboard mappings, see SKeyMap
	//! \param count: the size of the keyboard map array
	virtual void setKeyMap(irr::SKeyMap *map, irr::u32 count);

	//! Sets whether vertical movement should be allowed.
	virtual void setVerticalMovement(bool allow);

	//! Sets whether the Y axis of the mouse should be inverted.
	/** If enabled then moving the mouse down will cause
	the camera to look up. It is disabled by default. */
	virtual void setInvertMouse(bool invert);

	//! This animator will receive events when attached to the active camera
	virtual bool isEventReceiverEnabled() const
	{
		return true;
	}

	//! Returns the type of this animator
	virtual irr::scene::ESCENE_NODE_ANIMATOR_TYPE getType() const
	{
		return irr::scene::ESNAT_CAMERA_FPS;
	}

	//! Creates a clone of this animator.
	/** Please note that you will have to drop
	(IReferenceCounted::drop()) the returned pointer once you're
	done with it. */
	virtual irr::scene::ISceneNodeAnimator* createClone(irr::scene::ISceneNode* node, irr::scene::ISceneManager* newManager=0);

	struct SCamKeyMap
	{
		SCamKeyMap() {};
		SCamKeyMap(irr::s32 a, irr::EKEY_CODE k) : action(a), keycode(k) {}

		irr::s32 action;
		irr::EKEY_CODE keycode;
	};

	//! Sets the keyboard mapping for this animator
	/** Helper function for the clone method.
	\param keymap the new keymap array */
	void setKeyMap(const irr::core::array<SCamKeyMap>& keymap);

private:
	void allKeysUp();

	irr::gui::ICursorControl *CursorControl;

	irr::f32 MaxVerticalAngle;

	irr::f32 MoveSpeed;
	irr::f32 RotateSpeed;
	irr::f32 JumpSpeed;
	// -1.0f for inverted mouse, defaults to 1.0f
	irr::f32 MouseYDirection;

	irr::s32 LastAnimationTime;

	irr::core::array<SCamKeyMap> KeyMap;
	irr::core::position2d<irr::f32> CenterCursor, CursorPos;

	bool CursorKeys[6];

	bool firstUpdate;
	bool NoVerticalMovement;
	CIrrBPRigidBody * body;
};

#endif


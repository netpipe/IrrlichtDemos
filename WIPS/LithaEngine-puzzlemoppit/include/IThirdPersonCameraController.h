
#ifndef I_THIRD_PERSON_CAMERA_CONTROLLER_H
#define I_THIRD_PERSON_CAMERA_CONTROLLER_H

#include "ICameraController.h"

class ITransformable;
class IBody;
class ICameraCollider;
class IThirdPersonCameraCollider;
class ICharacter;

class IThirdPersonCameraController : public ICameraController
{
public:
	virtual ~IThirdPersonCameraController(){}
	
	// Follow a particular object
	// The object in question must then not be deleted all the while this camera controller exists.
	virtual void Follow(ITransformable *toFollow) = 0;
	
	// Special version of Follow that makes following a character easier.
	// Sets the given collider, and automatically adds the character's body to the collider
	// to be excluded from collisions.
	virtual void Follow(ICharacter *toFollow, ICameraCollider *collider) = 0;
	
	// Yaw, Pitch, Zoom
	// Set an axis to -1 to disable.
	virtual void SetInputAxes(s32 idAxisYaw, s32 idAxisPitch, s32 idAxisZoom = -1) = 0;
	
	// Speed at which the camera turns.
	virtual void SetTurnSpeed(f32 speed) = 0;
	
	// Speed at which the look angle changes to new angle.
	// Default is 0, disabled. (instant change, no smoothing)
	virtual void SetTurnSmoothing(f32 smooth) = 0;
	
	// min,max pitch in degrees.
	// default -60,+60
	virtual void SetPitchLimits(f32 min, f32 max) = 0;
	
	// Set the minimum and maximum distance for zoom.
	// By default these are both set to the internal distance, so zoom is disabled.
	virtual void SetZoomLimits(f32 min, f32 max) = 0;
	
	// Defaults to 1.0
	virtual void SetZoomSpeed(f32 speed) = 0;
	
	// Set the speed at which the zoom changes to the new zoom.
	// 0 to disable, for instant change and no smoothing.
	// Default is 0, disabled.
	virtual void SetZoomSmoothness(f32 smooth) = 0;
	
	// Set the camera angles (X,Y as pitch,yaw).
	// Must be in the range 0 to 360.
	// Useful to initialise the camera at a specific angle.
	virtual void SetAngles(const core::vector2df &angles) = 0;
	virtual core::vector2df GetAngles() = 0;
	
	virtual void SetZoom(f32 zoom) = 0;
	virtual f32 GetZoom() = 0;
	
	// Add some functionality for dealing with camera collisions with the environment.
	virtual void SetCollider(ICameraCollider *collider) = 0;
	
	// Creates a simple built in camera collider. This collider sets the actual camera position
	// to the first collision point between the camera target and the desired camera position.
	// So the camera will never go behind or through an object.
	// Useful for third person cameras.
	virtual IThirdPersonCameraCollider *CreateThirdPersonCameraCollider() = 0;
};

#endif

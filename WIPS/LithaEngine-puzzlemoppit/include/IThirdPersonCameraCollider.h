
#ifndef I_THIRD_PERSON_CAMERA_COLLIDER_H
#define I_THIRD_PERSON_CAMERA_COLLIDER_H

#include "ICameraCollider.h"

class IThirdPersonCameraCollider : public ICameraCollider
{
public:
	virtual ~IThirdPersonCameraCollider(){}
	
	// Set the distance the camera is set away from the surface it touches.
	// Default 0.5
	virtual void SetCorrectionDistance(f32 distance) = 0;
};

#endif

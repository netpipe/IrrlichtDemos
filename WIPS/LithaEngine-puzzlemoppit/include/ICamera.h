
#ifndef I_CAMERA_H
#define I_CAMERA_H

#include "IGraphic.h"

/*
Camera can have position, rotation etc set. Camera looks in direction of rotation.
The camera rotation can also be set in a conventional manner using SetTarget.
Optionally, camera can be defined to interpolate smoothly around the target with methods
SetOrbitTarget and SetTargetDistance.
*/
class ICamera : public IGraphic
{
public:
	virtual ~ICamera(){}
	
	// Rotate the camera to face a point.
	virtual void SetTarget(const core::vector3df &target) = 0;
	
	// Set the target AND define interpolation to occur around this target.
	// (like SetTargetDistance)
	virtual void SetOrbitTarget(const core::vector3df &target) = 0;
	
	// Set the distance to the target. (in the direction of camera rotation)
	// If this is other than zero, rotation occurs around this point during interpolation.
	// NOTE: Currently distance itself is not interpolated, so zooming will not be smooth.
	virtual void SetTargetDistance(f32 distance) = 0;
	virtual f32 GetTargetDistance() = 0;
	
	// Wrapped Irrlicht Camera methods.
	virtual f32 GetFOV() = 0;
	virtual f32 GetAspectRatio() = 0;
};

#endif


#include "ICamera.h"

class Camera : public ICamera
{
	scene::ICameraSceneNode *irrCamera;
	
	f32 targetDistance;
	
	// Target distance wants interpolating.
	// (for smooth zooming)
	f32 cachedTargetDistance;
	
public:
	Camera(scene::ICameraSceneNode *irrCamera);
	
	void SetTarget(const core::vector3df &target);
	void SetOrbitTarget(const core::vector3df &target);
	
	void SetTargetDistance(f32 distance);
	f32 GetTargetDistance();
	
	f32 GetFOV()			{ return irrCamera->getFOV(); }
	f32 GetAspectRatio()	{ return irrCamera->getAspectRatio(); }
	
	void CacheInterpolatableState();
	
	// Camera has custom position interpolation code.
	// (regular method does not ensure camera will remain looking at target)
	core::vector3df GetInterpolatedPosition(f32 alpha);
	
	void ReceiveRenderPosition(core::vector3df pos);
	void ReceiveRenderRotation(core::vector3df rot);
};





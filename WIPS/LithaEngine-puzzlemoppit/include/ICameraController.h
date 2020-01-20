
#ifndef I_CAMERA_CONTROLLER_H
#define I_CAMERA_CONTROLLER_H

#include "litha_internal.h"

class ITransformable;
class ICamera;

class ICameraController : public virtual IReferenceCounted
{
public:
	virtual ~ICameraController(){}
	
	virtual void Update(ICamera *camera, f32 dt) = 0;
};

#endif

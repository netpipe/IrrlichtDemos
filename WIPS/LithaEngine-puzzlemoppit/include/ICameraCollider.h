
#ifndef I_CAMERA_COLLIDER_H
#define I_CAMERA_COLLIDER_H

#include "litha_internal.h"

class ICollisionGeometry;

// Used to provide camera collision functionality to a camera controller.
class ICameraCollider : public virtual IReferenceCounted
{
public:
	~ICameraCollider(){}
	
	virtual void SetCollisionLayer(u32 layer) = 0;
	
	// Add some geometry to be excluded from collisions. Can be called multiple times.
	// e.g. might exclude a character's geometry.
	virtual void ExcludeGeometry(const Set<ICollisionGeometry *> &excluding) = 0;
	
	// Add a single geometry for exclusion.
	void ExcludeGeometry(ICollisionGeometry *excluding)
	{
		Set<ICollisionGeometry *> geometries;
		
		if (excluding)
			geometries.Insert(excluding);
		
		ExcludeGeometry(geometries);
	}
	
	// No longer exclude any geometry.
	virtual void ClearExcludedGeometry() = 0;
	
	// Called every step, performs processing on the collision.
	// cameraRay - represents the camera. Starts at the camera target and ends at the camera position.
	// resultPos - ProcessCollision should store the modified camera position here.
	// Returns true if a collision occurred and resultPos has been modified.
	// resultPos should NOT be modified if no collision occurs.
	virtual bool ProcessCollision(const core::line3df &cameraRay, core::vector3df &resultPos) = 0;
};

#endif

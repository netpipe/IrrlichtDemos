
#ifndef STATIC_BODY_H
#define STATIC_BODY_H

#include "IStaticBody.h"
#include <ode/ode.h>
#include <vector>

class ICollisionGeometry;

class StaticBody : public IStaticBody
{
	std::vector<ICollisionGeometry *> geometry;
	std::vector<dGeomID> odeGeoms;
	
public:
	StaticBody();
	~StaticBody();
	
	void AddCollisionGeometry(ICollisionGeometry *geom);
	Set<ICollisionGeometry *> GetCollisionGeometries();
	
	void SetPosition(const core::vector3df &pos);
	void SetRotation(const core::vector3df &rot);
	
	// Get Pos/Rot handled by ITransformable default implementation.
};

#endif

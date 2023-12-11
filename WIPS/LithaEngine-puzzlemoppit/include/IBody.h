
#ifndef I_BODY_H
#define I_BODY_H

#include "ITransformable.h"

class ICollisionGeometry;

class IBody : public ITransformable
{
public:
	virtual ~IBody(){}
	
	virtual void AddCollisionGeometry(ICollisionGeometry *geom) = 0;
	
	virtual Set<ICollisionGeometry *> GetCollisionGeometries() = 0;
};

#endif

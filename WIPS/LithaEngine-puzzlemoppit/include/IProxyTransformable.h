
#ifndef I_PROXY_TRANSFORMABLE_H
#define I_PROXY_TRANSFORMABLE_H

#include "ITransformable.h"

// Wraps all ITransformable methods.
// Removes the other ITransformable when removed.
class IProxyTransformable : public ITransformable
{
public:
	virtual ~IProxyTransformable(){}
	
	virtual ITransformable *GetProxiedTransformable() = 0;
	
	virtual void SetPositionOffset(const core::vector3df &offset) = 0;
};

#endif


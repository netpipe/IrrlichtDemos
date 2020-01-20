
#include "IProxyTransformable.h"

class IWorld;

class ProxyTransformable : public IProxyTransformable
{
	IWorld *world;
	ITransformable *other;
	
	core::vector3df positionOffset;
	
public:
	ProxyTransformable(IWorld *world, ITransformable *other);
	~ProxyTransformable();
	
	ITransformable *GetProxiedTransformable();
	
	void SetPositionOffset(const core::vector3df &offset);
	
	
	// wrapped methods
	
	void SetPosition(const core::vector3df &pos);
	void SetRotation(const core::vector3df &rot);
	
	core::vector3df GetPosition();
	core::vector3df GetRotation();
};


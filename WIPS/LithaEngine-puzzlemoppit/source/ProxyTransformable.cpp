
#include "ProxyTransformable.h"
#include "IWorld.h"

ProxyTransformable::ProxyTransformable(IWorld *world, ITransformable *other)
{
	this->world = world;
	this->other = other;
}

ProxyTransformable::~ProxyTransformable()
{
	world->RemoveTransformable(other);
}

ITransformable *ProxyTransformable::GetProxiedTransformable()
{
	return other;
}

void ProxyTransformable::SetPositionOffset(const core::vector3df &offset)
{
	positionOffset = offset;
}

// WRAPPED METHODS

void ProxyTransformable::SetPosition(const core::vector3df &pos)
{
	other->SetPosition(pos + positionOffset);
}

void ProxyTransformable::SetRotation(const core::vector3df &rot)
{
	other->SetRotation(rot);
}

core::vector3df ProxyTransformable::GetPosition()
{
	return other->GetPosition() - positionOffset;
}

core::vector3df ProxyTransformable::GetRotation()
{
	return other->GetRotation();
}


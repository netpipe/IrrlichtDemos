
#include "Camera.h"

Camera::Camera(scene::ICameraSceneNode *irrCamera)
{
	this->irrCamera = irrCamera;
	targetDistance = 0.0;
	cachedTargetDistance = 0.0;
}

void Camera::SetTarget(const core::vector3df &target)
{
	SetRotation( (target - GetAbsolutePosition()).getHorizontalAngle() );
}

void Camera::SetOrbitTarget(const core::vector3df &target)
{
	SetTarget(target);
	SetTargetDistance( (target - GetAbsolutePosition()).getLength() );
}

void Camera::SetTargetDistance(f32 distance)
{
	targetDistance = distance;
}

f32 Camera::GetTargetDistance()
{
	return targetDistance;
}

void Camera::CacheInterpolatableState()
{
	ITransformable::CacheInterpolatableState();
	
	cachedTargetDistance = targetDistance;
}

core::vector3df Camera::GetInterpolatedPosition(f32 alpha)
{
	// Let's interpolate targetDistance
	// I'm not entirely sure if this is correct. (untested!)
	f32 interpolatedTargetDistance = cachedTargetDistance + (targetDistance-cachedTargetDistance) * alpha;
	
	// Now rotate about target point.
	// This is in polar coordinates or something like that.
	
	// I drawed a diagram !!!1
	// but I eated it :(
	
	core::vector3df a = GetCachedPosition();
	core::vector3df c = GetPosition();
	
	core::quaternion quat;
	
	core::vector3df M(0,0,interpolatedTargetDistance);
	core::vector3df N(0,0,interpolatedTargetDistance);
	core::vector3df O(0,0,interpolatedTargetDistance);
	
	quat.set( core::DEGTORAD * GetCachedRotation() );
	M = quat * M;
	
	quat.set( core::DEGTORAD * GetInterpolatedRotation(alpha) );
	N = quat * N;
	
	quat.set( core::DEGTORAD * GetRotation() );
	O = quat * O;
	
	core::vector3df d = a + M;
	core::vector3df f = c + O;
	core::vector3df e = maths::interpolate_position( d, f, alpha );
	
	core::vector3df b = e - N;
	
	return b;
}

void Camera::ReceiveRenderPosition(core::vector3df pos)
{
	irrCamera->setPosition(pos);
}

void Camera::ReceiveRenderRotation(core::vector3df rot)
{
	// Camera scene nodes use absolute position for calculating target.
	// (done in ICameraSceneNode::setRotation when target and rotation are bound)
	irrCamera->updateAbsolutePosition();
	
	irrCamera->setRotation(rot);
}


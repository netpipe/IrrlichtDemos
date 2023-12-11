
#include "ThirdPersonCameraController.h"
#include "IWorld.h"
#include "ICamera.h"
#include "ITransformable.h"
#include "IPhysics.h"
#include "ICharacter.h"
#include "IDynamicBody.h"
#include "IThirdPersonCameraCollider.h"

// A simple implementation of a camera collider.
class ThirdPersonCollider : public IThirdPersonCameraCollider
{
	IPhysics *physics;
	u32 collisionLayer;
	Set<ICollisionGeometry *> excludedGeometries;
	
	f32 correctionDistance;
	
public:
	ThirdPersonCollider(IPhysics *physics)
	{
		this->physics = physics;
		collisionLayer = 0;
		correctionDistance = 0.5;
	}
	
	void SetCorrectionDistance(f32 distance)
	{
		correctionDistance = distance;
	}
	
	void SetCollisionLayer(u32 layer)
	{
		collisionLayer = layer;
	}
	
	void ExcludeGeometry(const Set<ICollisionGeometry *> &excluding)
	{
		excludedGeometries.Union(excluding);
	}
	
	void ClearExcludedGeometry()
	{
		excludedGeometries.clear();
	}
	
	bool ProcessCollision(const core::line3df &cameraRay, core::vector3df &resultPos)
	{
		RayCollision collisionResult;
		
		if (physics->RayCastExcluding( cameraRay, excludedGeometries, &collisionResult, collisionLayer ))
		{
			core::vector3df rayVec = cameraRay.getVector(); rayVec.normalize();
			collisionResult.collision.pos -= rayVec*correctionDistance;
			resultPos = collisionResult.collision.pos;
			return true;
		}
		
		return false;
	}
};

ThirdPersonCameraController::ThirdPersonCameraController(IWorld *world, scene::ISceneManager *smgr,
		f32 distance, f32 heightOffset, f32 minPitch, f32 maxPitch, f32 turnSpeed)
{
	this->world = world;
	
	this->heightOffset = heightOffset;
	this->minPitch = minPitch;
	this->maxPitch = maxPitch;
	
	following = NULL;
	
	world->SubscribeToInput(this);
	SetInputAxes(-1,-1,-1);
	
	SetTurnSpeed(turnSpeed);
	SetTurnSmoothing(0.0);
	
	// zoom is disabled by default
	SetZoomLimits(distance,distance);
	SetZoomSpeed(1.0);
	SetZoomSmoothness(0.0);
	
	// Initial distance (aka zoom...)
	nextDistance = distance;
	currentDistance = nextDistance;
	
	// Default camera angle
	SetAngles(core::vector2df(0,0));
	
	// Collisions?
	collider = NULL;
}

ThirdPersonCameraController::~ThirdPersonCameraController()
{
	if (collider)
		collider->drop();
	
	world->UnsubscribeFromInput(this);
}

void ThirdPersonCameraController::OnAxisChange(s32 id, f32 delta)
{
	if (id == idAxisPitch || id == idAxisYaw)
	{
		delta *= turnSpeed;
		
		// change angle...
		if (id == idAxisPitch)
			nextAngle.X += delta;
		else if (id == idAxisYaw)
			nextAngle.Y += delta;
		
		if (nextAngle.X > maxPitch)
			nextAngle.X = maxPitch;
		
		if (nextAngle.X < minPitch)
			nextAngle.X = minPitch;
	}
	// Zoom?
	else if (id == idAxisZoom)
	{
		nextDistance -= delta * zoomSpeed;
		
		if (nextDistance < minZoom)
			nextDistance = minZoom;
		else if (nextDistance > maxZoom)
			nextDistance = maxZoom;
	}
}

void ThirdPersonCameraController::Follow(ITransformable *toFollow)
{
	following = toFollow;
}

void ThirdPersonCameraController::Follow(ICharacter *toFollow, ICameraCollider *collider)
{
	following = toFollow;
	SetCollider(collider);
	
	// Exclude the character's geometry from collisions
	if (following && collider)
		collider->ExcludeGeometry( toFollow->GetBody()->GetCollisionGeometries() );
}

void ThirdPersonCameraController::SetInputAxes(s32 idAxisYaw, s32 idAxisPitch, s32 idAxisZoom)
{
	this->idAxisYaw = idAxisYaw;
	this->idAxisPitch = idAxisPitch;
	this->idAxisZoom = idAxisZoom;
}

void ThirdPersonCameraController::SetTurnSpeed(f32 speed)
{
	turnSpeed = speed;
}

void ThirdPersonCameraController::SetTurnSmoothing(f32 smooth)
{
	turnSmooth = smooth;
}

void ThirdPersonCameraController::SetPitchLimits(f32 min, f32 max)
{
	minPitch = min;
	maxPitch = max;
	
	if (nextAngle.X > maxPitch)
		nextAngle.X = maxPitch;
	
	if (nextAngle.X < minPitch)
		nextAngle.X = minPitch;
	
	if (currentAngle.X > maxPitch)
		currentAngle.X = maxPitch;
	
	if (currentAngle.X < minPitch)
		currentAngle.X = minPitch;
}

void ThirdPersonCameraController::SetZoomLimits(f32 min, f32 max)
{
	minZoom = min;
	maxZoom = max;
	
	if (currentDistance < min)
		currentDistance = min;
	else if (currentDistance > max)
		currentDistance = max;
	
	if (nextDistance < min)
		nextDistance = min;
	else if (nextDistance > max)
		nextDistance = max;
}

void ThirdPersonCameraController::SetZoomSpeed(f32 speed)
{
	zoomSpeed = speed;
}

void ThirdPersonCameraController::SetZoomSmoothness(f32 smooth)
{
	zoomSmooth = smooth;
}

void ThirdPersonCameraController::SetAngles(const core::vector2df &angles)
{
	nextAngle = angles;
	
	if (nextAngle.X > maxPitch)
		nextAngle.X = maxPitch;
	
	if (nextAngle.X < minPitch)
		nextAngle.X = minPitch;
	
	currentAngle = nextAngle;
}

core::vector2df ThirdPersonCameraController::GetAngles()
{
	return currentAngle;
}

void ThirdPersonCameraController::SetZoom(f32 zoom)
{
	currentDistance = zoom;
	nextDistance = zoom;
}

f32 ThirdPersonCameraController::GetZoom()
{
	return currentDistance;
}

void ThirdPersonCameraController::SetCollider(ICameraCollider *collider)
{
	SET_REF_COUNTED_POINTER(this->collider, collider);
}

IThirdPersonCameraCollider *ThirdPersonCameraController::CreateThirdPersonCameraCollider()
{
	return new ThirdPersonCollider(world->GetPhysics());
}

void ThirdPersonCameraController::Update(ICamera *camera, f32 dt)
{
	core::vector3df targetPos;
	
	// Smoothly interpolate distance? (for mouse wheel zoom of camera)
	if (zoomSmooth == 0.0)
		currentDistance = nextDistance;
	else
		currentDistance += (nextDistance-currentDistance) * dt * zoomSmooth;
	
	// pos must be gotten from followed object
	if (following)
		targetPos = following->GetAbsolutePosition();
	
	targetPos += core::vector3df(0,heightOffset,0);
	
	
	// Smoothly interpolate angles?
	if (turnSmooth == 0.0)
		currentAngle = nextAngle;
	else
	{
		// Previously used a quaternion and slerp, however I was getting some
		// kind of singularity at 90 degrees on the Y axis...
		currentAngle.X = maths::interpolate_angle(currentAngle.X, nextAngle.X, dt*turnSmooth);
		currentAngle.Y = maths::interpolate_angle(currentAngle.Y, nextAngle.Y, dt*turnSmooth);
	}
	
	core::quaternion quat(core::vector3df(currentAngle.X,currentAngle.Y,0.0) * core::DEGTORAD);
	core::vector3df offset(0,0,-currentDistance);
	offset = quat * offset;
	
	
	if (collider)
	{
		core::vector3df cameraPos = targetPos + offset;
		
		bool collisionOccurred = collider->ProcessCollision(
				core::line3df(targetPos, cameraPos),
				cameraPos
				);
		
		// cameraPos may have been modified by collider.
		camera->SetPosition(cameraPos);
		
		// Irrlicht camera has rotation bound to target.
		camera->SetRotation(core::vector3df(currentAngle.X,currentAngle.Y,0.0));
		
		// Set the distance to target point.
		// Will then keep looking exactly at this point during interpolation.
		// If a collision has occurred, we need to recalculate the distance using the changed cameraPos.
		camera->SetTargetDistance( collisionOccurred ?
				(cameraPos-targetPos).getLength() : currentDistance
				);
	}
	else // No collision detection for camera.
	{
		camera->SetPosition(targetPos + offset);
		
		// Irrlicht camera has rotation bound to target.
		camera->SetRotation(core::vector3df(currentAngle.X,currentAngle.Y,0.0));
		
		// Set the distance to target point.
		// Will then keep looking exactly at this point during interpolation.
		camera->SetTargetDistance(currentDistance);
	}
}

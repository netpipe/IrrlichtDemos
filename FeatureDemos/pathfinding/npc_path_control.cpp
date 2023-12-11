#include "npc_path_control.h"
#include "simple_util.h"

using namespace irr;

NpcPathControl::NpcPathControl
(
	IrrlichtDevice* device,
	scene::ISceneNode* node
)
:	Device(device),
	Node(node),
	Spline(0),
	Speed(SPEED),
	YOffset(0.0f),
	State(IDLE_STATE),
	IsFirstRun(true),
	
	P0(-1),
	P1(-1),
	P2(-1),
	P3(-1),
	RunCount(0),
	IsSplineStarted(false),
	IsSplineEnded(false),
	SplineElapsedTime(0.0f)
{
	Spline = new CatmullRom();
}


NpcPathControl::~NpcPathControl()
{
	if (Spline) { delete Spline; Spline = 0; }
}

f32 NpcPathControl::getAnimatedMeshSceneNodeYOffset(scene::IAnimatedMeshSceneNode*  node)
{
	f32 scale = 1.0f;
	scene::ISceneNode* parent = node->getParent();
	if (parent)
		scale = parent->getScale().Y;

	return node->getTransformedBoundingBox().getExtent().Y * 0.5f * scale;
}

void NpcPathControl::teleportTo
(
	scene::ISceneNode* node,
	const core::vector3df& pos,
	f32 yoffset
)
{
	core::vector3df p = pos;
	p.Y += yoffset;
	node->setPosition(p);
	resetAnimatorCollisionResponse(node);

}
	

void NpcPathControl::resetAnimatorCollisionResponse(scene::ISceneNode* node)
{
	node->updateAbsolutePosition();
	
	const core::list<scene::ISceneNodeAnimator*>& anims = node->getAnimators();
	//const core::list<scene::ISceneNodeAnimator*> anims = node->getAnimators();
	core::list<scene::ISceneNodeAnimator*>::ConstIterator iter = anims.begin();
	while (iter != anims.end())
	{
		if ((*iter)->getType() == scene::ESNAT_COLLISION_RESPONSE)
		{
			scene::ISceneNodeAnimatorCollisionResponse* anim = static_cast<scene::ISceneNodeAnimatorCollisionResponse*>(*iter);
			
			core::vector3df pos = node->getAbsolutePosition();
			pos += anim->getEllipsoidTranslation().Y * 2.0f; //FIXME
			node->setPosition(pos);
			
			anim->setTargetNode(node);
			return;
		}
		++iter;
	}
}	

	
void NpcPathControl::moveNodeTo(const core::vector3df& pos)
{
	Node->updateAbsolutePosition();
	core::vector3df oldPos = Node->getAbsolutePosition();
	
	if (oldPos.equals(pos, 0.001f))
		return;
	// update heading:
	core::vector3df dir = pos - oldPos;
	dir.Y = 0.0f;
	if (core::iszero(dir.getLengthSQ()))
		return;
	
	dir.normalize();
	
	//core::vector3df horizontalAngle = dir.getHorizontalAngle();
	//node->setRotation(horizontalAngle);
	
	core::vector3df heading(0.0f, 0.0f, 1.0f);
	
	core::quaternion q;
	q.rotationFromTo(heading, dir);
	
	core::vector3df eulerRadians;
	q.toEuler(eulerRadians);

	core::vector3df rot = eulerRadians * core::RADTODEG;
	Node->setRotation(rot);
	
	//FIXME: flicking motion
	// update position:
	//f32 yoffset = Node->getTransformedBoundingBox().getExtent().Y * 0.5f;
	//f32 yoffset = Node->getBoundingBox().getExtent().Y * 0.5f;
	//node->setPosition(core::vector3df(pos.X, pos.Y+yoffset, pos.Z));
	Node->setPosition(core::vector3df(pos.X, pos.Y+YOffset, pos.Z));
	//Node->setPosition(pos);
}

void NpcPathControl::start()
{
	if (Waypoints.size() <= 1)
		return;
	
	SplinePoints.clear();
	
	P0 = -1;
	P1 = -1;
	P2 = -1;
	P3 = -1;
	
	RunCount = Waypoints.size() - 1; // e.g. for 10 points, there are 9 segments to transverse
	
	IsSplineStarted = false;
	IsSplineEnded = false;
	SplineElapsedTime = 0.0f;
	
	State = RUNNING_STATE;
	
	moveNodeTo(Waypoints[0]);
}




void NpcPathControl::pause()
{
	if (State == RUNNING_STATE)
		State = PAUSE_STATE;
}


void NpcPathControl::resume()
{
	if (State == PAUSE_STATE)
		State = RUNNING_STATE;
}



void NpcPathControl::stop()
{
	P0 = -1;
	P1 = -1;
	P2 = -1;
	P3 = -1;
	
	RunCount = 0;
	
	IsSplineStarted = false;
	IsSplineEnded = false;
	SplineElapsedTime = 0.0f;
	
	State = IDLE_STATE;
}




void NpcPathControl::update()
{
	if (IsFirstRun)
	{
		init();
		IsFirstRun = false;
		return;
	}
	
	if (Waypoints.size() <= 1)
		return;
	
	if (RunCount == 0)
		return;
	
	Time = Device->getTimer()->getTime();
	
if (State == RUNNING_STATE)
{
	SplineElapsedTime += f32(Time - LastTime) * 0.001f;
	//printf("RunCount=%i ", RunCount);
	//printf("SplineElapsedTime=%f ", SplineElapsedTime);
	
	if ((!IsSplineStarted) && (!IsSplineEnded)) // new iteration
	{
		if ((P1 < 0) && (P2 < 0))
		{
			P1 = 0;
			P2 = 1;
		}
		else
		{
			++P1;
			++P2;
		}
		
		P0 = P1 - 1;
		if (P0 < 0)
			P0 = 0;
	
		P3 = P2 + 1;
		if (P3 >= s32(Waypoints.size()))
			--P3;
		
		Spline->setup(Waypoints[P0], Waypoints[P1], Waypoints[P2], Waypoints[P3], Speed);
		IsSplineStarted = true;
	}
	
	//printf("Spline->getTotalTime()=%f ", Spline->getTotalTime());
	//printf("(P0,P1,P2,P3)=(%i,%i,%i,%i)\n", P0,P1,P2,P3);
	
	if ((IsSplineStarted) && (!IsSplineEnded)) // running same iteration
	{
		core::vector3df pos;
		if (!Spline->getPos(SplineElapsedTime, pos))
		{
			IsSplineEnded = true;
		}
		else
		{
			SplinePoints.push_back(pos);
			moveNodeTo(pos);
		}
	}
	
	if (IsSplineStarted && IsSplineEnded) // iteration completed
	{
		--RunCount;
		SplineElapsedTime = 0.0f;
		IsSplineStarted = false;
		IsSplineEnded = false;
	}
}	
	LastTime = Time;
}


void NpcPathControl::init()
{
	
}


void NpcPathControl::setWaypoints(const core::array<core::vector3df>& waypoints)
{
	this->stop();
	
	Waypoints.clear();
	
	/***/
	if (Node)
	{
		Node->updateAbsolutePosition();
		Waypoints.push_back(Node->getAbsolutePosition());
	}
	/***/
	
	for (u32 n = 0; n < waypoints.size(); ++n)
	{
		core::vector3df waypoint = waypoints[n];
		Waypoints.push_back(waypoint);
	}
}




bool NpcPathControl::gotoWaypoint
(
	const core::vector3df& waypoint,
	f32 speed,
	f32 dt,
	f32 ddsq
)
{
	
	if (speed <= 0.0f)
		return false;
	
	Node->updateAbsolutePosition();
	core::vector3df pos = Node->getAbsolutePosition();
	
	if (pos.equals(waypoint))
		return false;
	
	core::vector3df dir = waypoint - pos;
	dir.normalize();
	
	core::vector3df newPos = dir * dt * speed + pos;
	//printf("f32(dt) * 0.001f * speed=%f\n", f32(dt) * 0.001f * speed);
	
	// update heading:
	core::vector3df horizontalAngle = dir.getHorizontalAngle();
	//horizontalAngle.Y -= 90.0f;
	Node->setRotation(horizontalAngle);
	
	// update position:
	Node->setPosition(newPos);

	//printf("newPos.getDistanceFromSQ(waypoint)=%f, ddsq=%f, ret=%i\n", newPos.getDistanceFromSQ(waypoint), ddsq, newPos.getDistanceFromSQ(waypoint) > ddsq);
	return (newPos.getDistanceFromSQ(waypoint) > ddsq);
}





//void NpcPathControl::

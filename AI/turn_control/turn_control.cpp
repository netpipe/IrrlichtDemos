#include "turn_control.h"


using namespace irr;


TurnControl::TurnControl
(
	IrrlichtDevice* device,
	scene::ISceneNode* node
)
:	Device(device),
	Node(node),

	TurnStartTime(0),
	TotalTurnAngleDegs(0.0f),
	CCW(true),
	OldRot(0.0f),
	IsTurning(false)
{
	
}


TurnControl::~TurnControl()
{
	
}


bool TurnControl::getRelativeHeadingDegs
(
	scene::ISceneNode* node,
	scene::ISceneNode* target,
	f32& degs
)
{
	node->updateAbsolutePosition();
	target->updateAbsolutePosition();
	
	core::vector3df targetDir = target->getAbsolutePosition() - node->getAbsolutePosition();
	if (core::iszero(targetDir.getLengthSQ()))
		return false;
	targetDir.normalize();
	
	
	core::vector3df forward(0.0f, 0.0f, 1.0f);
	core::matrix4 matrix = node->getAbsoluteTransformation();
	matrix.rotateVect(forward);
	forward.normalize();
	
	f32 y1 = targetDir.getHorizontalAngle().Y;
	f32 y2 = forward.getHorizontalAngle().Y;
	
	f32 y = y2 - y1;
	
	while (y < 0.0f)
		y += 360.0f;
	
	while (y >= 360.0f)
		y -= 360.0f;
	
	//printf("y1=%f, y2=%f, y=%f\n", y1, y2, y);
	degs = y;	
	return true;

}

core::vector3df TurnControl::getHeading(scene::ISceneNode* node)
{
	core::matrix4 matrix = node->getAbsoluteTransformation();
	core::vector3df heading(0.0f, 0.0f, 1.0f);
	matrix.rotateVect(heading);
	
	return heading.normalize();
}

void TurnControl::turnTo(scene::ISceneNode* target)
{
	f32 degs = 0.0f;
	if (getRelativeHeadingDegs(Node, target, degs))
	{
		bool ccw = true;
		if ((degs > 180.0f) && (degs < 360.0f))
		{
			ccw = false;
			degs = 360.0f - degs;
		}
				
		this->turnBy(degs, ccw);
	}
}


void TurnControl::update()
{
	
	
	
	if (IsTurning)
	{
		u32 time = Device->getTimer()->getTime();
		f32 t = (time - TurnStartTime) * 0.001f;
		f32 interpolate = 1.0f -exp(- RATE_CONSTANT * t);
		//printf("interpolate=%f\n", interpolate);

		if (interpolate > STOPPING_INTERPOLATE)
		{
			// comment out the following 2 lines if no abrupt change is desired
			core::vector3df r = getDesiredRot(OldRot, TotalTurnAngleDegs, CCW);
			Node->setRotation(r);
			
			IsTurning = false;
			return;
		}
		
		f32 turnAngleDegs = interpolate * TotalTurnAngleDegs;
		core::vector3df rot = getDesiredRot(OldRot, turnAngleDegs, CCW);
		Node->setRotation(rot);
		
	}
}


void TurnControl::turnBy(f32 degs, bool ccw)
{
	TurnStartTime = Device->getTimer()->getTime();
	TotalTurnAngleDegs = degs;
	CCW = ccw;
	
	OldRot = Node->getRotation();
	IsTurning = true;
}



void TurnControl::rotate(scene::ISceneNode* node, const core::vector3df& rot)
{
	core::matrix4 m;
	m.setRotationDegrees(rot);
	
	core::matrix4 n;
	n.setRotationDegrees(node->getRotation());
	m *= n;
	node->setRotation(m.getRotationDegrees());
	
}


core::vector3df TurnControl::getDesiredRot(const core::vector3df& oldRot, f32 turnAngleDegs, bool ccw)
{
	core::vector3df rot(0.0f, turnAngleDegs, 0.0f);
	if (ccw)
		rot *= -1.0f;
	
	core::matrix4 m;
	m.setRotationDegrees(rot);
	
	core::matrix4 n;
	n.setRotationDegrees(oldRot);
	m *= n;
	return m.getRotationDegrees();

}



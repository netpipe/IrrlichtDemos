// An animator that makes one node follow another

#ifndef __C_FOLLOW_ANIMATOR_H_INCLUDED__
#define __C_FOLLOW_ANIMATOR_H_INCLUDED__

#include "ISceneNodeAnimator.h"
#include "ISceneNode.h"
#include "ICameraSceneNode.h"

namespace irr
{

namespace scene
{

	class CFollowAnimator : public ISceneNodeAnimator
	{
	public:

		//! constructor
		CFollowAnimator() : 
			Target(0), FollowNode(0), lastTime(0), 
			Velocity(0,0,0), MaxSpeedSQ(0.3), Thrust(0.3) {}

		//! destructor
		virtual ~CFollowAnimator() 
		{
			if (Target)
				Target->drop();

			if (FollowNode)
				FollowNode->drop();
		}

		//! Animates a scene node.
		//! \param node: Node to animate.
		//! \param timeMs: Current time in milli seconds.
		virtual void animateNode(ISceneNode* node, u32 timeMs)
		{
			if (lastTime == 0)
			{
				lastTime = timeMs;
				return;
			}

			if (!node || (!FollowNode && !Target))
				return;

			f32 timePassed = (f32) (timeMs - lastTime);
			if (timePassed < 5.0f)
				return;
			
			lastTime = timeMs;

			bool isCamera = (node->getType() == ESNT_CAMERA);

			// get positions in world space
			core::vector3df currentPos, targetPos, followPos;

			currentPos = node->getAbsolutePosition();

			if (isCamera && Target)
			{
				ICameraSceneNode* cam = (ICameraSceneNode*)node;
				cam->setTarget(Target->getAbsolutePosition());
			}

			if (FollowNode)
			{
				followPos = FollowNode->getAbsolutePosition();

				core::vector3df direction = (followPos - currentPos).normalize();

				// convert direction to node space
				core::matrix4 m = node->getAbsoluteTransformation();
				m.makeInverse();
				m.rotateVect(direction);

				// change speed and heading
				Velocity += direction * Thrust * timePassed;

				core::vector3df d = Velocity * timePassed;

				// enforce maximum speed
				if (d.getLengthSQ() > MaxSpeedSQ)
				{
					d.setLength(MaxSpeedSQ);
				}

				// move the node
				node->setPosition(node->getPosition() + d);
			}

		}

		//! Sets the node to follow
		//! \param follow: scene node to follow
		virtual void setFollowNode(ISceneNode* follow)
		{
			if (FollowNode)
				FollowNode->drop();

			FollowNode = follow;

			if (FollowNode)
				FollowNode->grab();
		}

		//! If this animator is controlling a camera, sets the target
		//! \param target: node
		virtual void setTargetNode(ISceneNode* target)
		{
			if (Target)
				Target->drop();

			Target = target;

			if (Target)
				Target->grab();
		}

		//! Sets the maximum speed
		//! \param distancePerMilisec: The maximum distance to move in one milisec
		virtual void setMaxSpeed(f32 distancePerMilisec)
		{
			MaxSpeedSQ = distancePerMilisec * distancePerMilisec;
		}

		//! Sets the thrust
		//! \param thrust: The increase in velocity per milisec
		virtual void setMaxThrust(f32 thrust)
		{
			Thrust = thrust;
		}

	private:

		ISceneNode *Target, *FollowNode;
		u32 lastTime;
		core::vector3df Velocity;
		f32 MaxSpeedSQ, Thrust;

	};
} // end namespace scene
} // end namespace irr

#endif


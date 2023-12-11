/*
Copyright (c) 2009, Mathias Gottschlag, Christian Reiser

Permission to use, copy, modify, and/or distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#include "peakphysics/CharacterController.hpp"
#include "peakphysics/Body.hpp"
#include "peakphysics/Capsule.hpp"
#include "peakphysics/Physics.hpp"

#include <btBulletDynamicsCommon.h>

#include <iostream>

namespace peak
{
	CharacterController::CharacterController() : physics(0), body(0), shape(0),
		onground(false)
	{
	}
	CharacterController::~CharacterController()
	{
	}

	void CharacterController::init(Physics *physics)
	{
		this->physics = physics;
		// Create body/shape
		shape = new Capsule();
		((Capsule*)shape)->init(0.3f, 1.0f, 80.0f);
		body = new Body();
		body->init(physics, shape);
	}
	void CharacterController::destroy()
	{
		body->destroy();
		delete body;
		shape->destroy();
		delete shape;
	}

	void CharacterController::setHorizontalSpeed(Vector3F speed)
	{
		this->speed = speed;
	}
	bool CharacterController::onGround()
	{
		return onground;
	}

	class CharacterCallback : public btCollisionWorld::ClosestConvexResultCallback
	{
		public:
			CharacterCallback(btCollisionObject *me, const btVector3 &from,
				const btVector3 &to)
				: btCollisionWorld::ClosestConvexResultCallback(from, to),
				me(me)
			{
			}
			virtual btScalar addSingleResult(btCollisionWorld::LocalConvexResult &result,
				bool normal)
			{
				if (result.m_hitCollisionObject == me)
					return 1.0f;
				return btCollisionWorld::ClosestConvexResultCallback::addSingleResult(result,
					normal);
			}
		private:
			btCollisionObject *me;
	};

	void CharacterController::update()
	{
		// Reset rotation
		body->setRotation(Vector3F(0, 0, 0));
		body->setAngularVelocity(Vector3F(0, 0, 0));
		realspeed = body->getLinearVelocity();
		// Raycast
		Vector3F raystart = body->getPosition();
		Vector3F rayend = raystart + Vector3F(0.0f, -1.3f, 0.0f);
		CollisionInfo collinfo;
		bool onground = false;
		if (physics->castRay(raystart, rayend, &collinfo, body))
		{
			onground = true;
			body->setGravity(Vector3F(0, 0, 0));
			// Reset position
			Vector3F diff = collinfo.point - rayend;
			body->setPosition(collinfo.point + peak::Vector3F(0.0f, 1.1f, 0.0f));
			body->setLinearVelocity(Vector3F(0, 0, 0));
			if (speed.getLength() > 0)
			{
				// Move
				btConvexShape *shape = (btConvexShape*)body->getShape()->getShape();
				btTransform from = body->getTransform();
				btTransform to = from;
				to.setOrigin(to.getOrigin() + btVector3(speed.x / 20, 0, speed.z / 20));
				// Check collisions
				CharacterCallback cb(body->getBody(), from.getOrigin(),
					to.getOrigin());
				physics->getWorld()->convexSweepTest(shape, from, to, cb);
				if (!cb.hasHit())
				{
					body->setPosition(body->getPosition() + Vector3F(speed.x / 20, 0, speed.z / 20));
				}
			}
		}
		else
		{
			onground = false;
			body->setGravity(Vector3F(0.0f, -9.81f, 0.0f));
			body->getBody()->activate();
		}
	}

	Body *CharacterController::getBody()
	{
		return body;
	}
}

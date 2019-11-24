/*
Copyright (c) 2009, Mathias Gottschlag

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

#include "peakphysics/Physics.hpp"
#include "peakphysics/Body.hpp"

#include <btBulletDynamicsCommon.h>
#include <BulletSoftBody/btSoftBody.h>
#include <BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h>
#include <BulletSoftBody/btSoftRigidDynamicsWorld.h>
#include <BulletSoftBody/btSoftBodyHelpers.h>

namespace peak
{
	static bool customContactDestroyedCallback(void* userdata)
	{
		return false;
	}

	static bool customContactAddedCallback(btManifoldPoint &p,
		const btCollisionObject* object0, int id0, int index0,
		const btCollisionObject* object1, int id1, int index1)
	{
		return false;
	}

	Physics::Physics()
	{
	}
	Physics::~Physics()
	{
	}

	bool Physics::init()
	{
		// Create world bounding box
		worldbox = new btAxisSweep3(btVector3(-10000, -10000, -10000), btVector3(10000, 10000, 10000), 1149);

		// Create world
		config = new btSoftBodyRigidBodyCollisionConfiguration();
		dispatcher = new btCollisionDispatcher(config);
		solver = new btSequentialImpulseConstraintSolver();
		world = new btSoftRigidDynamicsWorld(dispatcher, worldbox, solver, config);
		world->setGravity(btVector3(0.0f, -9.81f, 0.0f));

		// Set up collision callbacks
		gContactDestroyedCallback = customContactDestroyedCallback;
		gContactAddedCallback = customContactAddedCallback;
		return true;
	}
	bool Physics::shutdown()
	{
		delete world;
		delete solver;
		delete worldbox;
		delete dispatcher;
		delete config;
		return true;
	}

	void Physics::addBody(Body *body)
	{
		world->addRigidBody(body->getBody());
	}
	void Physics::removeBody(Body *body)
	{
		world->removeRigidBody(body->getBody());
	}

	class RayCallback : public btCollisionWorld::ClosestRayResultCallback
	{
		public:
			RayCallback(btCollisionObject *me, const btVector3 &from,
				const btVector3 &to)
				: btCollisionWorld::ClosestRayResultCallback(from, to),
				me(me)
			{
			}
			virtual btScalar addSingleResult(btCollisionWorld::LocalRayResult &result,
				bool normal)
			{
				if (result.m_collisionObject == me)
					return 1.0f;
				return btCollisionWorld::ClosestRayResultCallback::addSingleResult(result,
					normal);
			}
		private:
			btCollisionObject *me;
	};

	bool Physics::castRay(Vector3F from, Vector3F to, CollisionInfo *info,
		Body *exclude)
	{
		// Cast ray
		btVector3 bfrom(from.x, from.y, from.z);
		btVector3 bto(to.x, to.y, to.z);
		RayCallback tempcallback(exclude ? exclude->getBody() : 0, bfrom, bto);
		world->rayTest(bfrom, bto, tempcallback);
		// Test whether a collision has happened
		if (!tempcallback.hasHit())
			return false;
		// Fill collision info
		if (!info)
			return true;
		info->lambda = tempcallback.m_closestHitFraction;
		info->point = from + (to - from) * info->lambda;
		info->body = (Body*)tempcallback.m_collisionObject->getUserPointer();
		return true;
	}

	btDynamicsWorld *Physics::getWorld()
	{
		return world;
	}

	void Physics::update()
	{
		world->stepSimulation(0.020f);
	}
}

/*
Copyright (C) 2008   Mathias Gottschlag, Christian Reiser

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in the
Software without restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the
Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "physics/World.h"
#include "physics/Body.h"
#include "core/Game.h"
#include "core/Buffer.h"
#include "core/NetworkData.h"
#include "network/Connection.h"
#include "core/Entity.h"

#include <btBulletDynamicsCommon.h>
#include <BulletSoftBody/btSoftBody.h>
#include <BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h>
#include <BulletSoftBody/btSoftRigidDynamicsWorld.h>
#include <BulletSoftBody/btSoftBodyHelpers.h>

namespace peak
{
	bool customContactDestroyedCallback( void* userData )
	{
		printf("custom destroyed callback!\n");
		return false;
	}

	bool customContactAddedCallback(btManifoldPoint &p, const btCollisionObject* object0,int id0,
		int index0,const btCollisionObject* object1,int id1,int index1)
	{
		if (object0->getUserPointer())
		{
			((Body*)object0->getUserPointer())->collideWith((Body*)object1->getUserPointer());
		}
		if (object1->getUserPointer())
		{
			((Body*)object1->getUserPointer())->collideWith((Body*)object0->getUserPointer());
		}
		return false;
	}

	World::World()
	{
		config = 0;
		dispatcher = 0;
		solver = 0;
		world = 0;

		id = 0;
		sync = false;

		timestep = 0;

		serversyncpos = 0;

		datasize = 0;
	}
	World::~World()
	{
	}

	bool World::init(void)
	{
		// Create world bounding box
		worldbox = new btAxisSweep3(btVector3(-10000, -10000, -10000), btVector3(10000, 10000, 10000), 1149);

		// Create world
		config = new btSoftBodyRigidBodyCollisionConfiguration();
		dispatcher = new btCollisionDispatcher(config);
		solver = new btSequentialImpulseConstraintSolver();
		world = new btSoftRigidDynamicsWorld(dispatcher, worldbox, solver, config);
		world->setGravity(btVector3(0, -9.81, 0));

		// Set up collision callbacks
		gContactDestroyedCallback = customContactDestroyedCallback;
		gContactAddedCallback = customContactAddedCallback;

		// Add world to world list
		worlds.push_back(this);

		return true;
	}
	bool World::destroy(void)
	{
		// Remove world from world list
		for (unsigned int i = 0; i < worlds.size(); i++)
		{
			if (worlds[i] == this)
			{
				worlds.erase(worlds.begin() + i);
				break;
			}
		}

		delete world;
		delete solver;
		delete worldbox;
		delete dispatcher;
		delete config;

		return true;
	}

	void World::setNetworkID(int id)
	{
		this->id = id;
	}
	int World::getNetworkID(void)
	{
		return id;
	}
	World *World::getWorld(int id)
	{
		// TODO
		return 0;
	}

	btDynamicsWorld *World::getBulletWorld()
	{
		return world;
	}

	void World::setSynchronized(bool sync)
	{
		this->sync = sync;
	}
	bool World::getSynchronized(void)
	{
		return sync;
	}

	void World::addBody(Body *body)
	{
		world->addRigidBody(body->getBody());
	}
	void World::removeBody(Body *body)
	{
		world->removeRigidBody(body->getBody());

		// Delete body from synchornized bodies
		std::map<int, Body*>::iterator it = syncbodies.begin();
		while (it != syncbodies.end())
		{
			if ((*it).second == body)
			{
				syncbodies.erase(it);
				break;
			}
			it++;
		}
		for (unsigned int i = 0; i < serverbodies.size(); i++)
		{
			if (serverbodies[i] == body)
			{
				serverbodies.erase(serverbodies.begin() + i);
				break;
			}
		}
	}

	void World::addSynchronizedBody(Body *body, int id)
	{
		syncbodies.insert(std::pair<int, Body*>(id, body));
		serverbodies.push_back(body);
		body->setID(id);
	}

	bool World::update(Buffer *data)
	{
		int updatecount = (data->getDataSize() - 1) / 46;
		//printf("Updating %d bodies.\n", updatecount);
		for (int i = 0; i < updatecount; i++)
		{
			int id = data->readInt();
			Vector3D position = data->readVector3D();
			Quaternion rotation = data->readQuaternion();
			Vector3D linearvel = data->readVector3D();
			Vector3D angularvel = data->readVector3D();

			if (syncbodies.find(id) != syncbodies.end())
			{
				Body *body = syncbodies[id];
				body->setPosition(position);
				body->setRotation(rotation);
				body->setLinearVelocity(linearvel);
				body->setAngularVelocity(angularvel);
			}
		}
		return true;
	}
	bool World::sendUpdates(float msecs)
	{
		if (serverbodies.size() == 0) return true;

		datasize += (float)200000 / 8 * msecs / 1000 / Game::get()->getClientCount() * 0.5;

		// Get update data
		Buffer *buffer = new Buffer;
		buffer->writeByte(ENM_Physics);
		unsigned int updating = 0;
		while (datasize > (int)buffer->getDataSize())
		{
			if (serversyncpos >= serverbodies.size()) serversyncpos = 0;
			buffer->writeInt(serverbodies[serversyncpos]->getID());
			buffer->writeVector3D(serverbodies[serversyncpos]->getPosition());
			buffer->writeQuaternion(serverbodies[serversyncpos]->getRotation());
			buffer->writeVector3D(serverbodies[serversyncpos]->getLinearVelocity());
			buffer->writeVector3D(serverbodies[serversyncpos]->getAngularVelocity());
			serversyncpos++;
			updating++;
			if (updating == serverbodies.size()) break;
		}
		datasize -= buffer->getDataSize();

		if (updating == 0) return true;
		//printf("Sending: %d updates.\n", updating);
		// Send data to clients
		for (int i = 0; i < Game::get()->getClientCount(); i++)
		{
			Connection *conn = Game::get()->getClientConnection(i);
			conn->sendData(buffer, false);
		}
		delete buffer;

		return true;
	}

	int World::getBodyNetworkID(void)
	{
		return ++lastbodynetworkid;
	}

	class BulletCallback : public btCollisionWorld::ClosestRayResultCallback
	{
	public:
		BulletCallback() : btCollisionWorld::ClosestRayResultCallback(btVector3(0.0, 0.0, 0.0), btVector3(0.0, 0.0, 0.0))
		{
			body = 0;
		}

		virtual btScalar addSingleResult(btCollisionWorld::LocalRayResult& rayResult, bool normalInWorldSpace)
		{
			printf("Result: %f\n", rayResult.m_hitFraction);
			body = (Body*)rayResult.m_collisionObject->getUserPointer();
			if (body->getEntity())
			{
				printf("Entity: %s\n", body->getEntity()->getName().c_str());
			}

			return ClosestRayResultCallback::addSingleResult (rayResult, normalInWorldSpace);
		}

		Body *body;
	};
	CollisionInfo World::castRay(Vector3D rayfrom, Vector3D rayto)
	{
		CollisionInfo info;

		BulletCallback tempcallback;
		getBulletWorld()->rayTest(btVector3(rayfrom.x, rayfrom.y, rayfrom.z), btVector3(rayto.x, rayto.y, rayto.z), tempcallback);

		//collision = tempcallback.hasHit();
		info.lambda = tempcallback.m_closestHitFraction;
		info.point = rayfrom + (rayto - rayfrom) * info.lambda;
		info.object = tempcallback.body;
		return info;
	}

	void World::doWork(float msecs)
	{
		world->stepSimulation(msecs * 0.001);
	}

	void World::doAllWork(float msecs)
	{
		for (unsigned int i = 0; i < worlds.size(); i++)
		{
			worlds[i]->doWork(msecs);
		}
	}

	std::vector<World*> World::worlds;
	int World::lastbodynetworkid = 0;
}


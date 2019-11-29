/*
Copyright (C) 2008  Mathias Gottschlag

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
#include "physics/Geometry.h"
#include "physics/Collision.h"
#include "core/Logger.h"
#include "core/Script.h"

namespace peak
{
	void nearCallback(void *data, dGeomID geom1, dGeomID geom2)
	{
		dBodyID body1 = dGeomGetBody(geom1);
		dBodyID body2 = dGeomGetBody(geom2);
		if (!body1 && !body2) return;
		World *world = (World*)data;
		Geometry *geometry1 = (Geometry*)dGeomGetData(geom1);
		Geometry *geometry2 = (Geometry*)dGeomGetData(geom2);
		
		CollisionInfo *info1 = 0;
		if (geometry1) info1 = geometry1->getCollisionInfo();
		CollisionInfo *info2 = 0;
		if (geometry2) info2 = geometry2->getCollisionInfo();
		
		// Get contact parameters
		float mu;
		float bounce;
		float bounce_vel;
		if (info1 && info2)
		{
			// FIXME: totally broken
			mu = (info1->friction + info2->friction) / 2;
			bounce = (info1->bounce + info2->bounce) / 2;
			bounce_vel = (info1->bouncevelocity + info2->bouncevelocity) / 2;
		}
		else if (info1)
		{
			mu = info1->friction;
			bounce = info1->bounce;
			bounce_vel = info1->bouncevelocity;
		}
		else if (info2)
		{
			mu = info2->friction;
			bounce = info2->bounce;
			bounce_vel = info2->bouncevelocity;
		}
		
		// Get contact points
		dContact contact[10];
		int count;
		if ((count = dCollide(geom1, geom2, 10, &contact[0].geom, sizeof(dContact))) > 0)
		{
			// Create collision info
			Collision collision;
			collision.geom1 = geometry1;
			collision.geom2 = geometry2;
			collision.contacts.resize(count);
			for (int i = 0; i < count; i++)
			{
				float *v = contact[i].geom.pos;
				collision.contacts[i].position = Vector3D(v[0], v[1], v[2]);
				v = contact[i].geom.normal;
				collision.contacts[i].normal = Vector3D(v[0], v[1], v[2]);
				collision.contacts[i].depth = contact[i].geom.depth;
			}
			// Call callbacks
			if (info1 && info1->callback1.script)
			{
				Script *script = info1->callback1.script;
				if (!script->callFunction<int>(info1->callback1.functionname,
					UserClass("peak::Collision", &collision)))
				{
					return;
				}
			}
			if (info2 && info2->callback1.script)
			{
				Script *script = info2->callback1.script;
				if (!script->callFunction<int>(info2->callback1.functionname,
					UserClass("peak::Collision", &collision)))
				{
					return;
				}
			}
			if (info1 && info1->callback2.script)
			{
				Script *script = info1->callback2.script;
				script->callFunction(info1->callback2.functionname,
					UserClass("peak::Collision", &collision));
			}
			if (info2 && info2->callback2.script)
			{
				Script *script = info2->callback2.script;
				script->callFunction(info2->callback2.functionname,
					UserClass("peak::Collision", &collision));
			}
			// Create contact joints
			for (int i = 0; i < count; i++)
			{
				if (!body1 || !body2 || !dAreConnectedExcluding(body1, body2,  dJointTypeContact))
				{
					contact[i].geom.pos[0] = collision.contacts[i].position.x;
					contact[i].geom.pos[1] = collision.contacts[i].position.y;
					contact[i].geom.pos[2] = collision.contacts[i].position.z;
					contact[i].geom.normal[0] = collision.contacts[i].normal.x;
					contact[i].geom.normal[1] = collision.contacts[i].normal.y;
					contact[i].geom.normal[2] = collision.contacts[i].normal.z;
					contact[i].geom.depth = collision.contacts[i].depth;
					contact[i].surface.mode = dContactBounce | dContactApprox1;
					contact[i].surface.mu = mu;
					contact[i].surface.bounce = bounce;
					contact[i].surface.bounce_vel = bounce_vel;
					dJointID contactjoint = dJointCreateContact(world->getWorld(), world->getContactGroup(), contact + i);
					dJointAttach(contactjoint, body1, body2);
				}
			}
		}
	}
	
	void nearCallbackManual(void *data, dGeomID geom1, dGeomID geom2)
	{
		World *world = (World*)data;
		if (!world) return;
		// Collide spaces if necessary
		if (dGeomIsSpace(geom1) || dGeomIsSpace (geom2))
		{
			dSpaceCollide2(geom1, geom2, data, nearCallbackManual);
			return;
		}
		
		// Fill contact structure
		dContactGeom contact[10];
		
		// Get contact points
		int count, i;
		if ((count = dCollide(geom1, geom2, 10, contact, sizeof(dContactGeom))) > 0)
		{
			if ((dGeomGetClass(geom1) ==  dRayClass) || (dGeomGetClass(geom2) ==  dRayClass)) {
				LDEBUG("Ray collision: %d contacts.\n", count);
			}
			for (i = 0; i < count; i++)
			{
				ContactInfo info;
				info.pos = Vector3D(contact[i].pos[0], contact[i].pos[1], contact[i].pos[2]);
				info.geom = (Geometry*)dGeomGetData(geom1);
				world->getContactPoints()->push_back(info);
			}
		}
	}
	
	World::World()
	{
		world = 0;
		worlds.push_back(this);
		id = ++lastid;
	}
	World::~World()
	{
		for (unsigned int i = 0; i < worlds.size(); i++)
		{
			if (worlds[i] == this)
			{
				worlds.erase(worlds.begin() + i);
				return;
			}
		}
	}
	
	bool World::init(void)
	{
		// Create ODE world
		dWorldID newworld = dWorldCreate();
		space = dSimpleSpaceCreate(0);
		contactgroup = dJointGroupCreate(0);
		dWorldSetGravity(newworld, 0, -9.81, 0);
		dWorldSetERP(newworld, 0.8);
		dWorldSetContactMaxCorrectingVel(newworld, 0.9);
		dWorldSetContactSurfaceLayer(newworld, 0.001);
		
		world = newworld;
		
		return true;
	}
	bool World::destroy(void)
	{
		dJointGroupDestroy(contactgroup);
		dSpaceDestroy(space);
		dWorldDestroy(world);
		world = 0;
		return true;
	}
	
	int World::collide(Geometry *geom)
	{
		contactpoints.clear();
		dSpaceCollide2(geom->getGeom(), (dGeomID)space, this, nearCallbackManual);
		return contactpoints.size();
	}
	std::vector<ContactInfo> *World::getContactPoints(void)
	{
		return &contactpoints;
	}
	ContactInfo World::getContactPoint(int index)
	{
		return contactpoints[index];
	}
	
	int World::getID(void)
	{
		return id;
	}
	
	void World::doWork(float msecs)
	{
		if (!world) return;
		if (msecs / 1000 > 0)
		{
			// Update physics
			dSpaceCollide(space, this, &nearCallback);
			dWorldQuickStep(world, msecs / 1000);
			dJointGroupEmpty(contactgroup);
		}
	}
	void World::doAllWork(float msecs)
	{
		for (unsigned int i = 0; i < worlds.size(); i++)
		{
			worlds[i]->doWork(msecs);
		}
	}
	
	dSpaceID World::getSpace(void)
	{
		return space;
	}
	dJointGroupID World::getContactGroup(void)
	{
		return contactgroup;
	}
	
	World *World::getWorld(int id)
	{
		for (unsigned int i = 0; i < worlds.size(); i++)
		{
			if (worlds[i]->getID() == id)
			{
				return worlds[i];
			}
		}
		return 0;
	}
	
	std::vector<World*> World::worlds;
	
	int World::lastid = 0;
}


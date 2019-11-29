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

#include "entity/PhysicsComponent.h"
#include "core/Game.h"
#include "physics/Geometry.h"
#include "physics/Body.h"
#include "physics/Joint.h"

namespace peak
{
	PhysicsComponent::PhysicsComponent()
	{
		world = 0;
	}
	PhysicsComponent::~PhysicsComponent()
	{
	}
	
	bool PhysicsComponent::init(Entity *entity)
	{
		world = Game::get()->getWorld();
		return true;
	}
	bool PhysicsComponent::destroy(void)
	{
		// Delete joints
		std::map<int, Joint*>::iterator jit = joints.begin();
		while (jit != joints.end())
		{
			(*jit).second->destroy();
			delete (*jit).second;
			jit++;
		}
		joints.clear();
		// Delete geometry
		std::map<int, Geometry*>::iterator git = geoms.begin();
		while (git != geoms.end())
		{
			(*git).second->destroy();
			delete (*git).second;
			git++;
		}
		geoms.clear();
		// Delete bodies
		std::map<int, Body*>::iterator bit = bodies.begin();
		while (bit != bodies.end())
		{
			(*bit).second->destroy();
			delete (*bit).second;
			bit++;
		}
		bodies.clear();

		return true;
	}
	
	bool PhysicsComponent::needsUpdate(void)
	{
		return false;
	}
	int PhysicsComponent::getUpdateSize(void)
	{
		return 0;
	}
	int PhysicsComponent::writeUpdateData(Buffer *buffer)
	{
		return false;
	}
	
	bool PhysicsComponent::updateFromData(Buffer *buffer)
	{
		return false;
	}
	
	bool PhysicsComponent::writeCompleteData(Buffer *buffer)
	{
		return true;
	}
	bool PhysicsComponent::readCompleteData(Buffer *buffer)
	{
		return true;
	}
	
	void PhysicsComponent::move(Vector3D pos)
	{
		// Move all bodies
		std::map<int, Body*>::iterator it = bodies.begin();
		while (it != bodies.end())
		{
			Body *body = (*it).second;
			body->setPosition(body->getPosition() + pos);
			it++;
		}
	}
	
	void PhysicsComponent::addBody(Body *body, int id)
	{
		bodies.insert(std::pair<int, Body*>(id, body));
	}
	Body *PhysicsComponent::getBody(int id)
	{
		return (*bodies.find(id)).second;
	}
	void PhysicsComponent::addJoint(Joint *joint, int id)
	{
		joints.insert(std::pair<int, Joint*>(id, joint));
	}
	Joint *PhysicsComponent::getJoint(int id)
	{
		return (*joints.find(id)).second;
	}
	void PhysicsComponent::addGeom(Geometry *geom, int id, int parentid)
	{
		Body *body = getBody(parentid);
		if (body) geom->setBody(body);
		geoms.insert(std::pair<int, Geometry*>(id, geom));
	}
	Geometry *PhysicsComponent::getGeom(int id)
	{
		return (*geoms.find(id)).second;
	}
	
	ComponentType PhysicsComponent::getType(void)
	{
		return ECT_Physics;
	}
	
	bool PhysicsComponent::doWork(float msecs)
	{
		return true;
	}
}


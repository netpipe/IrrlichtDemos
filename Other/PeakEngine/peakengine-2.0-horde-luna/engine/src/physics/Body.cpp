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

#include "physics/Body.h"
#include "physics/Shape.h"
#include "physics/World.h"
#include "core/GameEngine.h"
#include "core/Property.h"
#include "core/Event.h"

#include <btBulletDynamicsCommon.h>

namespace peak
{
	Body::Body()
	{
		body = 0;
		state = 0;
		entity = 0;

		networkid = 0;

		collisionevent = new Event;
		addEvent("Collision", collisionevent);
	}
	Body::~Body()
	{
		delete collisionevent;
	}

	bool Body::create(World *world, Shape *shape, bool autodeactivation)
	{
		state = new btDefaultMotionState(shape->getTransform());
		Vector3D inertia = shape->getInertia();

		btRigidBody::btRigidBodyConstructionInfo rbinfo(shape->getMass(), state, shape->getShape(), btVector3(inertia.x, inertia.y, inertia.z));
		body = new btRigidBody(rbinfo);
		body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
		body->setUserPointer(this);

		world->addBody(this);

		bodies.push_back(this);

		addProperty("position", new Vector3DProperty("position"));
		addProperty("rotation", new QuaternionProperty("rotation"));

		this->world = world;
		this->shape = shape;
		this->autodeactivation = autodeactivation;

		return true;
	}
	bool Body::destroy(void)
	{
		for (unsigned int i = 0; i < bodies.size(); i++)
		{
			if (bodies[i] == this)
			{
				bodies.erase(bodies.begin() + i);
				break;
			}
		}

		world->removeBody(this);
		delete body;
		delete state;

		return true;
	}

	void Body::setPosition(Vector3D position)
	{
		btTransform transform;
		transform.setIdentity();
		transform = body->getCenterOfMassTransform();
		transform.setOrigin(btVector3(position.x, position.y, position.z));
		body->setCenterOfMassTransform(transform);
	}
	Vector3D Body::getPosition(void)
	{
		btTransform transform = body->getCenterOfMassTransform();
		btVector3 pos = transform.getOrigin();
		return Vector3D(pos.x(),pos.y(),pos.z());
	}

	void Body::setRotation(Quaternion rotation)
	{
		btTransform transform = body->getCenterOfMassTransform();
		transform.setRotation(btQuaternion(rotation.x, rotation.y, rotation.z, rotation.w));
		body->setCenterOfMassTransform(transform);
	}
	Quaternion Body::getRotation(void)
	{
		btQuaternion quat = body->getOrientation();
		return Quaternion(quat.x(), quat.y(), quat.z(), quat.w());
	}

	void Body::update(void)
	{
		if (!autodeactivation)
		{
			// Reenable body
///			body->activate(true);
		}

		Property *position = getProperty("position");
		position->disableUpdates();
		Vector3D pos = getPosition();
		position->set(pos);
		position->enableUpdates();
		Property *rotation = getProperty("rotation");
		rotation->disableUpdates();
		Quaternion rot = getRotation();
		rotation->set(rot);
		rotation->enableUpdates();
	}
	void Body::updateAll(void)
	{
		for (unsigned int i = 0; i < bodies.size(); i++)
		{
			bodies[i]->update();
		}
	}

	std::vector<Body*> Body::getBodies(void)
	{
		return bodies;
	}

	void Body::collideWith(Body *other)
	{
		collisionbody = other;
		collisionevent->trigger(0);
	}
	Body *Body::getCollisionBody(void)
	{
		return collisionbody;
	}

	void Body::setEntity(Entity *entity)
	{
		this->entity = entity;
	}
	Entity *Body::getEntity(void)
	{
		return entity;
	}

	btRigidBody *Body::getBody(void)
	{
		return body;
	}

	Shape *Body::getShape()
	{
		return shape;
	}

	void Body::setForce(Vector3D force, Vector3D relativeposition)
	{
		body->applyForce(btVector3(force.x, force.y, force.z), btVector3(relativeposition.x, relativeposition.y, relativeposition.z));
		// TODO: Proper activation/deactivation
		body->activate(true);
	}

	void Body::clearForces(void)
	{
		body->clearForces();
	}

	void  Body::setLinearVelocity(Vector3D linearvel)
	{
		body->setLinearVelocity(btVector3(linearvel.x, linearvel.y, linearvel.z));
	}
	Vector3D Body::getLinearVelocity(void)
	{
		btVector3 linearvel = body->getLinearVelocity();
		return Vector3D(linearvel.x(), linearvel.y(), linearvel.z());
	}

	void  Body::setAngularVelocity(Vector3D angularvel)
	{
		body->setAngularVelocity(btVector3(angularvel.x, angularvel.y, angularvel.z));
	}

	Vector3D Body::getAngularVelocity(void)
	{
		btVector3 angularvel = body->getAngularVelocity();
		return Vector3D(angularvel.x(), angularvel.y(), angularvel.z());
	}

	void Body::setGravity(Vector3D gravity)
	{
		body->setGravity(btVector3(gravity.x, gravity.y, gravity.z));
	}
	Vector3D Body::getGravity(void)
	{
		btVector3 gravity = body->getGravity();
		return Vector3D(gravity.x(), gravity.y(), gravity.z());
	}

	void Body::setID(int id)
	{
		networkid = id;
	}
	int Body::getID(void)
	{
		return networkid;
	}

	void Body::onPropertyChanged(std::string name)
	{
		GameObject::onPropertyChanged(name);
		Property *property = getProperty(name);
		if (name == "position")
		{
			Vector3D posvalue = property->getVector3D();
			btTransform transform;
			transform.setIdentity();
			transform = body->getCenterOfMassTransform();
			transform.setOrigin(btVector3(posvalue.x, posvalue.y, posvalue.z));
			body->setCenterOfMassTransform(transform);
		}
		else if (name == "rotation")
		{
			Quaternion rotvalue = property->getQuaternion();
			btTransform transform = body->getCenterOfMassTransform();
			transform.setRotation(btQuaternion(rotvalue.x, rotvalue.y, rotvalue.z, rotvalue.w));
			body->setCenterOfMassTransform(transform);
		}
	}

	std::vector<Body*> Body::bodies;
}

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

#include "peakphysics/Body.hpp"
#include "peakphysics/Physics.hpp"
#include "peakphysics/Shape.hpp"

#include <btBulletDynamicsCommon.h>

namespace peak
{
	Body::Body() : body(0), physics(0), state(0), userdata(0)
	{
	}
	Body::~Body()
	{
	}

	bool Body::init(Physics *physics, Shape *shape, bool autodeactivation)
	{
		btTransform transform;
		transform.setIdentity();
		transform.setOrigin(btVector3(0, 0, 0));

		state = new btDefaultMotionState(transform);
		Vector3F inertia = shape->getInertia();

		btRigidBody::btRigidBodyConstructionInfo rbinfo(shape->getMass(), state,
			shape->getShape(), btVector3(inertia.x, inertia.y, inertia.z));
		body = new btRigidBody(rbinfo);
		//body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
		body->setUserPointer(this);

		physics->addBody(this);

		this->physics = physics;
		this->shape = shape;
		this->autodeactivation = autodeactivation;
		return true;
	}
	bool Body::destroy()
	{
		physics->removeBody(this);
		delete body;
		delete state;

		return true;
	}

	void Body::setPosition(const Vector3F &position)
	{
		btTransform transform;
		transform.setIdentity();
		transform = body->getCenterOfMassTransform();
		transform.setOrigin(btVector3(position.x, position.y, position.z));
		body->setCenterOfMassTransform(transform);
	}
	Vector3F Body::getPosition()
	{
		btTransform transform = body->getCenterOfMassTransform();
		btVector3 pos = transform.getOrigin();
		return Vector3F(pos.x(),pos.y(),pos.z());
	}

	void Body::setRotation(const Quaternion &rotation)
	{
		btTransform transform = body->getCenterOfMassTransform();
		transform.setRotation(btQuaternion(rotation.q[0], rotation.q[1],
			rotation.q[2], rotation.q[3]));
		body->setCenterOfMassTransform(transform);
	}
	void Body::setRotation(const Vector3F &rotation)
	{
		setRotation(Quaternion(rotation));
	}
	Quaternion Body::getRotation()
	{
		btQuaternion quat = body->getOrientation();
		return Quaternion(quat.x(), quat.y(), quat.z(), quat.w());
	}
	void Body::setLinearVelocity(const Vector3F &linearvel)
	{
		body->setLinearVelocity(btVector3(linearvel.x, linearvel.y, linearvel.z));
	}
	Vector3F Body::getLinearVelocity()
	{
		btVector3 linearvel = body->getLinearVelocity();
		return Vector3F(linearvel.x(), linearvel.y(), linearvel.z());
	}

	void Body::setAngularVelocity(const Vector3F &angularvel)
	{
		body->setAngularVelocity(btVector3(angularvel.x, angularvel.y, angularvel.z));
	}
	Vector3F Body::getAngularVelocity()
	{
		btVector3 angularvel = body->getAngularVelocity();
		return Vector3F(angularvel.x(), angularvel.y(), angularvel.z());
	}

	void Body::setGravity(const Vector3F &gravity)
	{
		body->setGravity(btVector3(gravity.x, gravity.y, gravity.z));
	}
	Vector3F Body::getGravity()
	{
		btVector3 gravity = body->getGravity();
		return Vector3F(gravity.x(), gravity.y(), gravity.z());
	}

	void Body::setUserData(void *data)
	{
		userdata = data;
	}
	void *Body::getUserData()
	{
		return userdata;
	}

	btRigidBody *Body::getBody()
	{
		return body;
	}
	btTransform &Body::getTransform()
	{
		return body->getWorldTransform();
	}

	Shape *Body::getShape()
	{
		return shape;
	}
}

/*
Copyright (C) 2008  Mathias Gottschlag, Lukas Kropatschek

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
#include "physics/World.h"

namespace peak
{
	Body::Body()
	{
		entity = 0;
		bodies.push_back(this);
		absforce = Vector3D(0, 0, 0);
		relforce = Vector3D(0, 0, 0);
	}
	Body::~Body()
	{
		std::list<Body*>::iterator it = bodies.begin();
		while (it != bodies.end())
		{
			if (*it == this)
			{
				bodies.erase(it);
				break;
			}
			it++;
		}
	}
	
	bool Body::init(World *world, Entity *entity)
	{
		body = dBodyCreate(world->getWorld());
		this->entity = entity;
		return true;
	}
	bool Body::destroy(void)
	{
		dBodyDestroy(body);
		return true;
	}
	
	void Body::setPosition(float x, float y, float z)
	{
		dBodySetPosition(body, x, y, z);
	}
	void Body::setPosition(Vector3D pos)
	{
		dBodySetPosition(body, pos.x, pos.y, pos.z);
	}
	Vector3D Body::getPosition(void)
	{
		const dReal *pos = dBodyGetPosition(body);
		return Vector3D(pos[0], pos[1], pos[2]);
	}
	
	void Body::setRotation(float x, float y, float z)
	{
		// FIXME: Wrong rotation order
		dMatrix3 matrix;
		dRFromEulerAngles(matrix, x, y, z);
		dBodySetRotation(body, matrix);
	}
	void Body::setRotation(Vector3D rot)
	{
		// FIXME: Wrong rotation order
		dMatrix3 matrix;
		dRFromEulerAngles(matrix, rot.x, rot.y, rot.z);
		dBodySetRotation(body, matrix);
	}
	void Body::setRotation(Quaternion rot)
	{
		dBodySetQuaternion(body, rot.q);
	}
	Quaternion Body::getRotation(void)
	{
		Quaternion orientation;
		const dReal *matrix;
		matrix = dBodyGetRotation(body);
		dRtoQ(matrix, orientation.q);
		return orientation;
	}
	
	void Body::setLinearVelocity(Vector3D vel)
	{
		dBodySetLinearVel(body, vel.x, vel.y, vel.z);
	}
	Vector3D Body::getLinearVelocity(void)
	{
		const dReal * vel = dBodyGetLinearVel(body);
		return Vector3D(vel[0], vel[1], vel[2]);
	}
	void Body::setAngularVelocity(Vector3D vel)
	{
		dBodySetAngularVel(body, vel.x, vel.y, vel.z);
	}
	Vector3D Body::getAngularVelocity(void)
	{
		const dReal * vel = dBodyGetAngularVel(body);
		return Vector3D(vel[0], vel[1], vel[2]);
	}
	
	void Body::addForce(Vector3D force)
	{
		dBodyAddForce(body, force.x, force.y, force.z);
	}
	
	void Body::setForce(Vector3D force)
	{
		absforce = force;
	}
	Vector3D Body::getForce(void)
	{
		return absforce;
	}
	void Body::setRelativeForce(Vector3D force)
	{
		relforce = force;
	}
	Vector3D Body::getRelativeForce(void)
	{
		return relforce;
	}
	
	void Body::setEntity(Entity *entity)
	{
		this->entity = entity;
	}
	Entity *Body::getEntity(void)
	{
		return entity;
	}
	
	void Body::setGravity(bool enabled)
	{
		dBodySetGravityMode(body, enabled);
	}
	bool Body::getGravity(void)
	{
		return dBodyGetGravityMode(body);
	}
	
	dBodyID Body::getBody(void)
	{
		return body;
	}
	
	void Body::update(void)
	{
		dBodyAddForce(body, absforce.x, absforce.y, absforce.z);
		dBodyAddRelForce(body, relforce.x, relforce.y, relforce.z);
	}
	void Body::updateAll(void)
	{
		std::list<Body*>::iterator it = bodies.begin();
		while (it != bodies.end())
		{
			(*it)->update();
			it++;
		}
	}
	
	std::list<Body*> Body::bodies;
}


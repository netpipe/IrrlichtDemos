/*
Copyright (C) 2008   Christian Reiser

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

#include "physics/RayCallback.h"
#include "physics/World.h"
#include "core/Game.h"
#include "core/Script.h"
#include "core/Logger.h"

#include <btBulletDynamicsCommon.h>



namespace peak
{
	RayCallback::RayCallback()
	{
		type = ECT_Ray;
	}
	RayCallback::~RayCallback()
	{
	}

	bool RayCallback::create(Vector3D rayfrom, Vector3D rayto, Script *script, std::string callbackname)
	{
		Callback::create();
		this->rayfrom = rayfrom;
		this->rayto = rayto;
		this->script = script;
		this->callbackname = callbackname;
		return true;
	}
	bool RayCallback::destroy()
	{
		return Callback::destroy();
	}

	void RayCallback::update()
	{
		class BulletCallback : public btCollisionWorld::ClosestRayResultCallback
		{
		public:
			BulletCallback(RayCallback *peakcallback) : btCollisionWorld::ClosestRayResultCallback(btVector3(0.0, 0.0, 0.0), btVector3(0.0, 0.0, 0.0))
			{
				this->peakcallback = peakcallback;
			}

			virtual btScalar addSingleResult(btCollisionWorld::LocalRayResult& rayResult,bool normalInWorldSpace)
			{
				std::vector<Body*> bodies = Body::getBodies();
				CollisionInfo info;
				
				for (unsigned int i = 0; i < bodies.size(); i++)
				{
					if (bodies[i]->getBody() == rayResult.m_collisionObject)
					{
						info.object = bodies[i];
						peakcallback->setInfo(info);
						break; 
					}
				}
				LDEBUG("Callback class was called!");
				return ClosestRayResultCallback::addSingleResult (rayResult, normalInWorldSpace);
			}
		protected:
			RayCallback *peakcallback;
		};
		BulletCallback tempcallback(this);
		Game::get()->getWorld()->getBulletWorld()->rayTest(btVector3(rayfrom.x, rayfrom.y, rayfrom.z), btVector3(rayto.x, rayto.y, rayto.z), tempcallback);
		LDEBUG("Ray test was made. Waiting for callback...");
		
		collision = tempcallback.hasHit();
		if (collision)
		{
			LDEBUG("Collision was determined\n");
			if (script->isFunction(callbackname))
			{
				LDEBUG("Callback will be called\n");
				script->callFunction(callbackname);
			}
		}
	}

	bool RayCallback::isCollision()
	{
		return collision;
	}

	void RayCallback::setRay(Vector3D rayfrom, Vector3D rayto)
	{
		this->rayto = rayto;
		this->rayfrom = rayfrom;
	}
	Vector3D RayCallback::getRay(bool index)
	{
		return index?rayto:rayfrom;
	}

	void RayCallback::setInfo(CollisionInfo info)
	{
		this->info = info;
	}
	CollisionInfo RayCallback::getInfo(void)
	{
		return info;
	}
}

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

#ifndef _WORLD_H_
#define _WORLD_H_

#include "physics/RayCallback.h"

#include <vector>
#include <map>

class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btDynamicsWorld;
class btSequentialImpulseConstraintSolver;
class btAxisSweep3;

//tolua_begin
namespace peak
{
	class Body;
	class Buffer;
	
	/**
	* \brief World used for physics simulations containing bodies and geometry.
	*/
	class World
	{
		public:
			World();
			~World();
			
			/**
			* \brief Initializes the world.
			*/
			bool init(void);
			/**
			* \brief Destroys the world.
			*/
			bool destroy(void);
			
			/**
			 * \brief Gets the world with the given id.
			 *
			 * Not working yet!
			 *
			 */
			static World *getWorld(int id);

			/**
			 * \brief Adds a body to the world.
			 *
			 * \param body This body will be added to the world.
			 *
			 */
			void addBody(Body *body);
			/**
			 * \brief Removes a body from the world.
			 *
			 * \param body This body will be removed from the world.
			 *
			 */
			void removeBody(Body *body);
			
			CollisionInfo castRay(Vector3D rayfrom, Vector3D rayto);
			
			/**
			* \brief Runs the physics simulation for a given time.
			* \param msecs Passed time
			*/
			void doWork(float msecs);
			
			/**
			* \brief Runs the physics simulation for all worlds.
			* \param msecs Passed time
			*/
			static void doAllWork(float msecs);
			
			//tolua_end
			/**
			 * \brief Gets the real bullet world, for internal use only.
			 */
			btDynamicsWorld *getBulletWorld();
		private:
		
			btDefaultCollisionConfiguration *config;
			btCollisionDispatcher *dispatcher;
			btSequentialImpulseConstraintSolver *solver;
			btDynamicsWorld *world;
			btAxisSweep3 *worldbox;
			
			float timestep;
			
			static std::vector<World*> worlds;
			
			//tolua_begin
	};
}
//tolua_end

#endif


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

#include "core/GameObject.h"
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
	class World : GameObject
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
			 * \brief Sets the network id of the world.
			 */
			void setNetworkID(int id);
			/**
			 * \brief Gets the network id of the world.
			 */
			int getNetworkID(void);
			/**
			 * \brief Gets the world with the given id.
			 *
			 * Not working yet!
			 *
			 */
			static World *getWorld(int id);

			
			// Note: Server/Client is set automatically based on Game::isServer()
			/**
			 * \brief Sets the synchronisation of the world
			 * 
			 * \param sync If true the world will be synchronized if false the world won't be synchronized
			 *
			 */
			void setSynchronized(bool sync);
			/**
			 * \brief Gets the synchronisation of the world
			 *
			 * \return Returns true if the world is synchronized and false if it isn't.
			 *
			 */
			bool getSynchronized(void);
			
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
			
			/**
			 * \brief Adds an synchronized body to the world.
			 *
			 * \param body This body will be added to the world.
			 * \param id The id of the body in the world.
			 */
			void addSynchronizedBody(Body *body, int id);
			
			/**
			 * \brief Updates the world via networking
			 */
			bool update(Buffer *data);
			/**
			 * \brief Send updates
			 */
			bool sendUpdates(float msecs);
			
			/**
			 * \brief Gets the body network id of the last created body.
			 */
			static int getBodyNetworkID(void);
			
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
			int id;
			bool sync;
			
			btDefaultCollisionConfiguration *config;
			btCollisionDispatcher *dispatcher;
			btSequentialImpulseConstraintSolver *solver;
			btDynamicsWorld *world;
			btAxisSweep3 *worldbox;
			
			std::map<int, Body*> syncbodies;
			std::vector<Body*> serverbodies;
			unsigned int serversyncpos;
			
			float timestep;
			
			static std::vector<World*> worlds;
			
			static int lastbodynetworkid;
			
			int datasize;
			//tolua_begin
	};
}
//tolua_end

#endif


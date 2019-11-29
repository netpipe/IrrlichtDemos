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

#ifndef _WORLD_H_
#define _WORLD_H_

#include <ode/ode.h>
#include <vector>

#include "core/Vector3D.h"

//tolua_begin
namespace peak
{
	class Geometry;
	
	/**
	 * \brief Struct holding contact information.
	 */
	struct ContactInfo
	{
		/**
		 * \brief Position of the contact
		 */
		Vector3D pos;
		/**
		 * \brief Geometry which collided with the geometry used in World::collide().
		 */
		Geometry *geom;
	};
	
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
			 * \brief Collides a geometry with the world.
			 *
			 * The contact points can be reached via getContactPoints().
			 *
			 * This does not in any way have any influence on the simulation.
			 * \param geom Geometry to check for collisions
			 * \return Number of contact points
			 */
			int collide(Geometry *geom);
			/**
			 * \brief Returns the contact points computed by the last call of collide().
			 */
			std::vector<ContactInfo> *getContactPoints(void);
			/**
			 * \brief Returns a single contact point computed by the last call of collide().
			 */
			ContactInfo getContactPoint(int index);
			
			/**
			 * \brief Returns the ID of this world.
			 *
			 * To support multiple worlds every world has an unique ID.
			 */
			int getID(void);
			
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
			
			/**
			 * \brief Returns the ID of the world used by ODE.
			 */
			dWorldID getWorld(void)
			{
				return world;
			}
			/**
			 * \brief Returns the ID of the geometry space used by ODE.
			 */
			dSpaceID getSpace(void);
			/**
			 * \brief Returns the ID of the joint group used by ODE holding the
			 * contact joints.
			 */
			dJointGroupID getContactGroup(void);
			
			/**
			 * \brief Returns the world with the given ID.
			 */
			static World *getWorld(int id);
		private:
			//tolua_end
			dWorldID world;
			dSpaceID space;
			dJointGroupID contactgroup;
			std::vector<ContactInfo> contactpoints;
			
			static std::vector<World*> worlds;
			
			int id;
			static int lastid;
			//tolua_begin
	};
}
//tolua_end

#endif


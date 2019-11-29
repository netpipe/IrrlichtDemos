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

#ifndef _PHYSICSCOMPONENT_H_
#define _PHYSICSCOMPONENT_H_

#include "entity/Component.h"
#include "core/Vector3D.h"
#include "core/Quaternion.h"

#include <map>

//tolua_begin
namespace peak
{
	class World;
	class Body;
	class Joint;
	class Geometry;
	
	/**
	 * \brief Class holding the physical properties of an entity
	 *
	 * This component only exists on the server.
	 */
	class PhysicsComponent : public Component
	{
		public:
			PhysicsComponent();
			virtual ~PhysicsComponent();
			
			virtual bool init(Entity *entity);
			virtual bool destroy(void);
			
			virtual bool needsUpdate(void);
			virtual int getUpdateSize(void);
			virtual int writeUpdateData(Buffer *buffer);
			
			virtual bool updateFromData(Buffer *buffer);
			
			virtual bool writeCompleteData(Buffer *buffer);
			virtual bool readCompleteData(Buffer *buffer);
			
			/**
			 * \brief Moves all bodies.
			 * \param pos Relative new position of the bodies
			 */
			void move(Vector3D pos);
			
			/**
			 * \brief Adds a body to the component.
			 * \param body Body to add
			 * \param id ID of the body
			 */
			void addBody(Body *body, int id);
			/**
			 * \brief Returns the body with the given ID
			 */
			Body *getBody(int id);
			/**
			 * \brief Adds a joint to the component
			 * \param joint Joint to add
			 * \param id ID of the joint
			 */
			void addJoint(Joint *joint, int id);
			/**
			 * \brief Returns the joint with the given ID
			 */
			Joint *getJoint(int id);
			/**
			 * \brief Adds a geom to the component
			 * \param geom Geometry to add
			 * \param id ID of the geom
			 * \param parentid ID of the parent of the geom (e.g. a Body)
			 */
			void addGeom(Geometry *geom, int id, int parentid);
			/**
			 * \brief Returns the geom with the given ID
			 */
			Geometry *getGeom(int id);
			
			virtual ComponentType getType(void);
			
			virtual bool doWork(float msecs);
		private:
			//tolua_end
			World *world;
			std::map<int, Body*> bodies;
			std::map<int, Joint*> joints;
			std::map<int, Geometry*> geoms;
			//tolua_begin
	};
}
//tolua_end

#endif


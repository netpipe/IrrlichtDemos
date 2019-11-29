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

#ifndef _BODY_H_
#define _BODY_H_

#include <ode/ode.h>
#include <list>

#include "core/Vector3D.h"
#include "core/Quaternion.h"

//tolua_begin
namespace peak
{
	class World;
	class Geometry;
	class Entity;
	
	/**
	 * \brief Class representing a body with its physical attributes.
	 */
	class Body
	{
		public:
			Body();
			~Body();
			
			/**
			 * \brief Creates a body.
			 * \param world World to place the body into
			 * \param entity Entity which holds the body
			 */
			bool init(World *world, Entity *entity = 0);
			/**
			 * \brief Destroys the body.
			 */
			bool destroy(void);
			
			/**
			 * \brief Sets the position of the body.
			 */
			void setPosition(float x, float y, float z);
			/**
			 * \brief Sets the position of the body.
			 * \param pos New position
			 */
			void setPosition(Vector3D pos);
			/**
			 * \brief Returns the position of the body.
			 */
			Vector3D getPosition(void);
			/**
			 * \brief Sets the rotation of the body.
			 */
			void setRotation(float x, float y, float z);
			/**
			 * \brief Sets the rotation of the body.
			 * \param rot Vector holding the new rotation (Euler angles)
			 */
			void setRotation(Vector3D rot);
			/**
			 * \brief Sets the rotation of the body.
			 * \param rot Quaternion holding the new rotation
			 */
			void setRotation(Quaternion rot);
			/**
			 * \brief Returns the rotation of the body as a quaternion.
			 */
			Quaternion getRotation(void);
			
			/**
			 * \brief Sets the linear velocity of the body
			 * \param vel New velocity
			 */
			void setLinearVelocity(Vector3D vel);
			/**
			 * \brief Returns the linear velocity of the body
			 */
			Vector3D getLinearVelocity(void);
			/**
			 * \brief Sets the angular velocity of the body
			 * \param vel New velocity
			 */
			void setAngularVelocity(Vector3D vel);
			/**
			 * \brief Returns the angular velocity of the body
			 */
			Vector3D getAngularVelocity(void);
			
			/**
			 * \brief Applies a force to the body
			 *
			 * The force only affects the next step of the simulation, so you
			 * need to call this function repeatedly.
			 *
			 * WARNING: Sometimes one frame on the server involves several simulation
			 * steps, in this case addForce does *not* work!
			 */
			void addForce(Vector3D force);
			
			/**
			 * \brief Sets the force which applies to the body.
			 */
			void setForce(Vector3D force);
			/**
			 * \brief Gets the force which applies to the body.
			 */
			Vector3D getForce(void);
			/**
			 * \brief Sets the force which applies to the body using its rotated axis.
			 */
			void setRelativeForce(Vector3D force);
			/**
			 * \brief Gets the force which applies to the body using its rotated axis.
			 */
			Vector3D getRelativeForce(void);
			
			/**
			 * \brief Sets the entity which holds this body
			 *
			 * This can be used for things like collision info.
			 * \param entity New entity
			 */
			void setEntity(Entity *entity);
			/**
			 * \brief Returns the entity which is associated with this body
			 */
			Entity *getEntity(void);
			
			/**
			 * \brief Sets whether the body is influenced by gravity.
			 */
			void setGravity(bool enabled);
			/**
			 * \brief Returns whether the body is influenced by gravity.
			 */
			bool getGravity(void);
			
			/**
			 * \brief Returns the ID of the body struct created by ODE.
			 *
			 * For internal use only.
			 */
			dBodyID getBody(void);
			
			//tolua_end
			/**
			 * \brief Updates the body.
			 *
			 * For internal use only, applies all forces once per simulation step.
			 */
			void update(void);
			/**
			 * \brief Updates all bodies.
			 *
			 * For internal use only.
			 */
			static void updateAll(void);
		private:
			dBodyID body;
			Entity *entity;
			Vector3D absforce;
			Vector3D relforce;
			
			static std::list<Body*> bodies;
			//tolua_begin
	};
}
//tolua_end

#endif


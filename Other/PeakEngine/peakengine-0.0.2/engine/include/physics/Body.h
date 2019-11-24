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

#ifndef _BODY_H_
#define _BODY_H_

#include "core/GameObject.h"
#include "core/Vector3D.h"
#include "core/Quaternion.h"

#include <vector>

class btRigidBody;
class btDefaultMotionState;

//tolua_begin
namespace peak
{
	class World;
	class Shape;
	class Entity;
	
	/**
	 * \brief Rigid body for the physics simulation
	 *
	 * The body has the properties "position" (Vector3D) and "rotation" (Quaternion)
	 * and the event "Collision".
	 */
	class Body : public GameObject
	{
		public:
			Body();
			~Body();
			
			/**
			 * Creates a body and places it into the given world
			 * \param world World to place to body into
			 * \param shape Shape to connect to the body
			 */
			bool create(World *world, Shape *shape);
			/**
			 * \brief Destroys the body
			 */
			bool destroy(void);
			
			/**
			 * \brief Sets the position of the body
			 */
			void setPosition(Vector3D position);
			/**
			 * \brief Returns the position of the body
			 */
			Vector3D getPosition(void);
			
			/**
			 * \brief Sets the rotaiton of the body
			 */
			void setRotation(Quaternion rotation);
			/**
			 * \brief Returns the rotation of the body
			 */
			Quaternion getRotation(void);
			
			/**
			 * \brief Applies a force to the body
			 * \brief force Force to apply
			 * \brief relativeposition Point to apply the force at (relative to body)
			 */
			void setForce(Vector3D force, Vector3D relativeposition = Vector3D(0, 0, 0));
			/**
			 * \brief Clears all applied forces
			 */
			void clearForces(void);
			
			/**
			 * \brief Sets the linear velocity of the body
			 */
			void  setLinearVelocity(Vector3D linearvel);
			/**
			 * \brief Returns the linear velocity of the body
			 */
			Vector3D getLinearVelocity(void);
			
			/**
			 * \brief Sets the angular velocity of the body
			 */
			void  setAngularVelocity(Vector3D angularvel);
			/**
			 * \brief Returns the angular velocity of the body
			 */
			Vector3D getAngularVelocity(void);

			/**
			 * \brief Sets the gravity which affects the body
			 */
			void setGravity(Vector3D gravity = Vector3D(0, -9.81, 0));
			/**
			 * \brief Returns the gravity which affects the body
			 */
			Vector3D getGravity(void);

			/**
			 * \brief Sets the network ID of the body
			 */
			void setID(int id);
			/**
			 * \brief Returns the network ID of the body
			 */
			int getID(void);
			
			/**
			 * \brief Returns the body with which this body collided
			 *
			 * This can only be called directly after a collision event
			 */
			Body *getCollisionBody(void);
			
			/**
			 * \brief Connects an entity to the body
			 */
			void setEntity(Entity *entity);
			/**
			 * \brief Gets the entity connected to this body
			 */
			Entity *getEntity(void);
			
			//tolua_end
			/**
			* \brief Updates the body.
			*
			* For internal use only.
			*/
			void update(void);
			/**
			* \brief Updates all bodies.
			*
			* For internal use only.
			*/
			static void updateAll(void);

			/**
			 * \brief Returns all bodies
			 */
			static std::vector<Body*> getBodies(void);
			
			/**
			 * \brief Collision callback
			 */
			void collideWith(Body *other);
			
			/**
			 * \brief Returns the Bullet body.
			 */
			btRigidBody *getBody();
			
			virtual void onPropertyChanged(std::string name);
		private:
			btRigidBody *body;
			World *world;
			btDefaultMotionState *state;
			
			Event *collisionevent;
			Body *collisionbody;
			
			Entity *entity;
			
			static std::vector<Body*> bodies;
			
			int networkid;
			//tolua_begin
	};
}
//tolua_end

#endif

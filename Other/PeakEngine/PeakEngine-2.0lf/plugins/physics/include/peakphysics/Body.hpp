/*
Copyright (c) 2009, Mathias Gottschlag, Christian Reiser

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

#ifndef _PEAKPHYSICS_BODY_HPP_
#define _PEAKPHYSICS_BODY_HPP_

#include <peakengine/support/Vector3.hpp>
#include <peakengine/support/Quaternion.hpp>

class btRigidBody;
class btDefaultMotionState;
class btTransform;

namespace peak
{
	class Physics;
	class Shape;

	class Body
	{
		public:
			Body();
			~Body();

			bool init(Physics *physics, Shape *shape,
				bool autodeactivation = true);
			bool destroy();

			/**
			 * Sets the position of the body.
			 */
			void setPosition(const Vector3F &position);
			/**
			 * Returns the position of the body.
			 */
			Vector3F getPosition();

			/**
			 * Sets the rotation of the body.
			 */
			void setRotation(const Quaternion &rotation);
			/**
			 * Sets the rotation of the body.
			 */
			void setRotation(const Vector3F &rotation);
			/**
			 * Returns the rotation of the body.
			 */
			Quaternion getRotation();
			/**
			 * Sets the linear velocity of the body.
			 */
			void setLinearVelocity(const Vector3F &linearvel);
			/**
			 * Returns the linear velocity of the body.
			 */
			Vector3F getLinearVelocity();

			/**
			 * Sets the angular velocity of the body.
			 */
			void setAngularVelocity(const Vector3F &angularvel);
			/**
			 * Returns the angular velocity of the body.
			 */
			Vector3F getAngularVelocity();

			/**
			 * Sets the gravity which affects the body.
			 */
			void setGravity(const Vector3F &gravity = Vector3F(0, -9.81, 0));
			/**
			 * Returns the gravity which affects the body.
			 */
			Vector3F getGravity();

			/**
			 * Attaches a user defined pointer to the body.
			 */
			void setUserData(void *data);
			/**
			 * Returns a pointer to user data.
			 */
			void *getUserData();

			/**
			 * Returns the Bullet body.
			 */
			btRigidBody *getBody();

			/**
			 * Returns the bullet transformation information which includes
			 * position/rotation. Only used internally, e.g. in
			 * CharacterController.
			 */
			btTransform &getTransform();

			/**
			 * Returns the shape.
			 */
			Shape *getShape();
		private:
			btRigidBody *body;
			Physics *physics;
			btDefaultMotionState *state;
			Shape *shape;
			bool autodeactivation;
			void *userdata;
	};
}

#endif

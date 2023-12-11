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

#ifndef _PEAKPHYSICS_PHYSICS_HPP_
#define _PEAKPHYSICS_PHYSICS_HPP_

#include <peakengine/support/Vector3.hpp>

#include <list>

class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btDynamicsWorld;
class btSequentialImpulseConstraintSolver;
class btAxisSweep3;

namespace peak
{
	class Body;

	/**
	 * Info about the outcome of a ray cast test or any other collision test.
	 */
	struct CollisionInfo
	{
		/**
		 * For ray casts, leads to the point where the ray hit any geometry.
		 * This point is start + lambda * (end - start).
		 */
		float lambda;
		/**
		 * The point where the collision happened.
		 */
		Vector3F point;
		/**
		 * The body which belongs to the geometry which caused the collision.
		 */
		Body *body;
	};

	/**
	 * Main class for a physics simulation. Holds bodies and can perform
	 * collision checks. update() has to be called once per game logic step
	 * (every 20 ms).
	 */
	class Physics
	{
		public:
			/**
			 * Constructor.
			 */
			Physics();
			/**
			 * Destructor.
			 */
			~Physics();

			/**
			 * Initializes the simulation.
			 */
			bool init();
			/**
			 * Destroys the simulation.
			 */
			bool shutdown();

			/**
			 * Adds a body to the world.
			 */
			void addBody(Body *body);
			/**
			 * Removes a body from the world.
			 */
			void removeBody(Body *body);

			/**
			 * Performs a ray cast collision test.
			 * @param from Start of the ray.
			 * @param to End of the ray.
			 * @param info If not 0, the variable will be filled with the
			 * collision information.
			 * @return True if a shape was hit by the ray.
			 */
			bool castRay(Vector3F from, Vector3F to, CollisionInfo *info = 0,
				Body *exclude = 0);

			/**
			 * Returns the Bullet world. Can be used e.g. for manual collision
			 * checking.
			 */
			btDynamicsWorld *getWorld();

			/**
			 * Moves the simulation forward in time for 20 ms. Moves all bodies
			 * according to the forces and performs collision checks.
			 */
			void update();
		private:
			btDefaultCollisionConfiguration *config;
			btCollisionDispatcher *dispatcher;
			btSequentialImpulseConstraintSolver *solver;
			btDynamicsWorld *world;
			btAxisSweep3 *worldbox;
	};
}

#endif

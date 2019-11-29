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

#ifndef _JOINT_H_
#define _JOINT_H_

#include <ode/ode.h>

//tolua_begin
namespace peak
{
	class Body;
	class World;
	
	/**
	 * \brief Class representing a joint connecting two bodies.
	 */
	class Joint
	{
		public:
			Joint();
			virtual ~Joint();
			
			/**
			 * \brief Creates a joint and places it in the given world.
			 *
			 * This is implemented by the joint classes.
			 */
			virtual bool init(World *world) = 0;
			/**
			 * \brief Destroys the joint.
			 */
			virtual bool destroy(void);
			
			/**
			 * \brief Sets the first body to connect the joint to.
			 * \param body1 First body
			 */
			virtual void setBody1(Body *body1);
			/**
			 * \brief Sets the second body to connect the joint to.
			 * \param body2 Second body
			 */
			virtual void setBody2(Body *body2);
			
			//tolua_end
			/**
			 * \brief Returns the joint struct used by ODE.
			 */
			virtual dJointID getJoint(void);
		protected:
			dJointID joint;
			Body *bodies[2];
			//tolua_begin
	};
}
//tolua_end

#endif


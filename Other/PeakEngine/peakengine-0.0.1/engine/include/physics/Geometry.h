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

#ifndef _GEOMETRY_H_
#define _GEOMETRY_H_

#include <ode/ode.h>
#include <string>

#include "core/Vector3D.h"

//tolua_begin
namespace peak
{
	class World;
	class Body;
	class ServerObject;
	class Script;
	
	struct CollisionCallback
	{
		Script *script;
		std::string functionname;
	};
	
	struct CollisionInfo
	{
		float friction;
		float bounce;
		float bouncevelocity;
		
		CollisionCallback callback1;
		CollisionCallback callback2;
	};
	
	/**
	 * \brief Base class for all geometry classes.
	 */
	class Geometry
	{
		public:
			Geometry();
			virtual ~Geometry();
			
			/**
			 * \brief Destroys the geometry.
			 * \return Returns false if an error occurred.
			 */
			virtual bool destroy(void);
			
			/**
			 * \brief Sets the position of the geometry.
			 *
			 * If the geometry is attached to a body, the position is relative to it.
			 * \param pos New position
			 */
			virtual void setPosition(Vector3D pos);
			
			/**
			 * \brief Attaches the geometry to a body.
			 * \param body New body
			 */
			virtual void setBody(Body *body);
			Body *getBody(void)
			{
				return body;
			}
			
			CollisionInfo *getCollisionInfo(void)
			{
				return &collinfo;
			}
			
			/**
			 * \brief Returns the ODE geom ID.
			 *
			 * For internal use only.
			 * \return ODE geom ID
			 */
			virtual dGeomID getGeom(void);
		protected:
			//tolua_end
			dGeomID geom;
			dGeomID transfgeom;
			ServerObject *object;
			CollisionInfo collinfo;
			Body *body;
			//tolua_begin
	};
}
//tolua_end

#endif


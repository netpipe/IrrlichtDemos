/*
Copyright (C) 2008   Christian Reiser

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

#ifndef _RAY_CALLBACK_H_
#define _RAY_CALLBACK_H_

#include "physics/Callback.h"
#include "core/Vector3D.h"

#include <string>

//tolua_begin
namespace peak
{
	class Body;
	class Script;

	/**
	 * \brief Structure which contains infos about collisions
	 * 
	 * A callback gets this structure as parameter, so the collision
	 * info is avaible in the script, too.
	 *
	 */
	struct CollisionInfo
	{
		CollisionInfo()
		{
			lambda = 0;
			object = 0;
		}
		/**
		 * \brief (Only for rays) that's the way from the ray origin to the collision point.
		 * 
		 */
		float lambda;		
		/**
		 * \brief The position where the collision has occured.
		 *
		 */
		Vector3D point;
		/**
		 * \brief The object with which the ray/convex intersects.
		 *
		 * That's very useful to findout with which entity the ray/convex has intersect.
		 * Use Body::getEntity to finally find out.
		 *
		 */
		Body *object;
	};

	/**
	 * \brief Class for custom ray callbacks.
	 *
	 * Example:
	 * \code
	 * (lua_start)
	 * -- We need a a script, in which the callback will be called.
	 * samplescript = entity:getScript();
	 * 
	 * raycallback = peak.RayCallback:new();
	 * raycallback:create(peak.Vector3D(0, 1, 0), peak.Vector3D(0, -1, 0), samplescript, MyRayCallback);
	 * 
	 * -- This is our callback function, it ll be called when the ray had intersected.
	 * function MyRayCallback()
	 * 	print("The Ray had intersected!!");
	 * 	-- That'll print "The Ray had intersected!!" every time in the console when the ray had intersected(logical!).
	 * (lua_end)
	 * \endcode
	 *
	 * You see: It's very easy to create a custom ray callback and you need that for very many things.
	 *
	 */
	class RayCallback : public Callback
	{
		public:
			RayCallback();
			virtual ~RayCallback();

			/**
			 * \brief Creates a custom ray callback and defines it.
			 * 
			 * \param rayto The Point where the ray ends.
			 * \param rayfrom The Point where the ray starts.
			 * \param script The Script where the callback function will be called.
			 * \param callbackname The name of the callback in the script. This function will be called when the ray had intersected.
			 * 
			 */
			bool create(Vector3D rayto, Vector3D rayfrom, Script *script, std::string callbackname);
			/**
			 * \brief Destroys the callback.
			 * 
			 * When this function is called the callback won't be called again.
			 */
			bool destroy();

			/**
			 * \brief Updates the callback, for internal use only.
			 */
			void update();

			/**
			 * \brief Returns true when the ray had intersected and false if not.
			 */
			bool isCollision();

			/**
			 * \brief Sets the ray start position AND end position new.
			 * 
			 * \param rayto The point where the ray ends.
			 * \param rayfrom The point where the ray starts.
			 * 
			 * If you only want to change one of the both components you can do something like that:
			 * \code
			 * (lua_start)
			 * raycallback:setRay(peak.Vector3D(1, 0, 0), raycallback:getRay(1));
			 * (lua_end)
			 * \endcode
			 */
			void setRay(Vector3D rayto, Vector3D rayfrom);
			/**
			 * \brief Gets the start position OR the end position of the ray.
			 * 
			 * \param index If index is true the end position is returned and if index is false the start positon is returned.
			 * 
			 * \return The return value depends on the "index" parameter. For more info see parameter documentation.
			 */
			Vector3D getRay(bool index);

			/**
			 * \brief Sets the collision info.
			 */
			void setInfo(CollisionInfo info);
			/**
			 * \brief Gets the collision info.
			 */
			CollisionInfo getInfo(void);
		private:
			//tolua_end
			std::string callbackname;
			Script *script;

			Vector3D rayto;
			Vector3D rayfrom;

			CollisionInfo info;
			bool collision;
			//tolua_begin
	};
}
//tolua_end

#endif

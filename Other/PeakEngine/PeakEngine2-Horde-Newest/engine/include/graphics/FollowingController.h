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

#ifndef _FOLLOWINGCONTROLLER_H_
#define _FOLLOWINGCONTROLLER_H_

#include "graphics/SceneNodeController.h"
#include "core/Vector3D.h"

//tolua_begin
namespace peak
{
	/**
	 * \brief Possible modes for FollowingController
	 *
	 * The modes can be or'd together.
	 */
	enum FollowingControllerMode
	{
		EFCM_PositionApplyX = 0x1,
		EFCM_PositionApplyY = 0x2,
		EFCM_PositionApplyZ = 0x4,
		EFCM_PositionApplyAll = 0x7,
		EFCM_RotationApplyX = 0x8,
		EFCM_RotationApplyY = 0x10,
		EFCM_RotationApplyZ = 0x20,
		EFCM_RotationApplyAll = 0x38
	};
	
	/**
	 * \brief Class which sets a scene node to a fixed distance to another scene
	 * node.
	 */
	class FollowingController : public SceneNodeController
	{
		public:
			FollowingController();
			virtual ~FollowingController();
			
			/**
			 * \brief Sets the target scene node
			 * \param target Target scene node
			 */
			void setTarget(SceneNode *target);
			/**
			 * \brief Returns the target scene node
			 */
			SceneNode *getTarget(void);
			
			/**
			 * \brief Sets the distance of the scene node
			 * \param distance New distance
			 */
			void setDistance(Vector3D distance);
			/**
			 * \brief Returns the distance of the scene node
			 */
			Vector3D getDistance(void);
			/**
			 * \brief Sets the relative rotation of the scene node
			 * \param rotation New rotation
			 */
			void setRotation(Vector3D rotation);
			/**
			 * \brief Returns the relative rotation
			 */
			Vector3D getRotation(void);
			
			/**
			 * \brief Sets the mode of the controller
			 *
			 * Default is EFCM_PositionApplyAll | EFCM_RotationApplyAll.
			 * \param mode New mode. Can be a combination of the values of
			 * FollowingControllerMode.
			 */
			void setMode(FollowingControllerMode mode);
			/**
			 * \brief Returns the mode of the controller
			 */
			FollowingControllerMode getMode(void);
			
			//tolua_end
			virtual void doWork(float msecs);
		private:
			SceneNode *target;
			Vector3D distance;
			FollowingControllerMode mode;
			Vector3D rotation;
			//tolua_begin
	};
}
//tolua_end

#endif


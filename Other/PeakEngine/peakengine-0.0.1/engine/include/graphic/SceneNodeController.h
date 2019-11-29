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

#ifndef _SCENENODECONTROLLER_H_
#define _SCENENODECONTROLLER_H_

#include <list>

//tolua_begin
namespace peak
{
	class SceneNode;
	
	/**
	 * \brief Class which automatically manipulates position and rotation of a
	 * SceneNode.
	 */
	class SceneNodeController
	{
		public:
			SceneNodeController();
			virtual ~SceneNodeController();
			
			/**
			 * \brief Sets the scene node which is controlled.
			 */
			virtual void setSceneNode(SceneNode *node)
			{
				this->node = node;
			}
			/**
			 * \brief Returns the scene node attached to the controller.
			 */
			virtual SceneNode *getSceneNode(void)
			{
				return node;
			}
			
			/**
			 * \brief Updates all controllers.
			 *
			 * This is called by the engine once every frame.
			 */
			static void doAllWork(float msecs);
			virtual void doWork(float msecs) = 0;
			//tolua_end
		protected:
			SceneNode *node;
			
			static std::list<SceneNodeController*> controllers;
			//tolua_begin
	};
}
//tolua_end

#endif


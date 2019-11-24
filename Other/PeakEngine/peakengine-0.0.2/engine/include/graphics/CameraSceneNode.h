/*
Copyright (C) 2008   Christian Reiser, Mathias Gottschlag

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

#ifndef _CAMERASCENENODE_H_
#define _CAMERASCENENODE_H_

#include "graphics/SceneNode.h"


//tolua_begin
namespace peak
{
	class CameraSceneNode : public SceneNode
	{
		public:
			CameraSceneNode(SceneNode *parent = 0);
			virtual ~CameraSceneNode();
		
			void setNearValue(float nearvalue);
			float getNearValue(void);
			void setFarValue(float nearvalue);
			float getFarValue(void);
			
			void setOrthogonal(bool orthogonal);
			bool isOrthogonal(void);
			
			void setFOV(float fov);
			float getFOV(void);
			
			void setAspectRatio(float ratio);
			float getAspectRatio(void);
			
			void render(void);
			
			//tolua_end
			static void updateAllCameras(void);
			void updateCamera(void);
		private:
			static std::vector<CameraSceneNode*> cameras;
			//tolua_begin
	};
}
//tolua_end

#endif

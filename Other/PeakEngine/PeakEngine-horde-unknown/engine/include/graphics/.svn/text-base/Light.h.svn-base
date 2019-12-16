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

#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "graphics/SceneNode.h"
#include "core/Vector2D.h"
#include "core/Color.h"

#include <string>

namespace irr
{
	namespace scene
	{
		class ILightSceneNode;
	}
}

// tolua_begin	
namespace peak
{
	enum LightType
	{
		ELT_Point,
		ELT_Direction,
		ELT_Spot
	};
	
	class Light : public SceneNode
	{
		public:
			Light(std::string name, std::string material = "light.scene.xml", SceneNode *parent = 0);
			~Light();
			
			void setRadius(float radius);
			float getRadius();
			
			void setColor(Color color);
			Color getColor();
			
			void setShadowMapCount(int count);
			int getShadowMapCount();
			
			void setShadowSplitLambda(float splitlambda);
			float getShadowSplitLambda();
			
			void setShadowMapBias(float bias);
			float getShadowMapBias();	
			
			void setType(LightType type);
			LightType getType(void);
			
			void setFOV(float fieldofview);
			float getFOV();
		
			
			// tolua_end
		private:
			float radius;
			Color color;
			int count;
			float splitlambda;
			float bias;
			float fov;
			// tolua_begin	
	};
}
// tolua_end
	
#endif


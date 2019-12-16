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

#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include <string>
#include <vector>

#include "core/Vector2D.h"

//tolua_begin
namespace peak
{
	enum MaterialType
	{
		EMT_Solid,
		EMT_VertexAlpha,
		EMT_AlphaChannel,
		EMT_Add,
		EMT_DetailMap,
		EMT_ParallaxMap,
		EMT_NormalMap
	};
	
	class Material
	{
		public:
			Material();
			~Material();
			
			bool load(std::string name);
			
			void setType(MaterialType type);
			MaterialType getType(void);
			
			void setTextureCount(int count);
			int getTextureCount(void);
			void setTexture(int index, std::string texture);
			std::string getTexture(int index);
			void setTextureScale(int index, Vector2D scale);
			Vector2D getTextureScale(int index);
			
			void setWireframe(bool wireframe);
			bool isWireframe(void);
			
			void setLighting(bool lighting);
			bool getLighting(void);
			
			void setBackfaceCulling(bool culling);
			bool getBackfaceCulling(void);
			
			void setWriteZBuffer(bool writez);
			bool getWriteZBuffer(void);
		private:
			//tolua_end
			MaterialType type;
			int texcount;
			std::vector<std::string> textures;
			std::vector<Vector2D> texturescale;
			bool wireframe;
			bool lighting;
			bool backfaces;
			bool writez;
			//tolua_begin
	};
}
//tolua_end

#endif


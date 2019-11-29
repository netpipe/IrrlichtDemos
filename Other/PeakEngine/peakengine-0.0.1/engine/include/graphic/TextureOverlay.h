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

#ifndef _TEXTUREOVERLAY_H_
#define _TEXTUREOVERLAY_H_

#include "graphic/Overlay.h"
#include "core/Color.h"

#include <string>

#ifdef _USE_IRRLICHT_
namespace irr
{
	namespace video
	{
		class S3DVertex;
		class ITexture;
	}
}
#endif

//tolua_begin
namespace peak
{
	class TextureOverlay : public Overlay
	{
		public:
			TextureOverlay(std::string filename, Overlay *parent = 0);
			virtual ~TextureOverlay();
			
			virtual void notifySizeChanged(void);
			virtual bool render(void);
		private:
			//tolua_end
			std::string texture;
			
			#ifdef _USE_IRRLICHT_
			irr::video::S3DVertex *vertices;
			irr::video::ITexture *textureptr;
			static unsigned short indices[6];
			#endif
			//tolua_begin
	};
}
//tolua_end

#endif


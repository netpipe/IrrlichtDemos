/*
Copyright (C) 2008   Lukas Kropatschek

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

#ifndef _IMAGE_H_
#define _IMAGE_H_

#include "core/Vector2DI.h"
#include "core/Color.h"

#include <string>

#ifdef _USE_IRRLICHT_
namespace irr
{
	namespace video
	{
		class IImage;
	}
}
#endif

#ifdef _USE_LIGHTFEATHER_
namespace lf
{
	namespace res
	{
		class CImage;
	}
}
#endif

//tolua_begin
namespace peak
{
	/**
	 * \brief Class representing a image
	 */
	class Image
	{
		public:
			Image();
			Image(std::string imagename);
			/**
			 * \brief Returns the dimension of the image as Vector2D
			 * \return dimension as Vector2D
			 */
			Vector2DI getDimension(void);

			/**
			 * \brief Loads the image
			 * \param imagename The filename of the image.
			 * \return true if the image was successfully loaded.
			 */
			bool load(std::string imagename);
			
			/**
			 * \brief Returns the color of the pixel.
			 * \param pixel The pixel of which the color should be returned.
			 * \return color of the pixel.
			 */
			Color getPixel(Vector2DI pixel);
			
			/**
			 * \brief Returns the color of the pixel.
			 * \param x x position of the pixel.
			 * \param y y position of the pixel.
			 * \return color of the pixel.
			 */
			Color getPixel(float x, float y);
			//tolua_end
			#ifdef _USE_IRRLICHT_
			irr::video::IImage *getImage(void);
			#endif

			#ifdef _USE_LIGHTFEATHER_
			lf::res::CImage *getImage(void);
			#endif
		private:
			#ifdef _USE_IRRLICHT_
			irr::video::IImage *image;
			#endif

			#ifdef _USE_LIGHTFEATHER_
			lf::res::CImage *image;
			#endif
			//tolua_begin
	};

}
//tolua_end
#endif /*IMAGE_H_*/

/*
Copyright (C) 2008   Mathias Gottschlag

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

#include "graphics/Image.h"
#include "graphics/GraphicsEngine.h"
#include "core/Logger.h"
#include "core/FileSystem.h"

#include <FreeImagePlus.h>

namespace peak
{
	
	Image::Image()
	{
		image = new fipImage;
	}
	Image::~Image()
	{
		delete image;
	}
	
	Image::Image(std::string imagename)
	{
		image = new fipImage;
		load(imagename);
	}
	
	Vector2DI Image::getDimension(void)
	{
		return Vector2DI(image->getWidth(), image->getHeight());;
	}
	
	bool Image::load(std::string imagename)
	{
		std::string file = FileSystem::get()->getFileExists(imagename.c_str());
		if (file == "")
		{
			LERROR("Image not found: %s\n", imagename.c_str());
			return false;
		}
		if (!image->load(file.c_str()))
		{
			LERROR("Couldn't load image: %s\n", file.c_str());
			return false;
		}
		
		return true;
	}
	
	Color Image::getPixel(float x, float y)
	{
		RGBQUAD color = {0, 0, 0, 0};
		if (image)
		{
			image->getPixelColor(x, y, &color);
		}
		Color ret(color.rgbRed, color.rgbGreen, color.rgbBlue, color.rgbReserved);
		return ret;
	}
	
	Color Image::getPixel(Vector2DI pixel)
	{
		RGBQUAD color = {0, 0, 0, 0};
		if (image)
		{
			image->getPixelColor(pixel.x, pixel.y, &color);
		}
		Color ret(color.rgbRed, color.rgbGreen, color.rgbBlue, color.rgbReserved);
		return ret;

	}
}

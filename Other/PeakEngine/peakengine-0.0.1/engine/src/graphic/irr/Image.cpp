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

#include "graphic/Image.h"
#include "graphic/GraphicsEngine.h"
#include "core/Logger.h"
#include "core/FileSystem.h"

#include <irrlicht.h>

namespace peak
{

	Image::Image()
	{
		image = 0;
	}

	Image::Image(std::string imagename)
	{
		std::string file = FileSystem::get()->getFileExists(imagename.c_str());
		image = GraphicsEngine::get()->getDriver()->createImageFromFile(file.c_str());
	}

	Vector2DI Image::getDimension(void)
	{
		Vector2DI ret;
		ret.x = image->getDimension().Width;
		ret.y = image->getDimension().Height;
		return ret;
	}

	bool Image::load(std::string imagename)
	{
		std::string file = FileSystem::get()->getFileExists(imagename.c_str());
		image = GraphicsEngine::get()->getDriver()->createImageFromFile(file.c_str());
		if(!image)
		{
			LERROR("Couldn't load image: %s \n", file.c_str());
			return false;
		}

		return true;
	}

	Color Image::getPixel(float x, float y)
	{
		Color ret;
		irr::video::SColor col = image->getPixel(x,y);
		ret.setAlpha(col.getAlpha());
		ret.setRed(col.getRed());
		ret.setGreen(col.getGreen());
		ret.setBlue(col.getBlue());

		return ret;
	}

	Color Image::getPixel(Vector2DI pixel)
	{
		Color ret;
		irr::video::SColor col = image->getPixel(pixel.x, pixel.y);
		ret.setAlpha(col.getAlpha());
		ret.setRed(col.getRed());
		ret.setGreen(col.getGreen());
		ret.setBlue(col.getBlue());

		return ret;
	}

	irr::video::IImage *Image::getImage(void)
	{
		return image;
	}

}

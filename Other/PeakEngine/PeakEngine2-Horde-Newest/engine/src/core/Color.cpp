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

#include "core/Color.h"

#include <cstring>

namespace peak
{
	
	Color::Color()
	{
	}
	
	Color::Color(unsigned int color)
	{
		memcpy(this->color, &color, 4);
	}
	
	Color::Color(unsigned char r, unsigned char g,
				 unsigned char b, unsigned char a)
	{
		color[3] = a;
		color[2] = r;
		color[1] = g;
		color[0] = b;
	}

	unsigned char Color::getAlpha(void)
	{
		return color[3];
	}
	
	unsigned char Color::getRed(void)
	{
		return color[2];
	}
	
	unsigned char Color::getGreen(void)
	{
		return color[1];		
	}
	
	unsigned char Color::getBlue(void)
	{
		return color[0];
	}
	
	void Color::setRed(unsigned char red)
	{
		color[2] = red;
	}
	
	void Color::setGreen(unsigned char green)
	{
		color[1] = green;
 	}
	
	void Color::setBlue(unsigned char blue)
	{
		color[0] = blue;
	}
	
	void Color::setAlpha(unsigned char alpha)
	{
		color[3] = alpha;
	}
	
	void Color::set(unsigned int color)
	{
		memcpy(this->color, &color, 4);
	}
	
	void Color::set(unsigned char r, unsigned char g,
					unsigned char b, unsigned char a)
	{
		color[3] = a;
		color[2] = r;
		color[1] = g;
		color[0] = b;
	}
	
	unsigned int Color::get()
	{
		unsigned int color;
		memcpy(&color, this->color, 4);
		return color;
	}
}


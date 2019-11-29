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

#include "menu/MenuElement.h"
#include "graphic/GraphicsEngine.h"
#include "core/Vector2DI.h"
#include "core/Util.h"

#ifdef _USE_IRRLICHT_
#include <irrlicht.h>
using namespace irr;
#endif

namespace peak
{
	bool MenuElement::destroy(void)
	{
		return false;
	}

	void MenuElement::setPosition(Vector2D position)
	{
		this->position = position;
		#ifdef _USE_IRRLICHT_
		if (element)
		{
			Vector2D size = getSize();
			element->setRelativePosition(core::rect<int>(position.x, position.y, position.x + size.x, position.y + size.y));
		}
		#endif
	}
	Vector2D MenuElement::getPosition(void)
	{
		return position;
	}

	void MenuElement::setSize(Vector2D size)
	{
		this->size = size;
		#ifdef _USE_IRRLICHT_
		if (element)
		{
			Vector2D position = getPosition();
			element->setRelativePosition(core::rect<int>(position.x, position.y, position.x + size.x, position.y + size.y));
		}
		#endif
	}
	Vector2D MenuElement::getSize(void)
	{
		return size;
	}

	void MenuElement::setID(int id)
	{
		this->id = id;
		#ifdef _USE_IRRLICHT_
		if (element)
		{
			element->setID(id);
		}
		#endif
	}
	int MenuElement::getID(void)
	{
		return id;
	}

	void MenuElement::setText(std::string text)
	{
		this->text = text;
		#ifdef _USE_IRRLICHT_
		if (element)
		{
			element->setText(toWString(text).c_str());
		}
		#endif
	}
	std::string MenuElement::getText(void)
	{
		return text;
	}
}


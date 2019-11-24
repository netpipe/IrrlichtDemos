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
#include "graphics/GraphicsEngine.h"
#include "core/Vector2DI.h"
#include "core/Util.h"
#include "core/SettingsManager.h"

#include "guichan.hpp"

namespace peak
{
	bool MenuElement::destroy(void)
	{
		return false;
	}
	
	void MenuElement::setPosition(Vector2D position)
	{
		this->position = position;
		if (widget)
		{
			Vector2DI windowsize = SettingsManager::get()->getVector2DI("window.resolution");
			widget->setPosition(position.x * windowsize.x, position.y * windowsize.y);
		}
	}
	Vector2D MenuElement::getPosition(void)
	{
		return position;
	}
	
	void MenuElement::setSize(Vector2D size)
	{
		this->size = size;
		if (widget)
		{
			Vector2DI windowsize = SettingsManager::get()->getVector2DI("window.resolution");
			if (size.x * windowsize.x != 0)
			{
				widget->setWidth(size.x * windowsize.x);
			}
			if (size.y * windowsize.y != 0)
			{
				widget->setHeight(size.y * windowsize.y);
			}
		}
	}
	Vector2D MenuElement::getSize(void)
	{
		return size;
	}
	
	void MenuElement::setID(int id)
	{
		this->id = id;
		if (widget)
		{
			widget->setId(toString(id));
		}
	}
	int MenuElement::getID(void)
	{
		return id;
	}
}


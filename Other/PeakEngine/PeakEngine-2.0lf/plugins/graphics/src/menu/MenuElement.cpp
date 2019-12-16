/*
Copyright (c) 2009, Mathias Gottschlag

Permission to use, copy, modify, and/or distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#include "peakgraphics/menu/MenuElement.hpp"
#include "peakgraphics/menu/Menu.hpp"
#include "peakgraphics/Graphics.hpp"
#include <peakengine/support/ScopedLock.hpp>

#include <lf/Lightfeather.h>
using namespace lf;

namespace peak
{
	MenuElement::MenuElement(Graphics *graphics, Menu *menu, MenuElement *parent)
		: graphics(graphics), menu(menu), newparent(parent), parent(0),
		widget(0), visible(true), changed(false)
	{
	}
	MenuElement::~MenuElement()
	{
	}

	void MenuElement::queueForLoading()
	{
		graphics->registerLoading(this);
	}

	bool MenuElement::load()
	{
		return false;
	}
	bool MenuElement::destroy()
	{
		if (widget)
			widget->drop();
		return true;
	}

	void MenuElement::setPosition(ScreenPosition position)
	{
		this->position = position;
		changed = true;
	}
	ScreenPosition MenuElement::getPosition()
	{
		return position;
	}

	void MenuElement::setSize(ScreenSize size)
	{
		this->size = size;
	}
	ScreenSize MenuElement::getSize()
	{
		return size;
	}

	void MenuElement::setVisible(bool visible)
	{
		this->visible = visible;
		changed = true;
	}
	bool MenuElement::isVisible()
	{
		return visible;
	}

	void MenuElement::updateParent()
	{
		ScopedLock lock(mutex);
		// We have to make sure the widget does not get deleted
		grab();
		if (widget)
			widget->grab();
		// Remove the node from the parent node
		if (parent)
			parent->removeChild(this);
		else
			menu->removeRootElement(this);
		// Add to the new parent
		parent = newparent;
		if (parent)
		{
			parent->children.push_back(this);
			if (parent->widget && widget)
				parent->widget->addChild(widget);
		}
		else
		{
			menu->addRootElement(this);
			if (widget)
				menu->getGUIManager()->addChild(widget);
		}
		newparent = 0;
		if (widget)
			widget->drop();
		drop();
	}
	void MenuElement::updatePosition()
	{
		if (!widget)
			return;
		// Get absolute position/size in pixels
		Vector2I screensize = graphics->getWindowSize();
		Vector2I abspos = position.getAbsolute(screensize);
		Vector2I abssize = size.getAbsolute(screensize);
		// Set size/position
		widget->setPosition(abspos.x, abspos.y);
		widget->setSize(abssize.x, abssize.y);
	}
	void MenuElement::update()
	{
		if (widget && changed)
		{
			// Apply position, size
			updatePosition();
			// Update visibility
			widget->setVisible(visible);
			// Reset change flag
			changed = false;
		}
		// Update children
		for (unsigned int i = 0; i < children.size(); i++)
		{
			children[i]->update();
		}
	}

	lf::gui::CGUIWidget *MenuElement::getWidget()
	{
		return widget;
	}

	void MenuElement::removeChild(MenuElement *child)
	{
		for (unsigned int i = 0; i < children.size(); i++)
		{
			if (children[i].get() == child)
			{
				if (widget && child->widget)
					widget->removeChild(widget);
				children.erase(children.begin() + i);
				return;
			}
		}
	}
}
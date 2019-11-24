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

#include "peakgraphics/menu/MenuButton.hpp"
#include "peakgraphics/menu/Menu.hpp"
#include "peakgraphics/menu/MenuInputListener.hpp"
#include <peakengine/support/ScopedLock.hpp>

#include <lf/Lightfeather.h>
using namespace lf;

namespace peak
{
	MenuButton::MenuButton(Graphics *graphics, std::string caption, Menu *menu,
		MenuElement *parent) : MenuElement(graphics, menu, parent),
		caption(caption)
	{
	}
	MenuButton::~MenuButton()
	{
	}

	bool MenuButton::load()
	{
		ScopedLock lock(mutex);
		// Create button
		widget = new gui::CGUIButton(core::stringw(caption.c_str()).c_str(), 1, 1);
		widget->setVisible(visible);
		// Set input handler
		menu->registerInput(this);
		((gui::CGUIButton*)widget)->addActionListener(menu->getInputListener());
		// Set parent
		if (parent)
		{
			if (parent->getWidget())
				parent->getWidget()->addChild(widget);
		}
		else
		{
			menu->addRootElement(this);
			menu->getGUIManager()->addChild(widget);
		}
		// Update position
		updatePosition();
		return true;
	}
	bool MenuButton::destroy()
	{
		if (widget)
			menu->deregisterInput(this);
		return MenuElement::destroy();
	}
}

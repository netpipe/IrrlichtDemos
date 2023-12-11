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

#include "peakgraphics/menu/MenuImage.hpp"
#include "peakgraphics/menu/Menu.hpp"
#include <peakengine/support/ScopedLock.hpp>

#include <lf/Lightfeather.h>
using namespace lf;

namespace peak
{
	MenuImage::MenuImage(Graphics *graphics, std::string file, Menu *menu,
		MenuElement *parent) : MenuElement(graphics, menu, parent),
		filename(file)
	{
	}
	MenuImage::~MenuImage()
	{
	}

	bool MenuImage::load()
	{
		ScopedLock lock(mutex);
		// Load texture
		CResourceManager *resmgr = CResourceManager::getInstancePtr();
		res::CTexture *texture = resmgr->loadTexture(filename.c_str(),
			filename.c_str());
		if (!texture)
			return false;
		// Create button
		widget = new gui::CGUIImage(texture);
		widget->setVisible(visible);
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
}

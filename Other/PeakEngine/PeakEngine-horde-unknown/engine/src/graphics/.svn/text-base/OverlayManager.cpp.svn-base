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

#include "graphics/OverlayManager.h"
#include "graphics/Overlay.h"

namespace peak
{
	OverlayManager *OverlayManager::get(void)
	{
		static OverlayManager overlaymgr;
		return &overlaymgr;
	}
	OverlayManager::~OverlayManager()
	{
	}
	
	bool OverlayManager::init(void)
	{
		// Initialize root overlay with 0 as parent
		Overlay *root = new Overlay();
		rootoverlay = root;
		root->setSize(Vector2D(1, 1));
		return true;
	}
	bool OverlayManager::shutdown(void)
	{
		if (rootoverlay)
		{
			delete rootoverlay;
		}
		return true;
	}
	
	Overlay *OverlayManager::getRootOverlay(void)
	{
		return rootoverlay;
	}
	
	bool OverlayManager::render(void)
	{
		if (rootoverlay)
		{
			return rootoverlay->render();
		}
		return false;
	}
	
	OverlayManager::OverlayManager()
	{
		rootoverlay = 0;
	}
}


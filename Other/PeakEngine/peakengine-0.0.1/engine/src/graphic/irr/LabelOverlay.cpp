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

#include "graphic/LabelOverlay.h"
#include "graphic/GraphicsEngine.h"
#include "core/FileSystem.h"
#include "core/Util.h"

#include <irrlicht.h>
using namespace irr;

namespace peak
{
	LabelOverlay::LabelOverlay(std::string label, Color color, std::string font, Overlay *parent) : Overlay(parent)
	{
		this->label = label;
		this->color = color;
		this->font = 0;
		if (font != "")
		{
			std::string fontfile = FileSystem::get()->getFileExists(std::string("materials/") + font);
			if (fontfile != "")
			{
				this->font = GraphicsEngine::get()->getGUI()->getFont(fontfile.c_str());
			}
		}
		if (!this->font)
		{
			this->font = GraphicsEngine::get()->getGUI()->getBuiltInFont();
		}
	}
	LabelOverlay::~LabelOverlay()
	{
	}

	void LabelOverlay::setLabel(std::string label)
	{
		this->label = label;
	}
	std::string LabelOverlay::getLabel(void)
	{
		return label;
	}

	void LabelOverlay::setColor(Color color)
	{
		this->color = color;
	}
	Color LabelOverlay::getColor(void)
	{
		return color;
	}

	bool LabelOverlay::render(void)
	{
		Vector2D position = getAbsolutePosition();
		Vector2D size = getAbsoluteSize();

		core::dimension2d<u32> windowsize = GraphicsEngine::get()->getDriver()->getCurrentRenderTargetSize();
		core::rect<s32> textrect(position.x * windowsize.Width,
			position.y * windowsize.Height,
			(position.x + size.x) * windowsize.Width,
			(position.y + size.y) * windowsize.Height);
		font->draw(toWString(label).c_str(), textrect, video::SColor(255, 255, 255, 255));

		return Overlay::render();
	}
}


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

#include "graphics/LabelOverlay.h"
#include "graphics/GraphicsEngine.h"
#include "core/FileSystem.h"
#include "core/Util.h"
#include "core/GameEngine.h"


#ifdef __EMSCRIPTEN__
#include "../support/Framework.h"
#else
#include <Horde3D.h>
#include <Horde3DUtils.h>
#endif
namespace peak
{
	LabelOverlay::LabelOverlay(std::string label, std::string font, Overlay *parent) : Overlay(parent)
	{
		this->label = label;
		material = Horde3D::addResource(ResourceTypes::Material, font.c_str(), 0);
		Horde3DUtils::loadResourcesFromDisk(GameEngine::get()->getRootDirectory().c_str());
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

	bool LabelOverlay::render(void)
	{
		Vector2D position = getAbsolutePosition();
		Vector2D size = getAbsoluteSize();

		Horde3DUtils::showText(label.c_str(), position.x, 1 - position.y - size.y, size.y, 7, material);

		return Overlay::render();
	}
}


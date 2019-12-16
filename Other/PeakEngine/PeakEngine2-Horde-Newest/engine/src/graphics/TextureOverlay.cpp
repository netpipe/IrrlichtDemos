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

#include "graphics/TextureOverlay.h"
#include "graphics/GraphicsEngine.h"
#include "core/FileSystem.h"
#include "core/SettingsManager.h"
#include "core/GameEngine.h"

#include <Horde3D.h>
#include <Horde3DUtils.h>

namespace peak
{
	TextureOverlay::TextureOverlay(std::string filename, Overlay *parent) : Overlay(parent)
	{
		material = Horde3D::addResource(ResourceTypes::Material, filename.c_str(), 0);
		Horde3DUtils::loadResourcesFromDisk(GameEngine::get()->getRootDirectory().c_str());
	}
	TextureOverlay::~TextureOverlay()
	{
	}

	void TextureOverlay::notifySizeChanged(void)
	{
		// Recalculate position
		Vector2D position = getAbsolutePosition();
		Vector2D size = getAbsoluteSize();
		size /= 2;
		Vector2D center = position + size;
		float rotation = getAbsoluteRotation();

		Vector2DI windowsize = GraphicsEngine::get()->getScreensize();
		float aspectratio = (float)windowsize.x / windowsize.y;
		// Create data to render
		Vector2D offset = size;
		offset.y /= aspectratio;
		offset.rotate(rotation);
		offset.y *= aspectratio;
		vertices[1] = Vector2D(center.x + offset.x, center.y + offset.y);
		vertices[3] = Vector2D(center.x - offset.x, center.y - offset.y);
		offset = size;
		offset.x = -offset.x;
		offset.y /= aspectratio;
		offset.rotate(rotation);
		offset.y *= aspectratio;
		vertices[0] = Vector2D(center.x + offset.x, center.y + offset.y);
		vertices[2] = Vector2D(center.x - offset.x, center.y - offset.y);

		// Update children
		Overlay::notifySizeChanged();
	}
	bool TextureOverlay::render(void)
	{
		// Render textured rectangle
		Horde3D::showOverlay(vertices[0].x, 1 - vertices[0].y, 0, 0,
			vertices[1].x, 1 - vertices[1].y, 1, 0,
			vertices[2].x, 1 - vertices[2].y, 1, 1,
			vertices[3].x, 1 - vertices[3].y, 0, 1,
			1, 1, 1, 1,
			material,
			7);

		return Overlay::render();
	}
}


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

#include "graphic/TextureOverlay.h"
#include "graphic/GraphicsEngine.h"
#include "core/FileSystem.h"

#include <irrlicht.h>
using namespace irr;

namespace peak
{
	TextureOverlay::TextureOverlay(std::string filename, Overlay *parent) : Overlay(parent)
	{
		texture = FileSystem::get()->getFileExists(std::string("materials/") + filename);
		vertices = new video::S3DVertex[4];
		textureptr = GraphicsEngine::get()->getDriver()->getTexture(texture.c_str());
	}
	TextureOverlay::~TextureOverlay()
	{
		delete[] vertices;
	}

	void TextureOverlay::notifySizeChanged(void)
	{
		// Recalculate position
		Vector2D position = getAbsolutePosition();
		Vector2D size = getAbsoluteSize();
		size /= 2;
		Vector2D center = position + size;
		float rotation = getAbsoluteRotation();

		core::dimension2d<u32> windowsize = GraphicsEngine::get()->getDriver()->getCurrentRenderTargetSize();
		float aspectratio = (float)windowsize.Width / windowsize.Height;
		// Create data to render
		Vector2D offset = size;
		offset.y /= aspectratio;
		offset.rotate(rotation);
		offset.y *= aspectratio;
		core::vector3df pos1 = core::vector3df(center.x + offset.x, center.y + offset.y, 0);
		core::vector3df pos3 = core::vector3df(center.x - offset.x, center.y - offset.y, 0);
		offset = size;
		offset.x = -offset.x;
		offset.y /= aspectratio;
		offset.rotate(rotation);
		offset.y *= aspectratio;
		core::vector3df pos2 = core::vector3df(center.x + offset.x, center.y + offset.y, 0);
		core::vector3df pos4 = core::vector3df(center.x - offset.x, center.y - offset.y, 0);

		vertices[0]  = video::S3DVertex(pos1, core::vector3df(-1,-1,-1), video::SColor(255, 255, 255, 255), core::vector2df(1, 1));
		vertices[1]  = video::S3DVertex(pos2, core::vector3df(1,-1,-1), video::SColor(255, 255, 255, 255), core::vector2df(0, 1));
		vertices[2]  = video::S3DVertex(pos3, core::vector3df(1, 1,-1), video::SColor(255, 255, 255, 255), core::vector2df(0, 0));
		vertices[3]  = video::S3DVertex(pos4, core::vector3df(-1, 1,-1), video::SColor(255, 255, 255, 255), core::vector2df(1, 0));

		// Update children
		Overlay::notifySizeChanged();
	}
	bool TextureOverlay::render(void)
	{
		// Render colored rectangle
		video::SMaterial material;
		material.Lighting = false;
		material.Wireframe = false;
		material.ZBuffer = 0;
		material.setTexture(0, textureptr);
		material.MaterialType = video::EMT_TRANSPARENT_ALPHA_CHANNEL;

		GraphicsEngine::get()->getDriver()->setMaterial(material);
		GraphicsEngine::get()->getDriver()->drawIndexedTriangleList(vertices, 4, indices, 2);

		return Overlay::render();
	}

	unsigned short TextureOverlay::indices[6] = {0,1,2, 0,2,3};
}


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

#include "graphic/RectangleOverlay.h"
#include "graphic/GraphicsEngine.h"

#include <irrlicht.h>
using namespace irr;

namespace peak
{
	RectangleOverlay::RectangleOverlay(Color color, Overlay *parent) : Overlay(parent)
	{
		for (unsigned int i = 0; i < 4; i++)
		{
			this->color[i] = color;
		}
	}
	RectangleOverlay::RectangleOverlay(Color color1, Color color2, Color color3, Color color4, Overlay *parent) : Overlay(parent)
	{
		color[0] = color1;
		color[1] = color2;
		color[2] = color3;
		color[3] = color4;
	}
	RectangleOverlay::~RectangleOverlay()
	{
	}

	bool RectangleOverlay::render(void)
	{
		Vector2D position = getAbsolutePosition();
		Vector2D size = getAbsoluteSize();
		size /= 2;
		Vector2D center = position + size;
		float rotation = getAbsoluteRotation();

		core::dimension2d<s32> windowsize = GraphicsEngine::get()->getDriver()->getCurrentRenderTargetSize();
		float aspectratio = (float)windowsize.Width / windowsize.Height;
		// Create data to render
		video::S3DVertex vertices[4];
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

		vertices[0]  = video::S3DVertex(pos1, core::vector3df(-1,-1,-1), video::SColor(color[2].get()), core::vector2df(0, 1));
		vertices[1]  = video::S3DVertex(pos2, core::vector3df(1,-1,-1), video::SColor(color[3].get()), core::vector2df(1, 1));
		vertices[2]  = video::S3DVertex(pos3, core::vector3df(1, 1,-1), video::SColor(color[0].get()), core::vector2df(1, 0));
		vertices[3]  = video::S3DVertex(pos4, core::vector3df(-1, 1,-1), video::SColor(color[1].get()), core::vector2df(0, 0));

		u16 indices[6] = {0,1,2, 0,2,3};

		// Render colored rectangle
		video::SMaterial material;
		material.Lighting = false;
		material.Wireframe = false;
		material.ZBuffer = 0;
		material.MaterialType = video::EMT_TRANSPARENT_VERTEX_ALPHA;

		GraphicsEngine::get()->getDriver()->setMaterial(material);
		GraphicsEngine::get()->getDriver()->drawIndexedTriangleList(&vertices[0], 4, &indices[0], 2);

		return Overlay::render();
	}
}


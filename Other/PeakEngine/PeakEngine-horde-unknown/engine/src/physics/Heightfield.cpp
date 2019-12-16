/*
Copyright (C) 2008   Mathias Gottschlag

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

#include "physics/Heightfield.h"
#include "graphics/Image.h"
#include "core/Logger.h"

#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>
#include <BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h>

namespace peak
{
	Heightfield::Heightfield()	{
		terrain = 0;
	}
	Heightfield::~Heightfield()	{
	}
	
	bool Heightfield::create(std::string heightmap, float mass, Vector3D scale)	{
		Image *heightmapimage = new Image();
		if (!heightmapimage->load(std::string("textures/") + heightmap))
		{
			LERROR("Could not load heightmap image %s.\n", (std::string("textures/") + heightmap).c_str());
			delete heightmapimage;
			return false;
		}
		
		float *heightvalues = new float[(heightmapimage->getDimension().x + 1) * (heightmapimage->getDimension().y + 1)];
		for (int y = 0; y < heightmapimage->getDimension().y; y++)
		{
			for (int x = 0; x < heightmapimage->getDimension().x; x++)
			{
				Color pixel = heightmapimage->getPixel(x, y);
				heightvalues[x + y * (heightmapimage->getDimension().x + 1)] = ((float)pixel.getRed() + (float)pixel.getGreen() / 256) * scale.y;
				heightvalues[x + y * (heightmapimage->getDimension().x + 1)] /= 257;
			}
		}
		for (int x = 0; x < heightmapimage->getDimension().x; x++)
		{
			heightvalues[x + heightmapimage->getDimension().y * (heightmapimage->getDimension().x + 1)] = heightvalues[x + (heightmapimage->getDimension().y - 1) * (heightmapimage->getDimension().x + 1)];
		}
		for (int y = 0; y < heightmapimage->getDimension().y + 1; y++)
		{
			heightvalues[heightmapimage->getDimension().x + y * (heightmapimage->getDimension().x + 1)] = heightvalues[(heightmapimage->getDimension().x - 1) + y * (heightmapimage->getDimension().x + 1)];
		}
		
		terrain = new btHeightfieldTerrainShape((heightmapimage->getDimension().x + 1), (heightmapimage->getDimension().y + 1), heightvalues, 257, 1, true, false);
		shape = terrain;
		terrain->setLocalScaling(btVector3(scale.x / heightmapimage->getDimension().x, 1, scale.z / heightmapimage->getDimension().y));
		
		transform = new btTransform();
		transform->setIdentity();
		transform->setOrigin(btVector3(0, 0, 0));


		inertia = Vector3D(0, 0, 0);
		if( mass != 0.0f)
		{
			btVector3 in;
			shape->calculateLocalInertia(mass, in);
			inertia = Vector3D(in.x(), in.y(), in.z());
		}
		this->mass = mass;
		return true;
	}
	bool Heightfield::destroy()	{
		delete terrain;
		return true;
	}
}


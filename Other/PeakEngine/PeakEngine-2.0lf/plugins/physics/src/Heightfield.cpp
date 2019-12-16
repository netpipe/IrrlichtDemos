/*
Copyright (c) 2008, Christian Reiser

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

#include "peakphysics/Heightfield.hpp"

#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h>
#include <FreeImagePlus.h>
#include <cstdio>

namespace peak
{
	Heightfield::Heightfield() : Shape(), terrain(0)
	{
	}
	Heightfield::~Heightfield()
	{
	}

	bool Heightfield::init(std::string heightmap, float mass, Vector3F scale)
	{
		// Load heightmap image
		fipImage *image = new fipImage;
		if (!image->load(heightmap.c_str()))
			return false;
		// Convert height data
		float *heightvalues = new float[image->getWidth() * image->getHeight()];
		for (int y = 0; y < image->getHeight(); y++)
		{
			for (int x = 0; x < image->getWidth(); x++)
			{
				RGBQUAD color;
				image->getPixelColor(x, image->getHeight() - y - 1, &color);
				heightvalues[x + y * image->getWidth()] = ((float)color.rgbRed + color.rgbGreen + color.rgbBlue) / 3.0f;
			}
		}
		// Create heightfield
		terrain = new btHeightfieldTerrainShape(image->getWidth(), image->getHeight(), heightvalues, 256, 1, true, false);
		terrain->setLocalScaling(btVector3(scale.x, scale.y, scale.z));
		shape = terrain;
		// Local transformation
		transform = new btTransform();
		transform->setIdentity();
		transform->setOrigin(btVector3(0, 8, 0));
		// Set mass
		inertia = Vector3F(0, 0, 0);
		if( mass != 0.0f)
		{
			btVector3 in;
			shape->calculateLocalInertia(mass, in);
			inertia = Vector3F(in.x(), in.y(), in.z());
		}
		this->mass = mass;
		delete image;
		return true;
	}
	bool Heightfield::destroy()
	{
		Shape::destroy();
		delete terrain;
		return true;
	}
}

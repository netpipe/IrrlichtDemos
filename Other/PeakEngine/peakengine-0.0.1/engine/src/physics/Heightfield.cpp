/*
Copyright (C) 2008  Mathias Gottschlag, Lukas Kropatschek

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
#include "physics/World.h"

#include "graphic/Image.h"
#include "core/Color.h"

// Does not work correctly?
//#define USE_ODE_HEIGHTFIELD

namespace peak
{
	#ifdef USE_ODE_HEIGHTFIELD
	dReal heightfield_callback(void* userdata, int x, int z)
	{
		Heightfield *heightfield = (Heightfield*)userdata;
		bool grayscale16bit = heightfield->getHeightmap()->getColorFormat() == res::ECF_GRAY16;
		float height = 0;
		Color pixel = heightfield->getHeightmap()->getPixel(x, z);
		if (grayscale16bit)
		{
			height = (float)((pixel.getRed() << 8)
				+ pixel.getGreen()) / 256;
		}
		else
		{
			height = (float)(pixel.getRed()
				+ pixel.getGreen()
				+ pixel.getBlue()) / 3;
		}
		height *= heightfield->getScale().y;
		//printf("Heightfield: %d/%d: %f\n", x, z, height);
		return height;
	}
	#endif

	Heightfield::Heightfield() : Geometry()
	{
	}
	Heightfield::~Heightfield()
	{
	}

	bool Heightfield::init(World *world, std::string heightmap, Vector3D scale)
	{
		// Load heightmap
		heightmapimage = new Image();
		heightmapimage->load(std::string("models/") + heightmap);


		#ifdef USE_ODE_HEIGHTFIELD
		// Create geometry object
		dHeightfieldDataID heightid = dGeomHeightfieldDataCreate();
		dGeomHeightfieldDataBuildCallback(heightid, this, heightfield_callback,
			(heightmapimage->getDimension().x * scale.x - 1),
			(heightmapimage->getDimension().y * scale.z - 1),
			heightmapimage->getDimension().x,
			heightmapimage->getDimension().y, 1, REAL(0.0), REAL(0.0), 0);
		dGeomHeightfieldDataSetBounds(heightid, REAL(0.0), REAL(255.0) * scale.y);
		geom = dCreateHeightfield(world->getSpace(), heightid, 1);
		#else

		// Create vertices
		dVector3 *vertices = new dVector3[heightmapimage->getDimension().x * heightmapimage->getDimension().y];
		for (int x = 0; x < heightmapimage->getDimension().x; x++)
		{
			for (int z = 0; z < heightmapimage->getDimension().y; z++)
			{
				int idx = x * heightmapimage->getDimension().y + z;
				vertices[idx][0] = (float)x * scale.x;
			    Color pixel = heightmapimage->getPixel(x, z);
				vertices[idx][1] = (float)(pixel.getRed() + pixel.getGreen() + pixel.getBlue()) / 3 * scale.y;
				vertices[idx][2] = (float)z * scale.z;
			}
		}

		// Create indices
		unsigned int *indices = new unsigned int[(heightmapimage->getDimension().x - 1) * (heightmapimage->getDimension().y - 1) * 6];
		for (int x = 0; x < heightmapimage->getDimension().y - 1; x++)
		{
			for (int z = 0; z < heightmapimage->getDimension().y - 1; z++)
			{
				int idx = (x * (heightmapimage->getDimension().y - 1) + z) * 6;
				indices[idx] = x * heightmapimage->getDimension().y + z;
				indices[idx + 1] = x * heightmapimage->getDimension().y + z + 1;
				indices[idx + 2] = (x + 1) * heightmapimage->getDimension().y + z + 1;
				indices[idx + 3] = (x + 1) * heightmapimage->getDimension().y + z;
				indices[idx + 4] = x * heightmapimage->getDimension().y + z;
				indices[idx + 5] = (x + 1) * heightmapimage->getDimension().y + z + 1;
			}
		}

		// Create geometry
		dTriMeshDataID trimesh = dGeomTriMeshDataCreate();
		dGeomTriMeshDataBuildSimple(trimesh, (dReal*)vertices, heightmapimage->getDimension().x * heightmapimage->getDimension().y,
			indices, (heightmapimage->getDimension().x - 1) * (heightmapimage->getDimension().y - 1) * 6);

		geom = dCreateTriMesh(world->getSpace(), trimesh, NULL, NULL, NULL);
		dGeomSetData(geom, this);

		#endif

		this->scale = scale;

		return true;
	}
	bool Heightfield::destroy(void)
	{
		dGeomDestroy(geom);
		return true;
	}

	Image *Heightfield::getHeightmap(void)
	{
		return heightmapimage;
	}
	Vector3D Heightfield::getScale(void)
	{
		return scale;
	}
}


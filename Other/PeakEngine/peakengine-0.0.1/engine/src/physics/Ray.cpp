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

#include "physics/Ray.h"
#include "physics/World.h"

namespace peak
{
	Ray::Ray() : Geometry()
	{
	}
	Ray::~Ray()
	{
	}
	
	bool Ray::init(World *world, Vector3D direction, float length)
	{
		if (world)
		{
			transfgeom = dCreateGeomTransform(world->getSpace());
		}
		else
		{
			transfgeom = dCreateGeomTransform(0);
		}
		dGeomSetData(transfgeom, this);
		geom = dCreateRay(0, length);
		dGeomTransformSetGeom(transfgeom, geom);
		dGeomRaySet(geom, 0, 0, 0, direction.x, direction.y, direction.z);
		dGeomSetPosition(geom, 0, -0.5, 0);
		dGeomSetData(transfgeom, this);
		return true;
	}
	bool Ray::destroy(void)
	{
		dGeomDestroy(geom);
		dGeomDestroy(transfgeom);
		return true;
	}
}


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

#include "physics/Geometry.h"
#include "physics/Body.h"

namespace peak
{
	Geometry::Geometry()
	{
		geom = 0;
		transfgeom = 0;
		object = 0;
		collinfo.callback1.script = 0;
		collinfo.callback2.script = 0;
		collinfo.friction = 10;
		collinfo.bounce = 0.5;
		collinfo.bouncevelocity = 0.5;
		body = 0;
	}
	Geometry::~Geometry()
	{
	}
	
	bool Geometry::destroy(void)
	{
		dGeomDestroy(geom);
		if (transfgeom)
		{
			dGeomDestroy(transfgeom);
		}
		return true;
	}
	
	void Geometry::setPosition(Vector3D pos)
	{
		dGeomSetPosition(geom, pos.x, pos.y, pos.z);
	}
	
	void Geometry::setBody(Body *body)
	{
		if (transfgeom)
		{
			dGeomSetBody(transfgeom, body->getBody());
		}
		else
		{
			dGeomSetBody(geom, body->getBody());
		}
		this->body = body;
	}
	
	dGeomID Geometry::getGeom(void)
	{
		if (transfgeom)
		{
			return transfgeom;
		}
		else
		{
			return geom;
		}
	}
}


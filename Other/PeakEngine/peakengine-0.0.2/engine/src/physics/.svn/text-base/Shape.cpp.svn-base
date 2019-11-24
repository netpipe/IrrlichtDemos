/*
Copyright (C) 2008   Christian Reiser, Mathias Gottschlag

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
 
#include "physics/Shape.h"

#include <btBulletDynamicsCommon.h>

namespace peak
{
	Shape::Shape()
	{
		shape = 0;
		transform = 0;
		mass = 0;
		inertia = Vector3D(0, 0, 0);
	}
	Shape::~Shape()
	{
	}
	
	bool Shape::destroy(void)
	{
		delete shape;
		delete transform;
		return true;
	}
	
	btCollisionShape *Shape::getShape(void)
	{
		return shape;
	}
	
	btTransform Shape::getTransform(void)
	{
		return *transform;
	}
	
	float Shape::getMass()
	{
		return mass;
	}
	
	Vector3D Shape::getInertia()
	{
		return inertia;
	}
}

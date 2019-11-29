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

#include "physics/Hinge2.h"
#include "physics/World.h"
#include "physics/Body.h"

namespace peak
{
	Hinge2::Hinge2()
	{
	}
	Hinge2::~Hinge2()
	{
	}
	
	bool Hinge2::init(World *world)
	{
		joint = dJointCreateHinge2(world->getWorld(), 0);
		return true;
	}
	
	void Hinge2::setBody1(Body *body1)
	{
		bodies[0] = body1;
		if (bodies[0] && bodies[1])
		{
			dJointAttach(joint, bodies[0]->getBody(), bodies[1]->getBody());
		}
	}
	void Hinge2::setBody2(Body *body2)
	{
		bodies[1] = body2;
		if (bodies[0] && bodies[1])
		{
			dJointAttach(joint, bodies[0]->getBody(), bodies[1]->getBody());
		}
	}
	
	void Hinge2::setAnchor(Vector3D anchor)
	{
		dJointSetHinge2Anchor(joint, anchor.x, anchor.y, anchor.z);
	}
	void Hinge2::setAxis1(Vector3D axis)
	{
		dJointSetHinge2Axis1(joint, axis.x, axis.y, axis.z);
	}
	void Hinge2::setAxis2(Vector3D axis)
	{
		dJointSetHinge2Axis2(joint, axis.x, axis.y, axis.z);
	}
}


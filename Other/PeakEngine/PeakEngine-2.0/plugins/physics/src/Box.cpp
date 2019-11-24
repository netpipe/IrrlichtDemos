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

#include "peakphysics/Box.hpp"

#include <btBulletDynamicsCommon.h>

namespace peak
{
	Box::Box() : Shape()
	{
	}
	Box::~Box()
	{
	}

	bool Box::init(Vector3F dimensions, float mass)
	{
		dimensions /= 2;
		shape = new btBoxShape(btVector3(dimensions.x, dimensions.y, dimensions.z));
		transform = new btTransform();
		transform->setIdentity();
		transform->setOrigin(btVector3(0, 0, 0));

		inertia = Vector3F(0, 0, 0);
		if (mass != 0.0f)
		{
			btVector3 in;
			shape->calculateLocalInertia(mass, in);
			inertia = Vector3F(in.x(), in.y(), in.z());
		}
		this->mass = mass;
		return true;
	}
}

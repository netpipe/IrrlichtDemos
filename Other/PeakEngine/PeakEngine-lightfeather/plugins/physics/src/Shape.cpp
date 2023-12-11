/*
Copyright (c) 2009, Mathias Gottschlag, Christian Reiser

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

#include "peakphysics/Shape.hpp"

#include <btBulletDynamicsCommon.h>

namespace peak
{
	Shape::Shape() : shape(0), mass(0)
	{
	}
	Shape::~Shape()
	{
	}

	bool Shape::destroy()
	{
		delete shape;
		return true;
	}

	float Shape::getMass()
	{
		return mass;
	}
	Vector3F Shape::getInertia()
	{
		return inertia;
	}

	void Shape::getBoundingSphere(float& radius, Vector3F& center)
	{
		btVector3 btcenter;
		shape->getBoundingSphere(btcenter, radius);
		center = Vector3F(btcenter.x(), btcenter.y(), btcenter.z());
	}

	btCollisionShape *Shape::getShape()
	{
		return shape;
	}

	void Shape::calculateInertia()
	{
		inertia = Vector3F(0, 0, 0);
		if (mass != 0.0f)
		{
			btVector3 in;
			shape->calculateLocalInertia(mass, in);
			inertia = Vector3F(in.x(), in.y(), in.z());
		}
	}
}

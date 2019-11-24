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

#include "peakphysics/Compound.hpp"

#include <btBulletDynamicsCommon.h>

namespace peak
{
		Compound::Compound()
		{
		}
		Compound::~Compound()
		{
		}

		bool Compound::init(bool dynamicaabbtree, float mass)
		{
			shape = new btCompoundShape(dynamicaabbtree);
			this->mass = mass;
			return true;
		}
		bool Compound::destroy()
		{
			Shape::destroy();
			for (unsigned int i = 0; i < childs.size(); i++)
			{
				childs[i]->destroy();
				delete childs[i];
			}
			childs.clear();
			return true;
		}

		void Compound::addShape(Shape *child, Vector3F offset, Quaternion orientation)
		{
			btTransform childtransform;
			childtransform.setIdentity();
			childtransform.setOrigin(btVector3(offset.x, offset.y, offset.z));
			childtransform.setRotation(btQuaternion(orientation.q[0], orientation.q[1], orientation.q[2], orientation.q[3]));
			((btCompoundShape*)shape)->addChildShape(childtransform, child->getShape());
			childs.push_back(child);
		}
		void Compound::removeShape(Shape *child)
		{
			((btCompoundShape*)shape)->removeChildShape(child->getShape());
			std::vector<Shape*>::iterator it = childs.begin();
			while (it != childs.end())
			{
				if (*it == child)
				{
					((Shape*)*it)->destroy();
					delete *it;
					childs.erase(it);
				}
			}
		}
}
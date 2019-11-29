/*
Copyright (C) 2008   Lukas Kropatschek, Mathias Gottschlag

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

#include "graphics/SceneNode.h"
#include "graphics/GraphicsEngine.h"
#include "core/GameEngine.h"
#include "core/Logger.h"

#include <Horde3D.h>
#include <math.h>
#include <support/quaternion.h>

namespace peak
{
	SceneNode::SceneNode()
	{
	}

	SceneNode::~SceneNode()
	{
		if (parent)
		{
			parent->removeChild(this);
		}
		if (node)
		{
			// Delete children
			while (children.size() > 0)
			{
				delete *children.begin();
			}
			// Remove node
			Horde3D::removeNode(node);
		}
	}
	
	void SceneNode::create(int node, SceneNode *parent)
	{
		if (!node)
		{
			node = Horde3D::addGroupNode(RootNode, "");
		}
		this->node = node;
		this->parent = 0;
		if (parent)
		{
			setParent(parent);
		}
		visible = true;
	}

	void SceneNode::setVisible(bool flag)
	{
		Horde3D::setNodeActivation(node, flag);
		visible = flag;
	}

	void SceneNode::setScale(Vector3D newscale)
	{
		float pos[3];
		float rot[3];
		Horde3D::getNodeTransform(node, &pos[0], &pos[1], &pos[2], &rot[0], &rot[1], &rot[2], 0, 0, 0);
		Horde3D::setNodeTransform(node, pos[0], pos[1], pos[2], rot[0], rot[1], rot[2], newscale.x, newscale.y, newscale.z);
	}

	void SceneNode::setPosition(Vector3D newpos)
	{
		float rot[3];
		float scale[3];
		Horde3D::getNodeTransform(node, 0, 0, 0, &rot[0], &rot[1], &rot[2], &scale[0], &scale[1], &scale[2]);
		Horde3D::setNodeTransform(node, newpos.x, newpos.y, newpos.z, rot[0], rot[1], rot[2], scale[0], scale[1], scale[2]);
	}
	void SceneNode::setPosition(float x, float y, float z)
	{
		setPosition(Vector3D(x, y, z));
	}

	void SceneNode::setRotation(Vector3D newrot)
	{
		const float *mdata;
		Horde3D::getNodeTransformMatrices(node, &mdata, 0);
		irr::core::CMatrix4<float> matrix;
		matrix.setM(mdata);
		matrix.setRotationRadians(irr::core::vector3df(newrot.x, newrot.y, newrot.z));
		Horde3D::setNodeTransformMatrix(node, matrix.pointer());
	}
	void SceneNode::setRotation(Quaternion newrot)
	{
		irr::core::quaternion q(newrot.x, newrot.y, newrot.z, newrot.w);
		irr::core::matrix4 m = q.getMatrix();
		m.makeInverse();
		float pos[3];
		float scale[3];
		Horde3D::getNodeTransform(node, &pos[0], &pos[1], &pos[2], 0, 0, 0, &scale[0], &scale[1], &scale[2]);
		Horde3D::setNodeTransformMatrix(node, m.pointer());
		setPosition(pos[0], pos[1], pos[2]);
		setScale(Vector3D(scale[0], scale[1], scale[2]));
	}
	void SceneNode::setRotation(float x, float y, float z)
	{
		setRotation(Vector3D(x, y, z));
	}
	void SceneNode::setRotation(float w, float x, float y, float z)
	{
		Quaternion q(x, y, z, w);
		setRotation(q);
	}

	void SceneNode::setParent(SceneNode *newparent)
	{
		if (parent)
		{
			parent->removeChild(this);
			parent = 0;
		}
		if (newparent)
		{
			parent = newparent;
			Horde3D::setNodeParent(node, newparent->node);
			parent->addChild(this);
		}
		else
		{
			Horde3D::setNodeParent(node, 0);
		}
	}
	SceneNode *SceneNode::getParent(void)
	{
		return parent;
	}

	Vector3D SceneNode::getPosition(void)
	{
		Vector3D pos;
		Horde3D::getNodeTransform(node, &pos.x, &pos.y, &pos.z, 0, 0, 0, 0, 0, 0);
		return pos;
	}

	Vector3D SceneNode::getRotationEuler(void)
	{
		Vector3D rot;
		Horde3D::getNodeTransform(node, 0, 0, 0, &rot.x, &rot.y, &rot.z, 0, 0, 0);
		rot.x *= M_PI / 180;
		rot.y *= M_PI / 180;
		rot.z *= M_PI / 180;
		return rot;
	}
	Quaternion SceneNode::getRotation(void)
	{
		return Quaternion(getRotationEuler());
	}

	bool SceneNode::isVisible(void)
	{
		return visible;
	}
	
	void SceneNode::getBoundingBox(Vector3D &min, Vector3D &max)
	{
		Horde3D::getNodeAABB(node, &min.x, &min.y, &min.z, &max.x, &max.y, &max.z);
	}
	
	void SceneNode::addChild(SceneNode *child)
	{
		children.push_back(child);
	}
	void SceneNode::removeChild(SceneNode *child)
	{
		std::list<SceneNode*>::iterator it = children.begin();
		while (it != children.end())
		{
			if (*it == child)
			{
				children.erase(it);
				return;
			}
			it++;
		}
	}
	
	int SceneNode::getNode(void)
	{
		return node;
	}
}


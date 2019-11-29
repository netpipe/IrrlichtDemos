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

#include "graphic/SceneNode.h"
#include "graphic/GraphicsEngine.h"
#include "graphic/Material.h"
#include "core/GameEngine.h"
#include "core/Logger.h"

#include <irrlicht.h>
using namespace irr;

namespace peak
{
	SceneNode::SceneNode(SceneNode *parent)
	{
		node = GraphicsEngine::get()->getSceneManager()->addDummyTransformationSceneNode();
		this->parent = 0;
		if (parent)
		{
			setParent(parent);
		}
	}

	SceneNode::~SceneNode()
	{
		if (parent)
		{
			parent->removeChild(this);
		}
		if (node)
		{
			// Keep children
			while (children.size() > 0)
			{
				// HACK!
				(*children.begin())->node = 0;
				(*children.begin())->setParent(0);
			}
			// Remove node
			node->remove();
		}
	}

	void SceneNode::setVisible(bool flag)
	{
		node->setVisible(flag);
	}

	void SceneNode::setScale(Vector3D newscale)
	{
		node->setScale(core::vector3df(newscale.x, newscale.y, newscale.z));
	}

	void SceneNode::setPosition(float x, float y, float z)
	{
		node->setPosition(core::vector3df(x, y, z));
	}

	void SceneNode::setRotation(float x, float y, float z)
	{

		node->setRotation(core::vector3df(x, y, z) * 180 / M_PI);
	}
	void SceneNode::setRotation(float w, float x, float y, float z)
	{
		core::quaternion rot(x, y, z, w);
		core::vector3df euler;
		rot.toEuler(euler);
		euler *= 180 / M_PI;
		node->setRotation(euler);
	}

	void SceneNode::setPosition(Vector3D newpos)
	{
		node->setPosition(core::vector3df(newpos.x, newpos.y, newpos.z));
	}

	void SceneNode::setRotation(Vector3D newrot)
	{
		node->setRotation(core::vector3df(newrot.x, newrot.y, newrot.z) * 180 / M_PI);
	}
	void SceneNode::setRotation(Quaternion newrot)
	{
		core::quaternion rot(newrot.q[1], newrot.q[2], newrot.q[3], newrot.q[0]);
		core::vector3df euler;
		rot.toEuler(euler);
		euler *= 180 / M_PI;
		node->setRotation(euler);
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
			if (node) node->setParent(newparent->getSceneNode());
			parent->addChild(this);
		}
		else
		{
			if (node) node->setParent(0);
		}
	}

	Vector3D SceneNode::getPosition(void)
	{
		Vector3D pos;
		pos.x = node->getPosition().X;
		pos.y = node->getPosition().Y;
		pos.z = node->getPosition().Z;
		return pos;
	}

	Vector3D SceneNode::getRotationEuler(void)
	{
		Vector3D rot;
		rot.x = node->getRotation().X * M_PI / 180;
		rot.y = node->getRotation().Y * M_PI / 180;
		rot.z = node->getRotation().Z * M_PI / 180;
		return rot;
	}
	Quaternion SceneNode::getRotation(void)
	{
		core::quaternion rot(node->getRotation().X * M_PI / 180,
			node->getRotation().Y * M_PI / 180, node->getRotation().Z * M_PI / 180);
		return Quaternion(rot.W, rot.X, rot.Y, rot.Z);
	}

	bool SceneNode::isVisible(void)
	{
		bool visible;
		visible = node->isVisible();
		return visible;
	}

	void SceneNode::setMaterial(std::string material)
	{
		this->material.load(material);
		reloadMaterial();
	}
	void SceneNode::setMaterial(Material &material)
	{
		this->material = material;
		reloadMaterial();
	}
	Material SceneNode::getMaterial(void)
	{
		return material;
	}
	void SceneNode::reloadMaterial(void)
	{
		if (node->getMaterialCount() == 0) return;
		// Set type
		switch (material.getType())
		{
			case EMT_Solid:
				node->setMaterialType(video::EMT_SOLID);
				break;
			case EMT_Add:
				node->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
				break;
			case EMT_ParallaxMap:
				node->setMaterialType(video::EMT_PARALLAX_MAP_SOLID);
				break;
			case EMT_NormalMap:
				node->setMaterialType(video::EMT_NORMAL_MAP_SOLID);
				break;
			case EMT_DetailMap:
				node->setMaterialType(video::EMT_DETAIL_MAP);
				break;
			default:
				node->setMaterialType(video::EMT_SOLID);
				LWARNING("Material type not supported.\n");
				break;
		}
		node->setMaterialFlag(video::EMF_LIGHTING, material.getLighting());
		node->setMaterialFlag(video::EMF_BACK_FACE_CULLING, material.getBackfaceCulling());
		// Set textures
		for (int i = 0; i < material.getTextureCount(); i++)
		{
			std::string path = GameEngine::get()->getRootDirectory() + "/materials/" + material.getTexture(i);
			node->setMaterialTexture(i, GraphicsEngine::get()->getDriver()->getTexture(path.c_str()));
			Vector2D scale = material.getTextureScale(i);
			core::matrix4 texturemat;
			texturemat.setScale(core::vector3df(scale.x, scale.y, 1));
			node->getMaterial(0).setTextureMatrix(i, texturemat);
		}
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

	irr::scene::ISceneNode *SceneNode::getSceneNode(void)
	{
		return node;
	}
}


/*
Copyright (c) 2009, Mathias Gottschlag

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

#include "peakgraphics/scene/SceneNode.hpp"
#include "peakgraphics/Graphics.hpp"
#include <peakengine/support/ScopedLock.hpp>

#include <lf/Lightfeather.h>
using namespace lf;

namespace peak
{
	SceneNode::SceneNode(Graphics *graphics) : Loadable(),
		transformationcount(0), scale(1, 1, 1), moving(true), visible(true),
		parent(0), newparent(0), node(0), graphics(graphics)
	{
	}
	SceneNode::~SceneNode()
	{
	}

	void SceneNode::setTransformation(const Vector3F &position,
		const Quaternion &rotation, unsigned int time)
	{
		ScopedLock lock(mutex);
		if (transformationcount == 0)
		{
			// We do not have any transformation information yet
			transformation[0].position = position;
			transformation[0].rotation = rotation;
			transformation[0].time = time;
			transformationcount = 1;
			return;
		}
		// Check whether the time already has been there
		for (unsigned int i = 0; i < transformationcount; i++)
		{
			if (transformation[i].time == (int)time)
			{
				transformation[i].position = position;
				transformation[i].rotation = rotation;
				return;
			}
		}
		// Add transformation entry
		if (transformationcount < 3)
		{
			transformation[transformationcount].position = position;
			transformation[transformationcount].rotation = rotation;
			transformation[transformationcount].time = time;
			transformationcount++;
		}
		else
		{
			// Drop the oldest entry
			unsigned int oldestentry = 0;
			unsigned int oldesttime = 0xFFFFFFFF;
			for (unsigned int i = 0; i < 3; i++)
			{
				if (transformation[i].time < (int)oldesttime)
				{
					oldesttime = transformation[i].time;
					oldestentry = i;
				}
			}
			for (unsigned int i = oldestentry; i < 2; i++)
			{
				transformation[i] = transformation[i + 1];
			}
			// Set the last entry
			transformation[2].position = position;
			transformation[2].rotation = rotation;
			transformation[2].time = time;
		}
	}
	void SceneNode::getTransformation(Vector3F &position, Quaternion &rotation,
		unsigned int time)
	{
		ScopedLock lock(mutex);
		if (transformationcount == 0)
		{
			// We do not have any transformation information yet
			position = Vector3F();
			rotation = Vector3F();
			return;
		}
		if (transformationcount == 1)
		{
			// We do not have enough information to interpolate yet
			position = transformation[0].position;
			rotation = transformation[0].rotation;
			return;
		}
		// Get exact values
		for (unsigned int i = 0; i < transformationcount; i++)
		{
			if (transformation[i].time == (int)time)
			{
				position = transformation[i].position;
				rotation = transformation[i].rotation;
				return;
			}
		}
		// Interpolate
		if (transformationcount == 2)
		{
			TransformationInfo &a = transformation[0];
			TransformationInfo &b = transformation[1];
			float s = (float)((int)time - a.time) / (b.time - a.time);
			position.interpolate(a.position, b.position, s);
			rotation.interpolate(a.rotation, b.rotation, s);
			return;
		}
		else
		{
			// Sort entries
			if (transformation[1].time < transformation[0].time)
			{
				TransformationInfo tmp = transformation[1];
				transformation[1] = transformation[0];
				transformation[0] = tmp;
			}
			if (transformation[2].time < transformation[1].time)
			{
				TransformationInfo tmp = transformation[2];
				transformation[2] = transformation[1];
				transformation[1] = tmp;
			}
			if (transformation[1].time < transformation[0].time)
			{
				TransformationInfo tmp = transformation[1];
				transformation[1] = transformation[0];
				transformation[0] = tmp;
			}
			// Interpolate
			TransformationInfo *a = &transformation[0];
			TransformationInfo *b = &transformation[1];
			if ((int)time > b->time)
			{
				a = &transformation[1];
				b = &transformation[2];
			}
			float s = (float)((int)time - a->time) / (b->time - a->time);
			position.interpolate(a->position, b->position, s);
			rotation.interpolate(a->rotation, b->rotation, s);
		}
	}

	void SceneNode::setScale(Vector3F scale)
	{
		ScopedLock lock(mutex);
		this->scale = scale;
	}
	Vector3F SceneNode::getScale()
	{
		ScopedLock lock(mutex);
		return scale;
	}

	void SceneNode::setParent(SceneNode *parent)
	{
		ScopedLock lock(mutex);
		// Register for parent change
		newparent = parent;
		graphics->registerParentChange(this);
	}
	SceneNode *SceneNode::getParent()
	{
		return parent;
	}

	void SceneNode::setMoving(bool moving)
	{
		this->moving = moving;
	}
	bool SceneNode::isMoving()
	{
		return moving;
	}

	void SceneNode::setVisible(bool visible)
	{
		this->visible = visible;
	}
	bool SceneNode::isVisible()
	{
		return visible;
	}

	void SceneNode::updateParent()
	{
		ScopedLock lock(mutex);
		if (!newparent)
			return;
		// We have to make sure the scene node does not get deleted
		grab();
		if (node)
			node->grab();
		// Remove the node from the parent node
		if (parent)
			parent->removeChild(this);
		// Add to the new parent
		parent = newparent;
		parent->children.push_back(this);
		if (parent->node && node)
			parent->node->addChild(node);
		newparent = 0;
		if (node)
			node->drop();
		drop();
	}

	void SceneNode::update(unsigned int time)
	{
		if (!moving)
			return;
		// Update position
		Vector3F position;
		Quaternion rotation;
		getTransformation(position, rotation, time);
		mutex.lock();
		if (node)
		{
			node->setPosition(core::vector3df(position.x, position.y,
				position.z));
			node->setRotation(core::quaternion(rotation.q[0], rotation.q[1],
				rotation.q[2], rotation.q[3]));
			node->setScale(core::vector3df(scale.x, scale.y, scale.z));
			node->setVisible(visible);
		}
		mutex.unlock();
		// Update children
		for (unsigned int i = 0; i < children.size(); i++)
			children[i]->update(time);
	}

	void SceneNode::removeChild(SceneNode *child)
	{
		for (unsigned int i = 0; i < children.size(); i++)
		{
			if (children[i].get() == child)
			{
				if (node && child->node)
					node->removeChild(node);
				children.erase(children.begin() + i);
				return;
			}
		}
	}
}
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

#include "entity/GraphicsComponent.h"
#include "entity/PhysicsComponent.h"
#include "core/Game.h"
#include "entity/Entity.h"
#include "graphic/SceneNode.h"
#include "physics/Body.h"

namespace peak
{
	GraphicsComponent::GraphicsComponent()
	{
	}
	GraphicsComponent::~GraphicsComponent()
	{
	}
	
	bool GraphicsComponent::init(Entity *entity)
	{
		physics = (PhysicsComponent*)entity->getComponent(ECT_Physics);
		return true;
	}
	bool GraphicsComponent::destroy(void)
	{
		std::map<int, SceneNodeData>::iterator it = scenenodes.begin();
		while (it != scenenodes.end())
		{
			delete (*it).second.scenenode;
			it++;
		}
		scenenodes.clear();
		return true;
	}
	
	bool GraphicsComponent::needsUpdate(void)
	{
		bool needupdate = false;
		std::map<int, SceneNodeData>::iterator it = scenenodes.begin();
		while (it != scenenodes.end())
		{
			SceneNodeData data = (*it).second;
			float diff = (data.scenenode->getPosition() - data.position).getLengthSq();
			if (diff > 0.01)
			{
				data.update |= 1;
			}
			Quaternion q = data.scenenode->getRotation();
			diff = data.scenenode->getRotation().getDifference(data.rotation);
			if (diff > 0.001)
			{
				data.update |= 2;
			}
			diff = (data.oldspeed - data.speed).getLengthSq();
			if (diff > 0.001)
			{
				data.update |= 5;
			}
			
			(*it).second = data;
			if (data.update)
			{
				needupdate = true;
			}
			it++;
		}
		return needupdate;
	}
	int GraphicsComponent::getUpdateSize(void)
	{
		// TODO
		return 0;
	}
	int GraphicsComponent::writeUpdateData(Buffer *buffer)
	{
		std::map<int, SceneNodeData>::iterator it = scenenodes.begin();
		while (it != scenenodes.end())
		{
			buffer->writeByte((*it).second.update);
			if ((*it).second.update & 1)
			{
				buffer->writeVector3D((*it).second.scenenode->getPosition());
				(*it).second.position = (*it).second.scenenode->getPosition();
				(*it).second.update -= 1;
			}
			if ((*it).second.update & 2)
			{
				buffer->writeQuaternion((*it).second.scenenode->getRotation());
				(*it).second.rotation = (*it).second.scenenode->getRotation();
				(*it).second.update -= 2;
			}
			if ((*it).second.update & 4)
			{
				if ((*it).second.speed.getLengthSq() < 0.01)
				{
					buffer->writeVector3D(Vector3D(0, 0, 0));
					(*it).second.oldspeed = Vector3D(0, 0, 0);
				}
				else
				{
					buffer->writeVector3D((*it).second.speed);
					(*it).second.oldspeed = (*it).second.speed;
				}
				(*it).second.update -= 4;
			}
			it++;
		}
		return 0;
	}
	
	bool GraphicsComponent::updateFromData(Buffer *buffer)
	{
		std::map<int, SceneNodeData>::iterator it = scenenodes.begin();
		while (it != scenenodes.end())
		{
			unsigned char update = buffer->readByte();
			if (update & 1)
			{
				(*it).second.scenenode->setPosition(buffer->readVector3D());
			}
			if (update & 2)
			{
				(*it).second.scenenode->setRotation(buffer->readQuaternion());
			}
			if (update & 4)
			{
				(*it).second.speed = buffer->readVector3D();
			}
			it++;
		}
		return true;
	}
	
	bool GraphicsComponent::writeCompleteData(Buffer *buffer)
	{
		std::map<int, SceneNodeData>::iterator it = scenenodes.begin();
		while (it != scenenodes.end())
		{
			buffer->writeVector3D((*it).second.scenenode->getPosition());
			buffer->writeQuaternion((*it).second.scenenode->getRotation());
			buffer->writeVector3D((*it).second.speed);
			it++;
		}
		return true;
	}
	bool GraphicsComponent::readCompleteData(Buffer *buffer)
	{
		std::map<int, SceneNodeData>::iterator it = scenenodes.begin();
		while (it != scenenodes.end())
		{
			(*it).second.scenenode->setPosition(buffer->readVector3D());
			(*it).second.scenenode->setRotation(buffer->readQuaternion());
			(*it).second.speed = buffer->readVector3D();
			it++;
		}
		return true;
	}
	
	void GraphicsComponent::addSceneNode(SceneNode *node, int id, int parentid)
	{
		SceneNodeData data;
		data.scenenode = node;
		data.parentid = parentid;
		data.update = 0;
		if (parentid != 0)
		{
			SceneNode *parent = getSceneNode(parentid);
			if (parent)
			{
				data.body = 0;
				node->setParent(parent);
			}
			else
			{
				if (physics)
				{
					data.body = physics->getBody(parentid);
				}
				else
				{
					data.body = 0;
				}
			}
		}
		else
		{
			data.body = 0;
		}
		scenenodes.insert(std::pair<int, SceneNodeData>(id, data));
	}
	SceneNode *GraphicsComponent::getSceneNode(int id)
	{
		std::map<int, SceneNodeData>::iterator it = scenenodes.find(id);
		if (it == scenenodes.end()) return 0;
		return (*scenenodes.find(id)).second.scenenode;
	}
	
	ComponentType GraphicsComponent::getType(void)
	{
		return ECT_Graphics;
	}
	
	bool GraphicsComponent::doWork(float msecs)
	{
		std::map<int, SceneNodeData>::iterator it = scenenodes.begin();
		while (it != scenenodes.end())
		{
			SceneNode *node = (*it).second.scenenode;
			Body *body = (*it).second.body;
			if (body)
			{
				// Synchronize graphics with physics
				node->setPosition(body->getPosition());
				node->setRotation(body->getRotation());
				(*it).second.speed = body->getLinearVelocity();
			}
			else
			{
				// Update position from speed
				Vector3D change = (*it).second.speed;
				change *= msecs * 0.001;
				node->setPosition(node->getPosition() + change);
			}
			it++;
		}
		return true;
	}
}


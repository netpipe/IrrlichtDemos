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

#include "sound/Listener.h"
#include "graphic/SceneNode.h"

#include <AL/al.h>

namespace peak
{
	Listener::Listener()
	{
		node = 0;
		rotation = Quaternion(0, 0, 0);
		adjustrotation = false;
		adjustspeed = false;
	}
	Listener::~Listener()
	{
	}
	
	void Listener::setPosition(Vector3D pos)
	{
		alListener3f(AL_POSITION, pos.x, pos.y, pos.z);
	}
	Vector3D Listener::getPosition(void)
	{
		Vector3D pos;
		alGetListener3f(AL_POSITION, &pos.x, &pos.y, &pos.z);
		return pos;
	}
	void Listener::setVelocity(Vector3D vel)
	{
		alListener3f(AL_VELOCITY, vel.x, vel.y, vel.z);
	}
	Vector3D Listener::getVelocity(void)
	{
		Vector3D vel;
		alGetListener3f(AL_VELOCITY, &vel.x, &vel.y, &vel.z);
		return vel;
	}
	void Listener::setRotation(Quaternion rot)
	{
		rotation = rot;
	}
	Quaternion Listener::getRotation(void)
	{
		return rotation;
	}
	
	void Listener::attachToSceneNode(SceneNode *node, bool rotation, bool adjustspeed)
	{
		this->node = node;
		adjustrotation = rotation;
		this->adjustspeed = adjustspeed;
		if (node)
		{
			setPosition(node->getPosition());
			if (adjustrotation)
			{
				setRotation(node->getRotation());
			}
			else
			{
				setRotation(Quaternion(0, 0, 0));
			}
		}
		else
		{
		}
		setVelocity(Vector3D(0, 0, 0));
	}
	
	void Listener::doWork(float msecs)
	{
		if (node)
		{
			setPosition(node->getPosition());
			if (adjustrotation)
			{
				setRotation(node->getRotation());
			}
		}
	}
}


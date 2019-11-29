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

#include "graphic/Overlay.h"
#include "graphic/OverlayManager.h"

namespace peak
{
	Overlay::Overlay(Overlay *parent)
	{
		this->parent = parent;
		if (!parent)
		{
			this->parent = OverlayManager::get()->getRootOverlay();
		}
		if (this->parent)
		{
			this->parent->children.push_back(this);
		}
		rotation = 0;
		clipchildren = false;
	}
	Overlay::~Overlay()
	{
		while (children.size() > 0)
		{
			delete *children.begin();
		}
		if (parent)
		{
			// Remove overlay from parent
			for (std::list<Overlay*>::iterator it = parent->children.begin(); it != parent->children.end(); it++)
			{
				if (*it == this)
				{
					parent->children.erase(it);
					break;
				}
			}
		}
	}
	
	void Overlay::setParent(Overlay *overlay)
	{
		if (parent)
		{
			// Remove overlay from parent
			for (std::list<Overlay*>::iterator it = parent->children.begin(); it != parent->children.end(); it++)
			{
				if (*it == this)
				{
					parent->children.erase(it);
					break;
				}
			}
		}
		// Set new parent
		parent = overlay;
		if (!parent)
		{
			parent = OverlayManager::get()->getRootOverlay();
		}
		// Root overlay?
		if (parent == this)
		{
			parent = 0;
		}
		// Add child to parent
		if (parent)
		{
			parent->children.push_back(this);
		}
	}
	Overlay *Overlay::getParent(void)
	{
		return parent;
	}
	
	void Overlay::setPosition(Vector2D position)
	{
		this->position = position;
		notifySizeChanged();
	}
	Vector2D Overlay::getPosition(void)
	{
		return position;
	}
	void Overlay::setSize(Vector2D size)
	{
		this->size = size;
		notifySizeChanged();
	}
	Vector2D Overlay::getSize(void)
	{
		return size;
	}
	
	void Overlay::setRotation(float rotation)
	{
		this->rotation = rotation;
		notifySizeChanged();
	}
	float Overlay::getRotation(void)
	{
		return rotation;
	}
	
	void Overlay::setClipChildren(bool clipchildren)
	{
		this->clipchildren = clipchildren;
	}
	bool Overlay::getClipChildren(void)
	{
		return clipchildren;
	}
	
	void Overlay::setAbsolutePosition(Vector2D position)
	{
		if (parent)
		{
			Vector2D parentpos = parent->getAbsolutePosition();
			Vector2D parentsize = parent->getAbsoluteSize();
			position -= parentpos;
			this->position = Vector2D(position.x / parentsize.x, position.y / parentsize.y);
		}
		else
		{
			this->position = position;
		}
		notifySizeChanged();
	}
	Vector2D Overlay::getAbsolutePosition(void)
	{
		if (parent)
		{
			Vector2D parentpos = parent->getAbsolutePosition();
			Vector2D parentsize = parent->getAbsoluteSize();
			return parentpos + Vector2D(parentsize.x * position.x, parentsize.y * position.y);
		}
		else
		{
			return position;
		}
	}
	void Overlay::setAbsoluteSize(Vector2D size)
	{
		if (parent)
		{
			Vector2D parentsize = parent->getAbsoluteSize();
			this->size = Vector2D(size.x / parentsize.x, size.y / parentsize.y);
		}
		else
		{
			this->size = size;
		}
		notifySizeChanged();
	}
	Vector2D Overlay::getAbsoluteSize(void)
	{
		if (parent)
		{
			Vector2D parentsize = parent->getAbsoluteSize();
			return Vector2D(parentsize.x * size.x, parentsize.y * size.y);
		}
		else
		{
			return size;
		}
	}
	void Overlay::setAbsoluteRotation(float rotation)
	{
		if (parent)
		{
			this->rotation = rotation - parent->getAbsoluteRotation();
		}
		else
		{
			this->rotation = rotation;
		}
		notifySizeChanged();
	}
	float Overlay::getAbsoluteRotation(void)
	{
		if (parent)
		{
			return rotation + parent->getAbsoluteRotation();
		}
		else
		{
			return rotation;
		}
	}
	
	void Overlay::notifySizeChanged(void)
	{
		for (std::list<Overlay*>::iterator it = children.begin(); it != children.end(); it++)
		{
			(*it)->notifySizeChanged();
		}
	}
	bool Overlay::render(void)
	{
		// Render children
		for (std::list<Overlay*>::iterator it = children.begin(); it != children.end(); it++)
		{
			if (!(*it)->render())
			{
				return false;
			}
		}
		return true;
	}
}


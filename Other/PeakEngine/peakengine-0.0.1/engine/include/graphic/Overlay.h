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

#ifndef _OVERLAY_H_
#define _OVERLAY_H_

#include "core/Vector2D.h"

#include <list>

//tolua_begin
namespace peak
{
	class Overlay
	{
		public:
			Overlay(Overlay *parent = 0);
			virtual ~Overlay();
			
			virtual void setParent(Overlay *overlay);
			virtual Overlay *getParent(void);
			
			virtual void setPosition(Vector2D position);
			virtual Vector2D getPosition(void);
			virtual void setSize(Vector2D size);
			virtual Vector2D getSize(void);
			
			virtual void setRotation(float rotation);
			virtual float getRotation(void);
			
			virtual void setClipChildren(bool clipchildren);
			virtual bool getClipChildren(void);
			
			virtual void setAbsolutePosition(Vector2D position);
			virtual Vector2D getAbsolutePosition(void);
			virtual void setAbsoluteSize(Vector2D size);
			virtual Vector2D getAbsoluteSize(void);
			virtual void setAbsoluteRotation(float rotation);
			virtual float getAbsoluteRotation(void);
			
			virtual void notifySizeChanged(void);
			virtual bool render(void);
		private:
			//tolua_end
			Vector2D position;
			Vector2D size; 
			float rotation;
			bool clipchildren;
			std::list<Overlay*> children;
			Overlay *parent;
			//tolua_begin
	};
}
//tolua_end

#endif


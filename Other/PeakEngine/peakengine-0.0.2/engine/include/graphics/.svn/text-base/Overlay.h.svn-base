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
	/**
	 * \brief Class for 2D graphics
	 */
	class Overlay
	{
		public:
			Overlay(Overlay *parent = 0);
			virtual ~Overlay();
			
			/**
			 * \brief Sets the parent of the overlay
			 * \param overlay New parent
			 */
			virtual void setParent(Overlay *overlay);
			/**
			 * \brief Returns the parent of the overlay
			 */
			virtual Overlay *getParent(void);
			
			/**
			 * \brief Sets the position of the overlay
			 * \param position New position
			 */
			virtual void setPosition(Vector2D position);
			/**
			 * \brief Returns the position of the overlay
			 */
			virtual Vector2D getPosition(void);
			/**
			 * \brief Sets the size of the overlay
			 * \param size New size
			 */
			virtual void setSize(Vector2D size);
			/**
			 * \brief Returns the size of the overlay
			 */
			virtual Vector2D getSize(void);
			
			/**
			 * \brief Sets the rotation of the overlay
			 * \param rotation New rotation
			 */
			virtual void setRotation(float rotation);
			/**
			 * \brief Returns the rotation of the overlay
			 */
			virtual float getRotation(void);
			
			/**
			 * \brief Sets whether the overlay clips children
			 * \param clipchildren If set to true, children are clipped against the
			 * area of the overlay
			 */
			virtual void setClipChildren(bool clipchildren);
			/**
			 * \brief Returns whether the overlay clips children
			 */
			virtual bool getClipChildren(void);
			
			/**
			 * \brief Sets the absolute position of the overlay
			 * \param position New position
			 */
			virtual void setAbsolutePosition(Vector2D position);
			/**
			 * \brief Returns the absolute position of the overlay
			 */
			virtual Vector2D getAbsolutePosition(void);
			/**
			 * \brief Sets the absolute size of the overlay
			 * \param size New size
			 */
			virtual void setAbsoluteSize(Vector2D size);
			/**
			 * \brief Returns the absolute size of the overlay
			 */
			virtual Vector2D getAbsoluteSize(void);
			/**
			 * \brief Sets the absolute rotation of the overlay
			 * \param rotation New rotation
			 */
			virtual void setAbsoluteRotation(float rotation);
			/**
			 * \brief Returns the absolute rotation of the overlay
			 */
			virtual float getAbsoluteRotation(void);
			
			//tolua_end
			/**
			 * \brief Gets called whenever size/position of the overlay are changed
			 */
			virtual void notifySizeChanged(void);
			/**
			 * \brief Renders the overlay and its children
			 */
			virtual bool render(void);
		private:
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


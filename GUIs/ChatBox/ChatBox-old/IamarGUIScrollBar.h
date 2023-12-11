// Copyright (C) 2002-2005 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __I_GUI_SCROLL_BAR_H_INCLUDED__
#define __I_GUI_SCROLL_BAR_H_INCLUDED__

#include "IGUIElement.h"

namespace irr
{
namespace gui
{

	//! Default scroll bar GUI element.
	class IamarGUIScrollBar : public IGUIElement
	{
	public:

		//! constructor
		IamarGUIScrollBar(IGUIEnvironment* environment, IGUIElement* parent, s32 id, core::rect<s32> rectangle)
			: IGUIElement(EGUIET_SCROLL_BAR, environment, parent, id, rectangle) {}

		//! destructor
		~IamarGUIScrollBar() {};

		//! gets the maximum value of the scrollbar.
		virtual s32 getMax() = 0;

		//! sets the maximum value of the scrollbar. must be > 0
		virtual void setMax(s32 max) = 0;

		//! gets the current position of the scrollbar
		virtual s32 getPos() = 0;

		//! sets the current position of the scrollbar
		virtual void setPos(s32 pos) = 0;
	};


} // end namespace gui
} // end namespace irr

#endif


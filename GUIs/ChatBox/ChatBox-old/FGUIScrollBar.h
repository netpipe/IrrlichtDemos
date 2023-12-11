// Copyright (C) 2002-2005 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __C_GUI_SCROLL_BAR_H_INCLUDED__
#define __C_GUI_SCROLL_BAR_H_INCLUDED__


#include "IamarGUIScrollBar.h"
#include "IGUIButton.h"

namespace irr
{
namespace gui
{

	class FGUIScrollBar : public IamarGUIScrollBar
	{
	public:

		//! constructor
		FGUIScrollBar(bool horizontal, IGUIEnvironment* environment,
			IGUIElement* parent, s32 id, core::rect<s32> rectangle,
			bool noclip=false);

		//! destructor
		~FGUIScrollBar();

		//! called if an event happened.
		virtual bool OnEvent(SEvent event);

		//! draws the element and its children
		virtual void draw();

		//! sets the position of the scrollbar
		virtual void setPos(s32 pos);

		//! gets the maximum value of the scrollbar.
		virtual s32 getMax();

		//! sets the maximum value of the scrollbar. must be > 0
		virtual void setMax(s32 max);

		//! gets the current position of the scrollbar
		virtual s32 getPos();
		
	private:

		void setPosFromMousePos(s32 x, s32 y);

		IGUIButton* UpButton;
		IGUIButton* DownButton;

		bool Dragging;
		bool Horizontal;
		bool NoClip;
		s32 Pos;
		s32 DrawPos;
		s32 DrawHeight;
		s32 Max;
		s32 SmallStep;


	};

} // end namespace gui
} // end namespace irr

#endif


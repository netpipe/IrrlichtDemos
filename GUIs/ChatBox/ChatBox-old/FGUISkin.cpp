// Copyright (C) 2002-2005 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#include "CGUISkin.h"
#include "IGUIFont.h"
#include "IVideoDriver.h"

namespace irr
{
namespace gui
{

CGUISkin::CGUISkin(EGUI_SKIN_TYPE type, video::IVideoDriver* driver)
: Font(0), Driver(driver)
{
	#ifdef _DEBUG
	setDebugName("CGUISkin");
	#endif


	Colors[EGDC_3D_DARK_SHADOW] =	video::SColor(101,50,50,50);
	Colors[EGDC_3D_SHADOW] =		video::SColor(101,130,130,130);
	Colors[EGDC_3D_FACE] =			video::SColor(101,210,210,210);
	Colors[EGDC_3D_HIGH_LIGHT] =	video::SColor(101,255,255,255);
	Colors[EGDC_3D_LIGHT] =			video::SColor(101,210,210,210);
	Colors[EGDC_ACTIVE_BORDER] =	video::SColor(101,16,14,115);
	Colors[EGDC_ACTIVE_CAPTION] =	video::SColor(101,255,255,255);
	Colors[EGDC_APP_WORKSPACE] =	video::SColor(101,100,100,100);
	Colors[EGDC_BUTTON_TEXT] =		video::SColor(101,0,0,0);
	Colors[EGDC_GRAY_TEXT] =		video::SColor(101,130,130,130);
	Colors[EGDC_HIGH_LIGHT] =		video::SColor(101,8,36,107);
	Colors[EGDC_HIGH_LIGHT_TEXT] =	video::SColor(101,255,255,255);
	Colors[EGDC_INACTIVE_BORDER] =	video::SColor(101,165,165,165);
	Colors[EGDC_INACTIVE_CAPTION] = video::SColor(101,210,210,210);
	Colors[EGDC_TOOLTIP] =			video::SColor(101,255,255,230);
	Colors[EGDC_SCROLLBAR] =		video::SColor(101,230,230,230);
	Colors[EGDC_WINDOW] =			video::SColor(101,255,255,255);

	Sizes[EGDS_SCROLLBAR_SIZE] = 14;
	Sizes[EGDS_MENU_HEIGHT] = 18;
	Sizes[EGDS_WINDOW_BUTTON_WIDTH] = 15;
	Sizes[EGDS_CHECK_BOX_WIDTH] = 18;
	Sizes[EGDS_MESSAGE_BOX_WIDTH] = 500;
	Sizes[EGDS_MESSAGE_BOX_HEIGHT] = 200;
	Sizes[EGDS_BUTTON_WIDTH] = 80;
	Sizes[EGDS_BUTTON_HEIGHT] = 30;

	Texts[EGDT_MSG_BOX_OK] = L"OK";
	Texts[EGDT_MSG_BOX_CANCEL] = L"Cancel";
	Texts[EGDT_MSG_BOX_YES] = L"Yes";
	Texts[EGDT_MSG_BOX_NO] = L"No";

	UseGradient = (type == EGST_WINDOWS_METALLIC);
}


//! destructor
CGUISkin::~CGUISkin()
{
	if (Font)
		Font->drop();
}



//! returns default color
video::SColor CGUISkin::getColor(EGUI_DEFAULT_COLOR color)
{
	return Colors[color];
}


//! sets a default color
void CGUISkin::setColor(EGUI_DEFAULT_COLOR which, video::SColor newColor)
{
	if (which>=0 && which<= EGDC_COUNT)
		Colors[which] = newColor;
}


//! returns default color
s32 CGUISkin::getSize(EGUI_DEFAULT_SIZE size)
{
	return Sizes[size];
}



//! sets a default size
void CGUISkin::setSize(EGUI_DEFAULT_SIZE which, s32 size)
{
	if (which >= 0 && which <= EGDS_COUNT) 
		Sizes[which] = size;
}



//! returns the default font
IGUIFont* CGUISkin::getFont()
{
	return Font;
}


//! sets a default font
void CGUISkin::setFont(IGUIFont* font)
{
	if (Font)
		Font->drop();

	Font = font;

	if (Font)
		Font->grab();
}


//! Returns a default text. For example for Message box button captions:
//! "OK", "Cancel", "Yes", "No" and so on.
const wchar_t* CGUISkin::getDefaultText(EGUI_DEFAULT_TEXT text)
{
	return Texts[text].c_str();
}


//! Sets a default text. For example for Message box button captions:
//! "OK", "Cancel", "Yes", "No" and so on.
void CGUISkin::setDefaultText(EGUI_DEFAULT_TEXT which, const wchar_t* newText)
{
	Texts[which] = newText;
}

//! draws a standard 3d button pane
/**	Used for drawing for example buttons in normal state. 
It uses the colors EGDC_3D_DARK_SHADOW, EGDC_3D_HIGH_LIGHT, EGDC_3D_SHADOW and
EGDC_3D_FACE for this. See EGUI_DEFAULT_COLOR for details. 
\param rect: Defining area where to draw.
\param clip: Clip area.
\param element: Pointer to the element which whiches to draw this. This parameter
is usually not used by ISkin, but can be used for example by more complex 
implementations to find out how to draw the part exactly. */
void CGUISkin::draw3DButtonPaneStandard(IGUIElement* element,
										const core::rect<s32>& r,
										const core::rect<s32>* clip)
{
	if (!Driver)
		return;

	core::rect<s32> rect = r;
	Driver->draw2DRectangle(getColor(EGDC_3D_DARK_SHADOW), rect, clip);

	rect.LowerRightCorner.X -= 1;
	rect.LowerRightCorner.Y -= 1;
	Driver->draw2DRectangle(getColor(EGDC_3D_HIGH_LIGHT), rect, clip);

	rect.UpperLeftCorner.X += 1;
	rect.UpperLeftCorner.Y += 1;
	Driver->draw2DRectangle(getColor(EGDC_3D_SHADOW), rect, clip);

	rect.LowerRightCorner.X -= 1;
	rect.LowerRightCorner.Y -= 1;

	if (!UseGradient)
		Driver->draw2DRectangle(getColor(EGDC_3D_FACE), rect, clip);
	else
	{
		video::SColor c1 = getColor(EGDC_3D_FACE);
		video::SColor c2 = c1.getInterpolated(getColor(EGDC_3D_DARK_SHADOW), 0.4f);
		Driver->draw2DRectangle(rect, c1, c1, c2, c2, clip);
	}
}


//! draws a pressed 3d button pane
/**	Used for drawing for example buttons in pressed state. 
It uses the colors EGDC_3D_DARK_SHADOW, EGDC_3D_HIGH_LIGHT, EGDC_3D_SHADOW and
EGDC_3D_FACE for this. See EGUI_DEFAULT_COLOR for details. 
\param rect: Defining area where to draw.
\param clip: Clip area.
\param element: Pointer to the element which whiches to draw this. This parameter
is usually not used by ISkin, but can be used for example by more complex 
implementations to find out how to draw the part exactly. */
void CGUISkin::draw3DButtonPanePressed(IGUIElement* element, 
	const core::rect<s32>& r,
	const core::rect<s32>* clip)
{
	if (!Driver)
		return;

	core::rect<s32> rect = r;
	Driver->draw2DRectangle(getColor(EGDC_3D_HIGH_LIGHT), rect, clip);

	rect.LowerRightCorner.X -= 1;
	rect.LowerRightCorner.Y -= 1;
	Driver->draw2DRectangle(getColor(EGDC_3D_DARK_SHADOW), rect, clip);

	rect.UpperLeftCorner.X += 1;
	rect.UpperLeftCorner.Y += 1;
	Driver->draw2DRectangle(getColor(EGDC_3D_SHADOW), rect, clip);

	rect.UpperLeftCorner.X += 1;
	rect.UpperLeftCorner.Y += 1;

	if (!UseGradient)
		Driver->draw2DRectangle(getColor(EGDC_3D_FACE), rect, clip);
	else
	{
		video::SColor c1 = getColor(EGDC_3D_FACE);
		video::SColor c2 = c1.getInterpolated(getColor(EGDC_3D_DARK_SHADOW), 0.4f);
		Driver->draw2DRectangle(rect, c1, c1, c2, c2, clip);
	}
}


//! draws a sunken 3d pane
/** Used for drawing the background of edit, combo or check boxes.
\param element: Pointer to the element which whiches to draw this. This parameter
is usually not used by ISkin, but can be used for example by more complex 
implementations to find out how to draw the part exactly. 
\param bgcolor: Background color.
\param flat: Specifies if the sunken pane should be flat or displayed as sunken 
 deep into the ground.
\param rect: Defining area where to draw.
\param clip: Clip area.	*/
void CGUISkin::draw3DSunkenPane(IGUIElement* element,
	video::SColor bgcolor, bool flat, bool fillBackGround,
	const core::rect<s32>& r,
	const core::rect<s32>* clip)
{
	if (!Driver)
		return;

	core::rect<s32> rect = r;

	if (flat)
	{
		// draw flat sunken pane
		if (fillBackGround)
			Driver->draw2DRectangle(bgcolor, rect, clip);

		rect.LowerRightCorner.Y = rect.UpperLeftCorner.Y + 1;
		Driver->draw2DRectangle(getColor(EGDC_3D_SHADOW), rect, clip);

		rect.LowerRightCorner.Y = r.LowerRightCorner.Y;
		rect.LowerRightCorner.X = rect.UpperLeftCorner.X + 1;
		Driver->draw2DRectangle(getColor(EGDC_3D_SHADOW), rect, clip);

		rect = r;
		rect.UpperLeftCorner.X = rect.LowerRightCorner.X - 1;
		Driver->draw2DRectangle(getColor(EGDC_3D_HIGH_LIGHT), rect, clip);

		rect = r;
		rect.UpperLeftCorner.Y = r.LowerRightCorner.Y - 1;
		rect.LowerRightCorner.Y = r.LowerRightCorner.Y;
		Driver->draw2DRectangle(getColor(EGDC_3D_HIGH_LIGHT), rect, clip);
	}
	else
	{
		// draw deep sunken pane
		if (fillBackGround)
			Driver->draw2DRectangle(getColor(EGDC_3D_HIGH_LIGHT), rect, clip);

		rect.LowerRightCorner.X -= 1;
		rect.LowerRightCorner.Y -= 1;
		Driver->draw2DRectangle(getColor(EGDC_3D_SHADOW), rect, clip);

		rect.UpperLeftCorner.X += 1;
		rect.UpperLeftCorner.Y += 1;
		Driver->draw2DRectangle(getColor(EGDC_3D_LIGHT), rect, clip);

		rect.LowerRightCorner.X -= 1;
		rect.LowerRightCorner.Y -= 1;
		Driver->draw2DRectangle(getColor(EGDC_3D_DARK_SHADOW), rect, clip);

		rect.UpperLeftCorner.X += 1;
		rect.UpperLeftCorner.Y += 1;
		Driver->draw2DRectangle(bgcolor, rect, clip);
	}
}


//! draws a window background
/** Used for drawing the background of dialogs and windows.
\param element: Pointer to the element which whiches to draw this. This parameter
is usually not used by ISkin, but can be used for example by more complex 
implementations to find out how to draw the part exactly. 
\param titleBarColor: Title color.
\param drawTitleBar: True to enable title drawing.
\param rect: Defining area where to draw.
\param clip: Clip area.
\return Returns rect where to draw title bar text. */
core::rect<s32> CGUISkin::draw3DWindowBackground(IGUIElement* element,
	bool drawTitleBar, video::SColor titleBarColor,
	const core::rect<s32>& r,
	const core::rect<s32>* cl)
{
	if (!Driver)
		return r;

	core::rect<s32> rect = r;

	rect.LowerRightCorner.Y = rect.UpperLeftCorner.Y + 1;
	Driver->draw2DRectangle(getColor(EGDC_3D_HIGH_LIGHT), rect, cl);
	
	rect.LowerRightCorner.Y = r.LowerRightCorner.Y;
	rect.LowerRightCorner.X = rect.UpperLeftCorner.X + 1;
	Driver->draw2DRectangle(getColor(EGDC_3D_HIGH_LIGHT), rect, cl);

	rect.UpperLeftCorner.X = r.LowerRightCorner.X - 1;
	rect.LowerRightCorner.X = r.LowerRightCorner.X;
	rect.UpperLeftCorner.Y = r.UpperLeftCorner.Y;
    rect.LowerRightCorner.Y = r.LowerRightCorner.Y;
	Driver->draw2DRectangle(getColor(EGDC_3D_DARK_SHADOW), rect, cl);

	rect.UpperLeftCorner.X -= 1;
	rect.LowerRightCorner.X -= 1;
	rect.UpperLeftCorner.Y += 1;
	rect.LowerRightCorner.Y -= 1;
	Driver->draw2DRectangle(getColor(EGDC_3D_SHADOW), rect, cl);

	rect.UpperLeftCorner.X = r.UpperLeftCorner.X;
	rect.UpperLeftCorner.Y = r.LowerRightCorner.Y - 1;
	rect.LowerRightCorner.Y = r.LowerRightCorner.Y;
	rect.LowerRightCorner.X = r.LowerRightCorner.X;
	Driver->draw2DRectangle(getColor(EGDC_3D_DARK_SHADOW), rect, cl);

	rect.UpperLeftCorner.X += 1;
	rect.LowerRightCorner.X -= 1;
	rect.UpperLeftCorner.Y -= 1;
	rect.LowerRightCorner.Y -= 1;
	Driver->draw2DRectangle(getColor(EGDC_3D_SHADOW), rect, cl);

	rect = r;
	rect.UpperLeftCorner.X +=1;
	rect.UpperLeftCorner.Y +=1;
	rect.LowerRightCorner.X -= 2;
	rect.LowerRightCorner.Y -= 2;

	if (!UseGradient)
		Driver->draw2DRectangle(getColor(EGDC_3D_FACE), rect, cl);
	else
	{
		video::SColor c2 = getColor(EGDC_3D_SHADOW);
		video::SColor c1 = getColor(EGDC_3D_FACE);
		Driver->draw2DRectangle(rect, c1, c1, c1, c2, cl);
	}
	
	rect = r;
	rect.UpperLeftCorner.X += 2;
	rect.UpperLeftCorner.Y += 2;
	rect.LowerRightCorner.X -= 2;
	rect.LowerRightCorner.Y = rect.UpperLeftCorner.Y + getSize(EGDS_WINDOW_BUTTON_WIDTH) + 2;
	
	if (drawTitleBar)
	{
		// draw title bar
		//if (!UseGradient)
		//	Driver->draw2DRectangle(titleBarColor, rect, cl);
		//else
		{
			video::SColor c = titleBarColor.getInterpolated(video::SColor(255,0,0,0), 0.2f);
			Driver->draw2DRectangle(rect, titleBarColor, c, titleBarColor, c, cl);
		}
	}

	return rect;
}


//! draws a standard 3d menu pane
/**	Used for drawing for menus and context menus. 
It uses the colors EGDC_3D_DARK_SHADOW, EGDC_3D_HIGH_LIGHT, EGDC_3D_SHADOW and
EGDC_3D_FACE for this. See EGUI_DEFAULT_COLOR for details. 
\param element: Pointer to the element which whiches to draw this. This parameter
is usually not used by ISkin, but can be used for example by more complex 
implementations to find out how to draw the part exactly. 
\param rect: Defining area where to draw.
\param clip: Clip area.	*/
void CGUISkin::draw3DMenuPane(IGUIElement* element,
	const core::rect<s32>& r,
	const core::rect<s32>* clip)
{
	if (!Driver)
		return;

	// in this skin, this is exactly what non pressed buttons look like,
	// so we could simply call
	// draw3DButtonPaneStandard(element, rect, clip);	
	// here.
	// but if the skin is transparent, this doesn't look that nice. So
	// We draw it a little bit better, with some more draw2DRectangle calls,
	// but there aren't that much menus visible anyway.

	core::rect<s32> rect = r;
	rect.LowerRightCorner.Y = rect.UpperLeftCorner.Y + 1;
	Driver->draw2DRectangle(getColor(EGDC_3D_HIGH_LIGHT), rect, clip);

	rect.LowerRightCorner.Y = r.LowerRightCorner.Y;
	rect.LowerRightCorner.X = rect.UpperLeftCorner.X + 1;
	Driver->draw2DRectangle(getColor(EGDC_3D_HIGH_LIGHT), rect, clip);

	rect.UpperLeftCorner.X = r.LowerRightCorner.X - 1;
	rect.LowerRightCorner.X = r.LowerRightCorner.X;
	rect.UpperLeftCorner.Y = r.UpperLeftCorner.Y;
    rect.LowerRightCorner.Y = r.LowerRightCorner.Y;
	Driver->draw2DRectangle(getColor(EGDC_3D_DARK_SHADOW), rect, clip);

	rect.UpperLeftCorner.X -= 1;
	rect.LowerRightCorner.X -= 1;
	rect.UpperLeftCorner.Y += 1;
	rect.LowerRightCorner.Y -= 1;
	Driver->draw2DRectangle(getColor(EGDC_3D_SHADOW), rect, clip);

	rect.UpperLeftCorner.X = r.UpperLeftCorner.X;
	rect.UpperLeftCorner.Y = r.LowerRightCorner.Y - 1;
	rect.LowerRightCorner.Y = r.LowerRightCorner.Y;
	rect.LowerRightCorner.X = r.LowerRightCorner.X;
	Driver->draw2DRectangle(getColor(EGDC_3D_DARK_SHADOW), rect, clip);

	rect.UpperLeftCorner.X += 1;
	rect.LowerRightCorner.X -= 1;
	rect.UpperLeftCorner.Y -= 1;
	rect.LowerRightCorner.Y -= 1;
	Driver->draw2DRectangle(getColor(EGDC_3D_SHADOW), rect, clip);

	rect = r;
	rect.UpperLeftCorner.X +=1;
	rect.UpperLeftCorner.Y +=1;
	rect.LowerRightCorner.X -= 2;
	rect.LowerRightCorner.Y -= 2;

	if (!UseGradient)
		Driver->draw2DRectangle(getColor(EGDC_3D_FACE), rect, clip);
	else
	{
		video::SColor c1 = getColor(EGDC_3D_FACE);
		video::SColor c2 = getColor(EGDC_3D_SHADOW);
		Driver->draw2DRectangle(rect, c1, c1, c2, c2, clip);
	}
}


//! draws a standard 3d tool bar
/**	Used for drawing for toolbars and menus.
\param element: Pointer to the element which whiches to draw this. This parameter
is usually not used by ISkin, but can be used for example by more complex 
implementations to find out how to draw the part exactly. 
\param rect: Defining area where to draw.
\param clip: Clip area.	*/
void CGUISkin::draw3DToolBar(IGUIElement* element,
	const core::rect<s32>& r,
	const core::rect<s32>* clip)
{
	if (!Driver)
		return;

	core::rect<s32> rect = r;

	rect.UpperLeftCorner.X = r.UpperLeftCorner.X;
	rect.UpperLeftCorner.Y = r.LowerRightCorner.Y - 1;
	rect.LowerRightCorner.Y = r.LowerRightCorner.Y;
	rect.LowerRightCorner.X = r.LowerRightCorner.X;
	Driver->draw2DRectangle(getColor(EGDC_3D_SHADOW), rect, clip);

	rect = r;
	rect.LowerRightCorner.Y -= 1;

	if (!UseGradient)
		Driver->draw2DRectangle(getColor(EGDC_3D_FACE), rect, clip);
	else
	{
		video::SColor c1 = getColor(EGDC_3D_FACE);
		video::SColor c2 = getColor(EGDC_3D_SHADOW);
		Driver->draw2DRectangle(rect, c1, c1, c2, c2, clip);
	}
}

//! draws a tab button
/**	Used for drawing for tab buttons on top of tabs.
\param element: Pointer to the element which whiches to draw this. This parameter
is usually not used by ISkin, but can be used for example by more complex 
implementations to find out how to draw the part exactly. 
\param active: Specifies if the tab is currently active.
\param rect: Defining area where to draw.
\param clip: Clip area.	*/
void CGUISkin::draw3DTabButton(IGUIElement* element, bool active,
	const core::rect<s32>& frameRect, const core::rect<s32>* clip, EGUI_VERTICAL_ALIGNMENT alignment)
{
	if (!Driver)
		return;

	core::rect<s32> tr = frameRect;

	if ( alignment == EGVA_TOP )
	{
		tr.LowerRightCorner.X -= 2;
		tr.LowerRightCorner.Y = tr.UpperLeftCorner.Y + 1;
		tr.UpperLeftCorner.X += 1;
		Driver->draw2DRectangle(getColor(EGDC_3D_HIGH_LIGHT), tr, clip);
		
		// draw left highlight
		tr = frameRect;
		tr.LowerRightCorner.X = tr.UpperLeftCorner.X + 1;
		tr.UpperLeftCorner.Y += 1;
		Driver->draw2DRectangle(getColor(EGDC_3D_HIGH_LIGHT), tr, clip);

		// draw grey background
		tr = frameRect;
		tr.UpperLeftCorner.X += 1;
		tr.UpperLeftCorner.Y += 1;
		tr.LowerRightCorner.X -= 2;
		Driver->draw2DRectangle(getColor(EGDC_3D_FACE), tr, clip);

		
		// draw right middle gray shadow
		tr.LowerRightCorner.X += 1;
		tr.UpperLeftCorner.X = tr.LowerRightCorner.X - 1;
		Driver->draw2DRectangle(getColor(EGDC_3D_SHADOW), tr, clip);

		
		tr.LowerRightCorner.X += 1;
		tr.UpperLeftCorner.X += 1;
		tr.UpperLeftCorner.Y += 1;
		Driver->draw2DRectangle(getColor(EGDC_3D_DARK_SHADOW), tr, clip);		
	}
	else
	{
		tr.LowerRightCorner.X -= 2;
		tr.UpperLeftCorner.Y = tr.LowerRightCorner.Y - 1;
		tr.UpperLeftCorner.X += 1;
		Driver->draw2DRectangle(getColor(EGDC_3D_HIGH_LIGHT), tr, clip);
		
		// draw left highlight
		tr = frameRect;
		tr.LowerRightCorner.X = tr.UpperLeftCorner.X + 1;
		tr.LowerRightCorner.Y -= 1;
		Driver->draw2DRectangle(getColor(EGDC_3D_HIGH_LIGHT), tr, clip);

		// draw grey background
		tr = frameRect;
		tr.UpperLeftCorner.X += 1;
		tr.UpperLeftCorner.Y -= 1;
		tr.LowerRightCorner.X -= 2;
		tr.LowerRightCorner.Y -= 1;
		Driver->draw2DRectangle(getColor(EGDC_3D_SHADOW), tr, clip);
		
		// draw right middle gray shadow
		tr.LowerRightCorner.X += 1;
		tr.UpperLeftCorner.X = tr.LowerRightCorner.X - 1;
		//tr.LowerRightCorner.Y -= 1;
		Driver->draw2DRectangle(getColor(EGDC_3D_SHADOW), tr, clip);

		
		tr.LowerRightCorner.X += 1;
		tr.UpperLeftCorner.X += 1;
		tr.LowerRightCorner.Y -= 1;
		Driver->draw2DRectangle(getColor(EGDC_3D_DARK_SHADOW), tr, clip);		
	}

}


//! draws a tab control body
/**	\param element: Pointer to the element which whiches to draw this. This parameter
is usually not used by ISkin, but can be used for example by more complex 
implementations to find out how to draw the part exactly. 
\param border: Specifies if the border should be drawn.
\param background: Specifies if the background should be drawn.
\param rect: Defining area where to draw.
\param clip: Clip area.	*/
void CGUISkin::draw3DTabBody(IGUIElement* element, bool border, bool background,
	const core::rect<s32>& rect, const core::rect<s32>* clip, s32 tabHeight, EGUI_VERTICAL_ALIGNMENT alignment)
{
	if (!Driver)
		return;

	core::rect<s32> tr = rect;

	if ( tabHeight == -1 )
		tabHeight = getSize(gui::EGDS_BUTTON_HEIGHT);

	// draw border.
	if (border)
	{
		if ( alignment == EGVA_TOP )
		{
			// draw left hightlight
			tr.UpperLeftCorner.Y += tabHeight + 2;
			tr.LowerRightCorner.X = tr.UpperLeftCorner.X + 1;
			Driver->draw2DRectangle(getColor(EGDC_3D_HIGH_LIGHT), tr, clip);

			
			// draw right shadow
			tr.UpperLeftCorner.X = rect.LowerRightCorner.X - 1;
			tr.LowerRightCorner.X = tr.UpperLeftCorner.X + 1;
			Driver->draw2DRectangle(getColor(EGDC_3D_SHADOW), tr, clip);

			// draw lower shadow
			tr = rect;
			tr.UpperLeftCorner.Y = tr.LowerRightCorner.Y - 1;
			Driver->draw2DRectangle(getColor(EGDC_3D_SHADOW), tr, clip);
			
		}
		else
		{
			// draw left hightlight
			tr.LowerRightCorner.Y -= tabHeight + 2;
			tr.LowerRightCorner.X = tr.UpperLeftCorner.X + 1;
			Driver->draw2DRectangle(getColor(EGDC_3D_HIGH_LIGHT), tr, clip);
			
			// draw right shadow
			tr.UpperLeftCorner.X = rect.LowerRightCorner.X - 1;
			tr.LowerRightCorner.X = tr.UpperLeftCorner.X + 1;
			Driver->draw2DRectangle(getColor(EGDC_3D_SHADOW), tr, clip);
			
			// draw lower shadow
			tr = rect;
			tr.LowerRightCorner.Y = tr.UpperLeftCorner.Y + 1;
			Driver->draw2DRectangle(getColor(EGDC_3D_HIGH_LIGHT), tr, clip);
			
		}
	}

	if (background)
	{
		
		if ( alignment == EGVA_TOP )
		{
			tr = rect;
			tr.UpperLeftCorner.Y += tabHeight + 2;
			tr.LowerRightCorner.X -= 1;
			tr.UpperLeftCorner.X += 1;
			tr.LowerRightCorner.Y -= 1;
		}
		else
		{
			tr = rect;
			tr.UpperLeftCorner.X += 1;
			tr.UpperLeftCorner.Y -= 1;
			tr.LowerRightCorner.X -= 1;
			tr.LowerRightCorner.Y -= tabHeight + 2;
			//tr.UpperLeftCorner.X += 1;
		}

		if (!UseGradient)
			Driver->draw2DRectangle(getColor(EGDC_3D_FACE), tr, clip);
		else
		{
			video::SColor c1 = getColor(EGDC_3D_FACE);
			video::SColor c2 = getColor(EGDC_3D_SHADOW);
			Driver->draw2DRectangle(tr, c1, c1, c2, c2, clip);
		}		
	}

}


} // end namespace gui
} // end namespace irr


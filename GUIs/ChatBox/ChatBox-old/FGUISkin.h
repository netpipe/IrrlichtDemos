// Copyright (C) 2002-2005 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __C_GUI_SKIN_H_INCLUDED__
#define __C_GUI_SKIN_H_INCLUDED__

#include "IGUISkin.h"
#include "irrString.h"

namespace irr
{
namespace video
{
	class IVideoDriver;
}
namespace gui
{

	class CGUISkin : public IGUISkin
	{
	public:

		CGUISkin(EGUI_SKIN_TYPE type, video::IVideoDriver* driver);

		//! destructor
		~CGUISkin();

		//! returns default color
		virtual video::SColor getColor(EGUI_DEFAULT_COLOR color);

		//! sets a default color
		virtual void setColor(EGUI_DEFAULT_COLOR which, video::SColor newColor);

		//! returns default color
		virtual s32 getSize(EGUI_DEFAULT_SIZE size);

		//! sets a default size
		virtual void setSize(EGUI_DEFAULT_SIZE which, s32 size);

		//! returns the default font
		virtual IGUIFont* getFont();

		//! sets a default font
		virtual void setFont(IGUIFont* font);

		//! Returns a default text.
		/** For example for Message box button captions:
		 "OK", "Cancel", "Yes", "No" and so on. */
		virtual const wchar_t* getDefaultText(EGUI_DEFAULT_TEXT text);

		//! Sets a default text. 
		/** For example for Message box button captions:
		 "OK", "Cancel", "Yes", "No" and so on. */
		virtual void setDefaultText(EGUI_DEFAULT_TEXT which, const wchar_t* newText);

		//! draws a standard 3d button pane
		/**	Used for drawing for example buttons in normal state. 
		It uses the colors EGDC_3D_DARK_SHADOW, EGDC_3D_HIGH_LIGHT, EGDC_3D_SHADOW and
		EGDC_3D_FACE for this. See EGUI_DEFAULT_COLOR for details. 
		\param rect: Defining area where to draw.
		\param clip: Clip area.
		\param element: Pointer to the element which whiches to draw this. This parameter
		is usually not used by ISkin, but can be used for example by more complex 
		implementations to find out how to draw the part exactly. */
		virtual void draw3DButtonPaneStandard(IGUIElement* element, 
			const core::rect<s32>& rect,
			const core::rect<s32>* clip=0);

		//! draws a pressed 3d button pane
		/**	Used for drawing for example buttons in pressed state. 
		It uses the colors EGDC_3D_DARK_SHADOW, EGDC_3D_HIGH_LIGHT, EGDC_3D_SHADOW and
		EGDC_3D_FACE for this. See EGUI_DEFAULT_COLOR for details. 
		\param rect: Defining area where to draw.
		\param clip: Clip area.
		\param element: Pointer to the element which whiches to draw this. This parameter
		is usually not used by ISkin, but can be used for example by more complex 
		implementations to find out how to draw the part exactly. */
		virtual void draw3DButtonPanePressed(IGUIElement* element, 
			const core::rect<s32>& rect,
			const core::rect<s32>* clip=0);

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
		virtual void draw3DSunkenPane(IGUIElement* element,
			video::SColor bgcolor, bool flat, bool fillBackGround,
			const core::rect<s32>& rect,
			const core::rect<s32>* clip=0);

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
		virtual core::rect<s32> draw3DWindowBackground(IGUIElement* element,
			bool drawTitleBar, video::SColor titleBarColor,
			const core::rect<s32>& rect,
			const core::rect<s32>* clip=0);

		//! draws a standard 3d menu pane
		/**	Used for drawing for menus and context menus. 
		It uses the colors EGDC_3D_DARK_SHADOW, EGDC_3D_HIGH_LIGHT, EGDC_3D_SHADOW and
		EGDC_3D_FACE for this. See EGUI_DEFAULT_COLOR for details. 
		\param element: Pointer to the element which whiches to draw this. This parameter
		is usually not used by ISkin, but can be used for example by more complex 
		implementations to find out how to draw the part exactly. 
		\param rect: Defining area where to draw.
		\param clip: Clip area.	*/
		virtual void draw3DMenuPane(IGUIElement* element,
			const core::rect<s32>& rect,
			const core::rect<s32>* clip=0);

		//! draws a standard 3d tool bar
		/**	Used for drawing for toolbars and menus.
		\param element: Pointer to the element which whiches to draw this. This parameter
		is usually not used by ISkin, but can be used for example by more complex 
		implementations to find out how to draw the part exactly. 
		\param rect: Defining area where to draw.
		\param clip: Clip area.	*/
		virtual void draw3DToolBar(IGUIElement* element,
			const core::rect<s32>& rect,
			const core::rect<s32>* clip=0);

		//! draws a tab button
		/**	Used for drawing for tab buttons on top of tabs.
		\param element: Pointer to the element which whiches to draw this. This parameter
		is usually not used by ISkin, but can be used for example by more complex 
		implementations to find out how to draw the part exactly. 
		\param active: Specifies if the tab is currently active.
		\param rect: Defining area where to draw.
		\param clip: Clip area.	*/
		virtual void draw3DTabButton(IGUIElement* element, bool active,
			const core::rect<s32>& rect, const core::rect<s32>* clip=0, EGUI_VERTICAL_ALIGNMENT alignment=EGVA_TOP);

		//! draws a tab control body
		/**	\param element: Pointer to the element which whiches to draw this. This parameter
		is usually not used by ISkin, but can be used for example by more complex 
		implementations to find out how to draw the part exactly. 
		\param border: Specifies if the border should be drawn.
		\param background: Specifies if the background should be drawn.
		\param rect: Defining area where to draw.
		\param clip: Clip area.	*/
		virtual void draw3DTabBody(IGUIElement* element, bool border, bool background,
			const core::rect<s32>& rect, const core::rect<s32>* clip=0, s32 tabHeight=-1, EGUI_VERTICAL_ALIGNMENT alignment=EGVA_TOP);

	private:

		video::SColor Colors[EGDC_COUNT];
		s32 Sizes[EGDS_COUNT];
		IGUIFont* Font;
		core::stringw Texts[EGDT_COUNT];
		video::IVideoDriver* Driver;
		bool UseGradient;
	};




} // end namespace gui
} // end namespace irr

#endif


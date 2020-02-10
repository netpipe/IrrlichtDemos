// Copyright (C) 2002-2006 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __I_GUI_SKIN_H_INCLUDED__
#define __I_GUI_SKIN_H_INCLUDED__

#include "IUnknown.h"
#include "SColor.h"
#include "IGUISkin.h"
#include "rect.h"

namespace irr
{
namespace gui
{
	class IGUIFont;
	class IGUIElement;

	//! Enumeration of available default skins.
	/** To set one of the skins, use the following code, for example to set
	the Windows classic skin:
	\code
	gui::IGUISkin* newskin = environment->createSkin(gui::EGST_WINDOWS_CLASSIC);
	environment->setSkin(newskin);
	newskin->drop();
	\endcode
	*/
	enum EGUI_SKIN_TYPE
	{
		//! Default windows look and feel
		EGST_WINDOWS_CLASSIC,

		//! Like EGST_WINDOWS_CLASSIC, but with metallic shaded windows and buttons
		EGST_WINDOWS_METALLIC
	};

	//! Enumeration for skin colors
	enum EGUI_DEFAULT_COLOR
	{
		//! Dark shadow for three-dimensional display elements.
		EGDC_3D_DARK_SHADOW = 0,
		//! Shadow color for three-dimensional display elements (for edges facing away from the light source).
		EGDC_3D_SHADOW,
		//! Face color for three-dimensional display elements and for dialog box backgrounds.
		EGDC_3D_FACE,
		//! Highlight color for three-dimensional display elements (for edges facing the light source.)
		EGDC_3D_HIGH_LIGHT,
		//! Light color for three-dimensional display elements (for edges facing the light source.)
		EGDC_3D_LIGHT,
		//! Active window border.
		EGDC_ACTIVE_BORDER,
		//! Active window title bar text.
		EGDC_ACTIVE_CAPTION,
		//! Background color of multiple document interface (MDI) applications.
		EGDC_APP_WORKSPACE,
		//! Text on a button
		EGDC_BUTTON_TEXT,
		//! Grayed (disabled) text.
		EGDC_GRAY_TEXT,
		//! Item(s) selected in a control.
		EGDC_HIGH_LIGHT,
		//! Text of item(s) selected in a control.
		EGDC_HIGH_LIGHT_TEXT,
		//! Inactive window border.
		EGDC_INACTIVE_BORDER,
		//! Inactive window caption.
		EGDC_INACTIVE_CAPTION,
		//! Tool tip color
		EGDC_TOOLTIP,
		//! Scrollbar gray area
		EGDC_SCROLLBAR,
		//! Window background
		EGDC_WINDOW,

		EGDC_COUNT
	};

	//! Enumeration for default sizes.
	enum EGUI_DEFAULT_SIZE
	{
		//! default with / height of scrollbar
		EGDS_SCROLLBAR_SIZE = 0,

		//! height of menu
		EGDS_MENU_HEIGHT,

		//! width of a window button
		EGDS_WINDOW_BUTTON_WIDTH,

		//! width of a checkbox check
		EGDS_CHECK_BOX_WIDTH,

		//! width of a messagebox
		EGDS_MESSAGE_BOX_WIDTH,

		//! height of a messagebox
		EGDS_MESSAGE_BOX_HEIGHT,

		//! width of a default button
		EGDS_BUTTON_WIDTH,

		//! height of a default button
		EGDS_BUTTON_HEIGHT,
		
		//! this value is not used, it only specifies the amount of default sizes
		//! available.
		EGDS_COUNT
	};


	enum EGUI_DEFAULT_TEXT
	{
		//! Text for the OK button on a message box
		EGDT_MSG_BOX_OK,

		//! Text for the Cancel button on a message box
		EGDT_MSG_BOX_CANCEL,

		//! Text for the Yes button on a message box
		EGDT_MSG_BOX_YES,

		//! Text for the No button on a message box
		EGDT_MSG_BOX_NO,

		//! this value is not used, it only specifies the amount of default texts
		//! available.
		EGDT_COUNT
	};


	//! A skin modifies the look of the GUI elements.
	class IGUISkin : public virtual IUnknown
	{
	public:

		//! destructor
		~IGUISkin() {};

		//! returns default color
		virtual video::SColor getColor(EGUI_DEFAULT_COLOR color) = 0;

		//! sets a default color
		virtual void setColor(EGUI_DEFAULT_COLOR which, video::SColor newColor) = 0;

		//! returns default color
		virtual s32 getSize(EGUI_DEFAULT_SIZE size) = 0;

		//! Returns a default text. 
		/** For example for Message box button captions:
		"OK", "Cancel", "Yes", "No" and so on. */
		virtual const wchar_t* getDefaultText(EGUI_DEFAULT_TEXT text) = 0;

		//! Sets a default text.
		/** For example for Message box button captions:
		"OK", "Cancel", "Yes", "No" and so on. */
		virtual void setDefaultText(EGUI_DEFAULT_TEXT which, const wchar_t* newText) = 0;

		//! sets a default size
		virtual void setSize(EGUI_DEFAULT_SIZE which, s32 size) = 0;

		//! returns the default font
		virtual IGUIFont* getFont() = 0;

		//! sets a default font
		virtual void setFont(IGUIFont* font) = 0;

		//! draws a standard 3d button pane
		/**	Used for drawing for example buttons in normal state. 
		It uses the colors EGDC_3D_DARK_SHADOW, EGDC_3D_HIGH_LIGHT, EGDC_3D_SHADOW and
		EGDC_3D_FACE for this. See EGUI_DEFAULT_COLOR for details. 
		\param element: Pointer to the element which whiches to draw this. This parameter
		is usually not used by ISkin, but can be used for example by more complex 
		implementations to find out how to draw the part exactly. 
		\param rect: Defining area where to draw.
		\param clip: Clip area.	*/
		virtual void draw3DButtonPaneStandard(IGUIElement* element,
			const core::rect<s32>& rect,
			const core::rect<s32>* clip=0) = 0;

		//! draws a pressed 3d button pane
		/**	Used for drawing for example buttons in pressed state. 
		It uses the colors EGDC_3D_DARK_SHADOW, EGDC_3D_HIGH_LIGHT, EGDC_3D_SHADOW and
		EGDC_3D_FACE for this. See EGUI_DEFAULT_COLOR for details. 
		\param element: Pointer to the element which whiches to draw this. This parameter
		is usually not used by ISkin, but can be used for example by more complex 
		implementations to find out how to draw the part exactly. 
		\param rect: Defining area where to draw.
		\param clip: Clip area.	*/
		virtual void draw3DButtonPanePressed(IGUIElement* element, 
			const core::rect<s32>& rect,
			const core::rect<s32>* clip=0) = 0;

		//! draws a sunken 3d pane
		/** Used for drawing the background of edit, combo or check boxes.
		\param element: Pointer to the element which whiches to draw this. This parameter
		is usually not used by ISkin, but can be used for example by more complex 
		implementations to find out how to draw the part exactly. 
		\param bgcolor: Background color.
		\param flat: Specifies if the sunken pane should be flat or displayed as sunken 
		 deep into the ground.
		\param fillBackGround: Specifies if the background should be filled with the background
		color or not be drawn at all.
		\param rect: Defining area where to draw.
		\param clip: Clip area.	*/
		virtual void draw3DSunkenPane(IGUIElement* element,
			video::SColor bgcolor, bool flat, bool fillBackGround,
			const core::rect<s32>& rect,
			const core::rect<s32>* clip=0) = 0;

		//! draws a window background
		/** Used for drawing the background of dialogs and windows.
		\param element: Pointer to the element which whiches to draw this. This parameter
		is usually not used by ISkin, but can be used for example by more complex 
		implementations to find out how to draw the part exactly. 
		\param titleBarColor: Title color.
		\param drawTitleBar: True to enable title drawing.
		\param rect: Defining area where to draw.
		\param clip: Clip area.	
		\return Returns rect where it would be good to draw title bar text. */
		virtual core::rect<s32> draw3DWindowBackground(IGUIElement* element,
			bool drawTitleBar, video::SColor titleBarColor,
			const core::rect<s32>& rect,
			const core::rect<s32>* clip=0) = 0;

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
			const core::rect<s32>* clip=0) = 0;

		//! draws a standard 3d tool bar
		/**	Used for drawing for toolbars and menus.
		\param element: Pointer to the element which whiches to draw this. This parameter
		is usually not used by ISkin, but can be used for example by more complex 
		implementations to find out how to draw the part exactly. 
		\param rect: Defining area where to draw.
		\param clip: Clip area.	*/
		virtual void draw3DToolBar(IGUIElement* element,
			const core::rect<s32>& rect,
			const core::rect<s32>* clip=0) = 0;

		//! draws a tab button
		/**	Used for drawing for tab buttons on top of tabs.
		\param element: Pointer to the element which whiches to draw this. This parameter
		is usually not used by ISkin, but can be used for example by more complex 
		implementations to find out how to draw the part exactly. 
		\param active: Specifies if the tab is currently active.
		\param rect: Defining area where to draw.
		\param clip: Clip area.	*/
		virtual void draw3DTabButton(IGUIElement* element, bool active,
			const core::rect<s32>& rect, const core::rect<s32>* clip=0) = 0;

		//! draws a tab control body
		/**	\param element: Pointer to the element which whiches to draw this. This parameter
		is usually not used by ISkin, but can be used for example by more complex 
		implementations to find out how to draw the part exactly. 
		\param border: Specifies if the border should be drawn.
		\param background: Specifies if the background should be drawn.
		\param rect: Defining area where to draw.
		\param clip: Clip area.	*/
		virtual void draw3DTabBody(IGUIElement* element, bool border, bool background,
			const core::rect<s32>& rect, const core::rect<s32>* clip=0) = 0;
	};


} // end namespace gui
} // end namespace irr

#endif


// Copyright (C) 2002-2006 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __E_GUI_ELEMENT_TYPES_H_INCLUDED__
#define __E_GUI_ELEMENT_TYPES_H_INCLUDED__

namespace irr
{
namespace gui
{

//! List of all basic Irrlicht GUI elements. 
/** An IGUIElement returns this when calling IGUIElement::getType(); */
enum EGUI_ELEMENT_TYPE
{
	//! Unknown type. Use this when creating your own elements.
	EGUIET_ELEMENT = 0,

	//! A button (IGUIButton)
	EGUIET_BUTTON,

	//! A check box (IGUICheckBox)
	EGUIET_CHECK_BOX,

	//! A combo box (IGUIComboBox)
	EGUIET_COMBO_BOX,

	//! A context menu (IGUIContextMenu)
    EGUIET_CONTEXT_MENU,

	//! An edit box (IGUIEditBox)
	EGUIET_EDIT_BOX,

	//! A file open dialog (IGUIFileOpenDialog)
	EGUIET_FILE_OPEN_DIALOG,

	//! A in/out fader (IGUIInOutFader)
	EGUIET_IN_OUT_FADER,

	//! An image (IGUIImage)
	EGUIET_IMAGE,

	//! A list box (IGUIListBox)
	EGUIET_LIST_BOX,

	//! A mesh viewer (IGUIMeshViewer)
	EGUIET_MESH_VIEWER,

	//! A modal screen
	EGUIET_MODAL_SCREEN,

	//! A scroll bar (IGUIScrollBar)
	EGUIET_SCROLL_BAR,

	//! A static text (IGUIStaticText)
	EGUIET_STATIC_TEXT,

	//! A tab (IGUITab)
	EGUIET_TAB,

	//! A tab control
	EGUIET_TAB_CONTROL,

	//! A tool bar (IGUIToolBar)
	EGUIET_TOOL_BAR,

	//! A window
	EGUIET_WINDOW,

	//! Not an element, amount of elements in there
	EGUIET_COUNT
};


} // end namespace gui
} // end namespace irr

#endif


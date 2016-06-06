// Copyright (C) 2002-2005 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

// 07.10.2005 - Multicolor-Listbox addet by A. Buschhüter (Acki)
//                                          A_Buschhueter@gmx.de

#ifndef __I_GUI_CHAT_BOX_BAR_H_INCLUDED__
#define __I_GUI_CHAT_BOX_BAR_H_INCLUDED__

#include "IGUIElement.h"
#include "irrTypes.h"
// --------------------------------------------------------
#include "SColor.h"
// --------------------------------------------------------

namespace irr
{
namespace gui
{
	class IGUIFont;

	//! Default list box GUI element.
	class IGUIChatBox : public IGUIElement
	{
	public:
// --------------------------------------------------------
    virtual s32 addItem(const wchar_t* text, video::SColor color) = 0;
    virtual void setItemColor(s32 index, video::SColor color) = 0;
// --------------------------------------------------------

		//! constructor
		IGUIChatBox(IGUIEnvironment* environment, IGUIElement* parent, s32 id, core::rect<s32> rectangle)
			: IGUIElement(EGUIET_ELEMENT, environment, parent, id, rectangle) 
		{
		}

		//! destructor
		~IGUIChatBox() {};

		//! returns amount of list items
		virtual s32 getItemCount() = 0;

		//! returns string of a list item. the may id be a value from 0 to itemCount-1
		virtual const wchar_t* getListItem(s32 id) = 0;

		//! adds an list item, returns id of item
		virtual s32 addItem(const wchar_t* text) = 0;

		//! clears the list, deletes all items in the listbox
		virtual void clear() = 0;
	};


} // end namespace gui
} // end namespace irr

#endif


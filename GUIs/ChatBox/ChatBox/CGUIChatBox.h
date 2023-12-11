// Copyright (C) 2002-2008 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

// 07.10.2005 - Multicolor-Listbox addet by A. Buschhüter (Acki)
//                                          A_Buschhueter@gmx.de

// 16.12.2008 - Updated for Irrlicht 1.5 by Kai "DrHalan" Mast

#ifndef __C_GUI_CHAT_BOX_BAR_H_INCLUDED__
#define __C_GUI_CHAT_BOX_BAR_H_INCLUDED__

#include <irrlicht.h>

namespace irr
{
    namespace gui
    {
        class CGUIChatBox : public IGUIElement
        {
        public:
// --------------------------------------------------------
            virtual s32 addItem(const wchar_t* text, video::SColor color);
            virtual void setItemColor(s32 index, video::SColor color);
// --------------------------------------------------------

            //! constructor
            CGUIChatBox(IGUIEnvironment* environment, IGUIElement* parent,
                        s32 id, core::rect<s32> rectangle, bool clip=true,
                        bool drawBack=false, bool moveOverSelect=false);

            //! destructor
            ~CGUIChatBox();

            //! returns amount of list items
            virtual s32 getItemCount();

            //! returns string of a list item. the may be a value from 0 to itemCount-1
            virtual const wchar_t* getListItem(s32 id);

            //! adds an list item, returns id of item
            virtual s32 addItem(const wchar_t* text);

            //! clears the list
            virtual void clear();

            //! called if an event happened.
            virtual bool OnEvent(SEvent event);

            //! draws the element and its children
            virtual void draw();

        private:

            struct ListItem
            {
                core::stringw text;
                core::array< core::stringw > BrokenText;
                video::SColor color;
                u32 height;
            };

            void breakText(ListItem &item);
            void recalculateItemHeight();

            core::array< ListItem > Items;
            s32 ItemHeight;
            s32 TotalItemHeight;
            gui::IGUIFont* Font;
            gui::IGUIScrollBar* ScrollBar;
            bool Clip;
            bool DrawBack;
            bool MoveOverSelect;
        };


    } // end namespace gui
} // end namespace irr

#endif

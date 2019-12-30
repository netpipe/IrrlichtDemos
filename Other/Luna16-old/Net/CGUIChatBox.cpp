// Copyright (C) 2002-2008 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#include "CGUIChatBox.h"

namespace irr
{
    namespace gui
    {

//! constructor
        CGUIChatBox::CGUIChatBox(IGUIEnvironment* environment, IGUIElement* parent,
                                 s32 id, core::rect<s32> rectangle, bool clip,
                                 bool drawBack, bool moveOverSelect)
                : IGUIElement(EGUIET_ELEMENT, environment, parent, id, rectangle),
                ItemHeight(0), TotalItemHeight(0), Font(0), ScrollBar(0),
                Clip(clip), DrawBack(drawBack),
                MoveOverSelect(moveOverSelect)
        {
#ifdef _DEBUG
            setDebugName("CGUIChatBox");
#endif

            IGUISkin* skin = Environment->getSkin();
            s32 s = skin->getSize(EGDS_SCROLLBAR_SIZE);
            //ScrollBar = Environment->addScrollBar(false, core::rect<s32>(RelativeRect.getWidth() - s, 0, RelativeRect.getWidth(), RelativeRect.getHeight()), this);
            ScrollBar = environment->addScrollBar(false, core::rect<s32>(RelativeRect.getWidth() - s, 0, RelativeRect.getWidth(), RelativeRect.getHeight()),
                                                  this, -1);

            ScrollBar->setPos(0);

            recalculateItemHeight();
        }


//! destructor
        CGUIChatBox::~CGUIChatBox()
        {
            if (ScrollBar)
                ScrollBar->drop();

            if (Font)
                Font->drop();
        }



//! returns amount of list items
        s32 CGUIChatBox::getItemCount()
        {
            return Items.size();
        }



//! returns string of a list item. the may be a value from 0 to itemCount-1
        const wchar_t* CGUIChatBox::getListItem(s32 id)
        {
            if (id<0 || id>((s32)Items.size())-1)
                return 0;

            return Items[id].text.c_str();
        }



//! adds an list item, returns id of item
        s32 CGUIChatBox::addItem(const wchar_t* text)
        {
            ListItem i;
            i.text = text;
            i.color = EGDC_BUTTON_TEXT;

            breakText(i);
            Items.push_back(i);
            recalculateItemHeight();
            return Items.size() - 1;
        }



//! clears the list
        void CGUIChatBox::clear()
        {
            Items.clear();

            if (ScrollBar)
                ScrollBar->setPos(0);

            recalculateItemHeight();
        }



        void CGUIChatBox::recalculateItemHeight()
        {
            IGUISkin* skin = Environment->getSkin();

            if (Font != skin->getFont())
            {
                if (Font)
                    Font->drop();

                Font = skin->getFont();
                ItemHeight = 0;

                if (Font)
                {
                    ItemHeight = Font->getDimension(L"A").Height;
                    Font->grab();
                }

                for ( u32 i = 0 ; i < Items.size() ; ++i )
                    breakText( Items[i] );
            }

            u32 totalH = 0;
            for ( u32 i = 0 ; i < Items.size() ; ++i )
                totalH += Items[i].height;

            TotalItemHeight = totalH + 3;
            ScrollBar->setMax(TotalItemHeight - AbsoluteRect.getHeight());
        }

//! called if an event happened.
        bool CGUIChatBox::OnEvent(SEvent event)
        {
            switch (event.EventType)
            {
            case EET_GUI_EVENT:
                switch (event.GUIEvent.EventType)
                {
                case gui::EGET_SCROLL_BAR_CHANGED:
                    if (event.GUIEvent.Caller == ScrollBar)
                    {
                        // Whats this for?
                        //s32 pos = (event.GUIEvent.Caller)->getPos();
                        return true;
                    }
                    break;
                case gui::EGET_ELEMENT_FOCUS_LOST:
                {
                    return true;
                }
                break;
                default:
                    break;
                }
                break;
            case EET_MOUSE_INPUT_EVENT:
            {
                core::position2d<s32> p(event.MouseInput.X, event.MouseInput.Y);

                switch (event.MouseInput.Event)
                {
                case EMIE_MOUSE_WHEEL:
                    ScrollBar->setPos(ScrollBar->getPos() + (s32)event.MouseInput.Wheel*-10);
                    return true;

                case EMIE_LMOUSE_PRESSED_DOWN:
                    if (Environment->hasFocus(this) &&
                            ScrollBar->getAbsolutePosition().isPointInside(p) &&
                            ScrollBar->OnEvent(event))
                        return true;

                    Environment->setFocus(this);
                    return true;

                case EMIE_LMOUSE_LEFT_UP:
                    if (Environment->hasFocus(this) &&
                            ScrollBar->getAbsolutePosition().isPointInside(p) &&
                            ScrollBar->OnEvent(event))
                        return true;

                    Environment->removeFocus(this);
                    return true;
                default:
                    break;
                }
            }
            break;
            default:
                break;
            }

            return Parent ? Parent->OnEvent(event) : false;
        }

//! draws the element and its children
        void CGUIChatBox::draw()
        {
            if (!IsVisible)
                return;

            recalculateItemHeight(); // if the font changed

            IGUISkin* skin = Environment->getSkin();

            // What was this for?
            //irr::video::IVideoDriver* driver = Environment->getVideoDriver();

            core::rect<s32>* clipRect = 0;
            if (Clip)
                clipRect = &AbsoluteClippingRect;

            // draw background
            core::rect<s32> frameRect(AbsoluteRect);

            skin->draw3DSunkenPane(this, skin->getColor(EGDC_3D_HIGH_LIGHT), true, DrawBack, frameRect, clipRect);


            // draw items

            core::rect<s32> clientClip(AbsoluteRect);
            clientClip.UpperLeftCorner.Y += 1;
            clientClip.UpperLeftCorner.X += 0;
            clientClip.LowerRightCorner.X = AbsoluteRect.LowerRightCorner.X - skin->getSize(EGDS_SCROLLBAR_SIZE);
            clientClip.LowerRightCorner.Y -= 1;

            if (clipRect)
                clientClip.clipAgainst(*clipRect);

            frameRect = AbsoluteRect;
            frameRect.UpperLeftCorner.X += 1;
            frameRect.LowerRightCorner.X = AbsoluteRect.LowerRightCorner.X - skin->getSize(EGDS_SCROLLBAR_SIZE);
            frameRect.LowerRightCorner.Y = AbsoluteRect.UpperLeftCorner.Y + ItemHeight;

            frameRect.UpperLeftCorner.Y -= ScrollBar->getPos();
            frameRect.LowerRightCorner.Y -= ScrollBar->getPos();

            for (s32 i=0; i< s32(Items.size()); ++i)
            {
                if ( frameRect.UpperLeftCorner.Y + s32(Items[i].height) >= AbsoluteRect.UpperLeftCorner.Y &&
                        frameRect.UpperLeftCorner.Y <= AbsoluteRect.LowerRightCorner.Y)
                {
                    if (Font)
                    {
                        core::rect<s32> r = frameRect;
                        for (u32 j=0; j < Items[i].BrokenText.size(); ++j)
                        {
                            Font->draw(Items[i].BrokenText[j].c_str(), r, Items[i].color, false, true, &clientClip);

                            r.LowerRightCorner.Y += (s32)ItemHeight;
                            r.UpperLeftCorner.Y += (s32)ItemHeight;
                        }
                    }
                }

                //u32 test = (Items[i].BrokenText.size()) * height;
                frameRect.UpperLeftCorner.Y += Items[i].height;
                frameRect.LowerRightCorner.Y += Items[i].height;
            }

            IGUIElement::draw();
        }


        s32 CGUIChatBox::addItem(const wchar_t* text, video::SColor color)
        {
            ListItem i;
            i.text = text;
            i.color = color;

            breakText( i );
            Items.push_back(i);
            recalculateItemHeight();

            ScrollBar->setPos( ScrollBar->getMax() );

            return Items.size() - 1;
        }

        void CGUIChatBox::setItemColor(s32 index, video::SColor color)
        {
            if ((index < s32(Items.size())) && (index >= 0))
            {
                Items[index].color = color;
            }
        }

        void CGUIChatBox::breakText(ListItem &item)
        {
            IGUISkin* skin = Environment->getSkin();

            if (!skin)
                return;

            item.BrokenText.clear();

            if (!Font)
                return;

            core::stringw line;
            core::stringw word;
            core::stringw whitespace;
            s32 size = item.text.size();
            s32 length = 0;
            s32 elWidth = RelativeRect.getWidth() - ScrollBar->getRelativePosition().getWidth() - 1;
            wchar_t c;

            for (s32 i=0; i<size; ++i)
            {
                c = item.text[i];
                bool lineBreak = false;

                if (c == L'\n')
                {
                    lineBreak = true;
                    c = ' ';
                }

                if (c == L' ' || c == 0)
                {

                    if (word.size())
                    {
                        // here comes the next whitespace, look if
                        // we can break the last word to the next line.
                        s32 whitelgth = Font->getDimension(whitespace.c_str()).Width;
                        s32 worldlgth = Font->getDimension(word.c_str()).Width;

                        if (length + worldlgth + whitelgth > elWidth)
                        {
                            // break to next line
                            length = worldlgth;
                            item.BrokenText.push_back(line);
                            line = word;
                        }
                        else
                        {
                            // add word to line
                            line += whitespace;
                            line += word;
                            length += whitelgth + worldlgth;
                        }

                        word = L"";
                        whitespace = L"";
                    }

                    whitespace += c;

                    // compute line break
                    if (lineBreak)
                    {
                        line += whitespace;
                        line += word;
                        item.BrokenText.push_back(line);
                        line = L"";
                        word = L"";
                        whitespace = L"";
                        length = 0;
                    }
                }
                else
                {
                    // yippee this is a word..
                    word += c;
                }
            }

            s32 whitelgth = Font->getDimension(whitespace.c_str()).Width;
            s32 worldlgth = Font->getDimension(word.c_str()).Width;

            if (length + worldlgth + whitelgth > elWidth)
            {
                // break to next line
                length = worldlgth;
                item.BrokenText.push_back(line);
                line = word;
            }
            else
            {
                // add word to line
                line += whitespace;
                line += word;
                length += whitelgth + worldlgth;
            }

            item.BrokenText.push_back(line);

            item.height = item.BrokenText.size() * ItemHeight;
        }

    } // end namespace gui
} // end namespace irr

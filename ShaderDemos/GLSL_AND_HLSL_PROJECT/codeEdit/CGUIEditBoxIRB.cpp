// Copyright (C) 2002-2011 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

// This component was modified to code editing in IRB
// Modifications made by Christian Clavet, Andres Jesse Porfirio & Lonesome Ducky

#include "CGUIEditBoxIRB.h"
#include "IGUIScrollBar.h"

#include "IGUISkin.h"
#include "IGUIEnvironment.h"
#include "IGUIFont.h"
#include "IVideoDriver.h"
#include "IrrlichtDevice.h"
#include "rect.h"
//#include "os.h"
#include "Keycodes.h"

#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm> //For VS2013

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
	std::stringstream ss(s);
	std::string item;
	while(std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}


std::vector<std::string> split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	return split(s, delim, elems);
}

irr::core::stringw STextAction::redo(irr::core::stringw target) {
	irr::core::stringw s = target.subString(0,oldStart);
	s += newText;
	s.append(target.subString(oldStart+oldText.size(),target.size()));
	return s;
}

irr::core::stringw STextAction::undo(irr::core::stringw target) {
	irr::core::stringw s = target.subString(0,oldStart);
	s += oldText;
	s.append(target.subString(oldStart+newText.size(),target.size()));
	return s;
}

/*
todo:
optional scrollbars
ctrl+left/right to select word
double click/ctrl click: word select + drag to select whole words, triple click to select line
optional? dragging selected text
numerical
*/

namespace irr
{
namespace gui
{

//! constructor
CGUIEditBoxIRB::CGUIEditBoxIRB(const wchar_t* text, bool border, bool lines,
	IGUIEnvironment* environment, IGUIElement* parent, s32 id,
	const core::rect<s32>& rectangle, IrrlichtDevice* device)
	: IGUIElement(EGUIET_EDIT_BOX, environment, parent, id, rectangle), MouseMarking(false),
	Border(border), LineNumbering(lines), OverrideColorEnabled(false), MarkBegin(0), MarkEnd(0),
	OverrideColor(video::SColor(101,255,255,255)), OverrideFont(0), LastBreakFont(0),
	Operator(0), BlinkStartTime(0), CursorPos(0), HScrollPos(0), VScrollPos(0), Max(0),
	WordWrap(true), MultiLine(true), AutoScroll(true), PasswordBox(false),
	PasswordChar(L'*'), HAlign(EGUIA_UPPERLEFT), VAlign(EGUIA_UPPERLEFT),
	CurrentTextRect(0,0,1,1), FrameRect(rectangle), Inserting(false),
	LinecountBackColor(video::SColor(255,220,220,220)), LinecountLineColor(video::SColor(255,192,192,192)),
	LinecountTextColor(video::SColor(255,0,128,192)), SelectionBackColor(video::SColor(255,70,70,180)),
	SelectionBackColor1(video::SColor(255,130,130,180))

{
#ifdef _DEBUG
	setDebugName("CGUIEditBoxIRB");
#endif

	Text = text;
	IRRdevice = device;

	if (Environment)
		Operator = Environment->getOSOperator();

	if (Operator)
		Operator->grab();

	// this element can be tabbed to
	setTabStop(false);
	setTabOrder(-1);

	IGUISkin *skin = 0;
	if (Environment)
		skin = Environment->getSkin();

	if (Border && skin)
	{
		FrameRect.UpperLeftCorner.X += skin->getSize(EGDS_TEXT_DISTANCE_X)+1;
		FrameRect.UpperLeftCorner.Y += skin->getSize(EGDS_TEXT_DISTANCE_Y)+1;
		FrameRect.LowerRightCorner.X -= skin->getSize(EGDS_TEXT_DISTANCE_X)+1;
		FrameRect.LowerRightCorner.Y -= skin->getSize(EGDS_TEXT_DISTANCE_Y)+1;
	}
//irr::core::rect<s32> myRect(s32 x, s32 y, s32 w, s32 h);

	SelectionTextColor=skin->getColor(EGDC_HIGH_LIGHT_TEXT);
	ScrollbarH = Environment->addScrollBar(true,this->myRect(2,FrameRect.getHeight()-20,FrameRect.getWidth()-18,20),this,-1);
	Scrollbar = Environment->addScrollBar(false,this->myRect(FrameRect.getWidth()-15,2,20,FrameRect.getHeight()-22),this,-1);
	LineToggle = Environment->addButton(this->myRect(FrameRect.getWidth()-15,FrameRect.getHeight()-20,20,20),this,-1,L"#",menustring[BT_LINECOUNT].c_str());
	LineToggle->setAlignment(EGUIA_LOWERRIGHT,EGUIA_LOWERRIGHT,EGUIA_LOWERRIGHT,EGUIA_LOWERRIGHT);


	Scrollbar->setSubElement(true);
	Scrollbar->setTabStop(false);
	Scrollbar->setAlignment(EGUIA_LOWERRIGHT, EGUIA_LOWERRIGHT, EGUIA_UPPERLEFT, EGUIA_LOWERRIGHT);
	Scrollbar->setPos(0);
	Scrollbar->setSmallStep(3);

	ScrollbarH->setSubElement(true);
	ScrollbarH->setTabStop(false);
	ScrollbarH->setSmallStep(3);
	ScrollbarH->setMax(0);
	ScrollbarH->setPos(0);
	ScrollbarH->setVisible(false);

	BackgroundColor=skin->getColor(EGDC_WINDOW);

	if (MultiLine)
	{
		LineToggle->setVisible(true);
		Scrollbar->setVisible(true);
		ScrollbarH->setVisible(false);
		LineNumbering = true;
	}

	// Init the text spacing for adding GUI elements
	if (LineNumbering)
		LeftSpace = 60; // for the line counter
	else
		LeftSpace = 0;

	RightSpace = 25; // scrollbar
	UpperSpace = 0;
	LowerSpace = 0; // scrollbar

	breakText();
	calculateScrollPos();

	// Define the defaults strings for the interface
	menustring[CM_CUT]=L"Cut";
	menustring[CM_COPY]=L"Copy";
	menustring[CM_PASTE]=L"Paste";
	menustring[CM_DELETE]=L"Delete";
	menustring[CM_UNDO]=L"Undo";
	menustring[CM_REDO]=L"Redo";
	menustring[BT_LINECOUNT]=L"Toggle linecount display";
	LineToggle->setToolTipText(menustring[BT_LINECOUNT].c_str());
	//Environment->setFocus(this);
}


//! destructor
CGUIEditBoxIRB::~CGUIEditBoxIRB()
{
	if (OverrideFont)
		OverrideFont->drop();

	if (Operator)
		Operator->drop();
}

//! Sets another skin independent font.
void CGUIEditBoxIRB::setOverrideFont(IGUIFont* font)
{
	if (OverrideFont == font)
		return;

	if (OverrideFont)
		OverrideFont->drop();

	OverrideFont = font;

	if (OverrideFont)
		OverrideFont->grab();

	breakText();
}


//! Sets another color for the text.
void CGUIEditBoxIRB::setOverrideColor(video::SColor color)
{
	OverrideColor = color;
	OverrideColorEnabled = true;
}


//! Turns the border on or off
void CGUIEditBoxIRB::setDrawBorder(bool border)
{
	Border = border;
}


//! Sets if the text should use the overide color or the color in the gui skin.
void CGUIEditBoxIRB::enableOverrideColor(bool enable)
{
	OverrideColorEnabled = enable;
}


//! Enables or disables word wrap
void CGUIEditBoxIRB::setWordWrap(bool enable)
{
	WordWrap = enable;
	breakText();
}


void CGUIEditBoxIRB::updateAbsolutePosition()
{
	core::rect<s32> oldAbsoluteRect(AbsoluteRect);
	IGUIElement::updateAbsolutePosition();
	if ( oldAbsoluteRect != AbsoluteRect )
	{
		breakText();
	}
}


//! Checks if word wrap is enabled
bool CGUIEditBoxIRB::isWordWrapEnabled() const
{
	_IRR_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX;
	return WordWrap;
}


//! Enables or disables newlines.
void CGUIEditBoxIRB::setMultiLine(bool enable)
{
	MultiLine = enable;
	LineToggle->setVisible(enable);
	Scrollbar->setVisible(enable);
	//ScrollbarH->setVisible(enable);
	LineNumbering = enable;
}


//! Checks if multi line editing is enabled
bool CGUIEditBoxIRB::isMultiLineEnabled() const
{
	_IRR_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX;
	return MultiLine;
}


void CGUIEditBoxIRB::setPasswordBox(bool passwordBox, wchar_t passwordChar)
{
	PasswordBox = passwordBox;
	if (PasswordBox)
	{
		PasswordChar = passwordChar;
		setMultiLine(false);
		setWordWrap(false);
		BrokenText.clear();
	}
}


bool CGUIEditBoxIRB::isPasswordBox() const
{
	_IRR_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX;
	return PasswordBox;
}


//! Sets text justification
void CGUIEditBoxIRB::setTextAlignment(EGUI_ALIGNMENT horizontal, EGUI_ALIGNMENT vertical)
{
	HAlign = horizontal;
	VAlign = vertical;
}


void CGUIEditBoxIRB::setBackgroundColor(video::SColor color)
{
	BackgroundColor = color;
}

void CGUIEditBoxIRB::setLineCountColors(irr::video::SColor background, irr::video::SColor line, irr::video::SColor text)
{
	LinecountBackColor=background;
	LinecountLineColor=line;
	LinecountTextColor=text;
}

void CGUIEditBoxIRB::setDisplayLineCount(bool display)
{
	if (!display)
		{
			LineNumbering=false;
			LeftSpace=0;
			//irr::core::rect<s32> myRect(s32 x, s32 y, s32 w, s32 h);
			core::rect<s32> localClipRect = this->myRect(FrameRect.UpperLeftCorner.X+LeftSpace,FrameRect.UpperLeftCorner.Y,FrameRect.getWidth()-(LeftSpace/2),FrameRect.getHeight());
			localClipRect.clipAgainst(AbsoluteClippingRect);
			breakText();
		}

		else
		{
			LineNumbering=true;
			LeftSpace=60;
			//irr::core::rect<s32> myRect(s32 x, s32 y, s32 w, s32 h);
			core::rect<s32> localClipRect = this->myRect(FrameRect.UpperLeftCorner.X+LeftSpace,FrameRect.UpperLeftCorner.Y,FrameRect.getWidth()-(LeftSpace/2),FrameRect.getHeight());
			localClipRect.clipAgainst(AbsoluteClippingRect);
			breakText();
		}
}

//! Display the current linecount GUI display status
bool CGUIEditBoxIRB::isLineCountDisplayed()
{
	return LineNumbering;
}
void CGUIEditBoxIRB::setElementText(u32 index, core::stringw text)
{
	if (index<GUI_COUNT+1)
		menustring[index]=text.c_str();

	// If the Element is the tooltip of the toggle of linecount, set it at the same time.
	if (index==BT_LINECOUNT)
		LineToggle->setToolTipText(text.c_str());
}

//! Set the selection colors
void CGUIEditBoxIRB::setSelectionColors(video::SColor background, video::SColor text, video::SColor background1)
{
	SelectionBackColor = background;
	SelectionBackColor1 = background1;
	SelectionTextColor = text;

}

//! called if an event happened.
bool CGUIEditBoxIRB::OnEvent(const SEvent& event)
{
	if (IsEnabled && IsVisible)
	{

		switch(event.EventType)
		{
		case EET_GUI_EVENT:
			if (event.GUIEvent.EventType == irr::gui::EGET_MENU_ITEM_SELECTED)
			{
				int sel = static_cast<IGUIContextMenu*>(event.GUIEvent.Caller)->getSelectedItem();

				if (sel == CM_CUT)
					cut();
				else if (sel == CM_COPY)
					copy();
				else if (sel == CM_PASTE)
					paste();
				else if (sel == CM_DELETE)
					deleteText();
				else if (sel == CM_UNDO)
					undo();
				else if (sel == CM_REDO)
					redo();


				//Environment->setFocus(this);
				InMenu = false;
			}
			if (event.GUIEvent.EventType == EGET_ELEMENT_FOCUS_LOST)
			{
				if (event.GUIEvent.Caller == this)
				{
					MouseMarking = false;
					OldMarkBegin = MarkBegin;
					OldMarkEnd = MarkEnd;
					//setTextMarkers(0,0);
					//Environment->setFocus(this);
				}
				break;
			}
			if (event.GUIEvent.EventType == EGET_EDITBOX_ENTER)
			{
				if (event.GUIEvent.Caller == this)
				{

				}
				break;
			}

			if (event.GUIEvent.EventType == EGET_SCROLL_BAR_CHANGED)
			{

				if (event.GUIEvent.Caller == Scrollbar)
				{
					if (MultiLine || (WordWrap && BrokenText.size() > 1) )
					{
						IGUISkin* skin = Environment->getSkin();
						IGUIFont* font = OverrideFont;
						if (!OverrideFont)
							font = skin->getFont();
						s32 scrollMove = font->getDimension(L"O").Height;
						VScrollPos = (s32)Scrollbar->getPos()*scrollMove;


					}
				}
				break;
			}

			if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED)
			{
				if (event.GUIEvent.Caller == LineToggle)
				{
					if (LineNumbering)
					{
						LineNumbering=false;
						breakText();
						LeftSpace=0;
						//irr::core::rect<s32> myRect(s32 x, s32 y, s32 w, s32 h);
						core::rect<s32> localClipRect = this->myRect(FrameRect.UpperLeftCorner.X+LeftSpace,FrameRect.UpperLeftCorner.Y,FrameRect.getWidth()-(LeftSpace/2),FrameRect.getHeight());
						localClipRect.clipAgainst(AbsoluteClippingRect);
					}

					else
					{
						LineNumbering=true;
						breakText();
						LeftSpace=60;
						//irr::core::rect<s32> myRect(s32 x, s32 y, s32 w, s32 h);
						core::rect<s32> localClipRect = this->myRect(FrameRect.UpperLeftCorner.X+LeftSpace,FrameRect.UpperLeftCorner.Y,FrameRect.getWidth()-(LeftSpace/2),FrameRect.getHeight());
						localClipRect.clipAgainst(AbsoluteClippingRect);
					}
				}

			}
			break;

		case EET_KEY_INPUT_EVENT:
			if (processKey(event))
				return true;
			break;

		case EET_MOUSE_INPUT_EVENT:
			if (processMouse(event))
				return true;
			break;

		default:
			break;
		}
	}

	return IGUIElement::OnEvent(event);
}


bool CGUIEditBoxIRB::processKey(const SEvent& event)
{
	if (!event.KeyInput.PressedDown)
		return false;

	bool textChanged = false;
	s32 newMarkBegin = MarkBegin;
	s32 newMarkEnd = MarkEnd;

	// control shortcut handling

	if (event.KeyInput.Control)
	{
		// german backlash '\' entered with control + '?'
		if ( event.KeyInput.Char == '\\' )
		{
			inputChar(event.KeyInput.Char);
			return true;
		}

		switch(event.KeyInput.Key)
		{

		case KEY_KEY_Z:
			undo();
			break;
		case KEY_KEY_Y:
			redo();
			break;
		case KEY_KEY_A:
			// select all
			newMarkBegin = 0;
			newMarkEnd = Text.size();
			break;
		case KEY_KEY_C:
			// copy to clipboard
			copy();
			break;
		case KEY_KEY_X:
			// cut to the clipboard
			cut();
			return true;
		case KEY_KEY_V:
			paste();
			return true;
		case KEY_HOME:
			// move/highlight to start of text
			if (event.KeyInput.Shift)
			{
				newMarkEnd = CursorPos;
				newMarkBegin = 0;
				CursorPos = 0;
			}
			else
			{
				CursorPos = 0;
				newMarkBegin = 0;
				newMarkEnd = 0;
			}
			break;
		case KEY_END:
			// move/highlight to end of text
			if (event.KeyInput.Shift)
			{
				newMarkBegin = CursorPos;
				newMarkEnd = Text.size();
				CursorPos = 0;
			}
			else
			{
				CursorPos = Text.size();
				newMarkBegin = 0;
				newMarkEnd = 0;
			}
			break;
		default:
			return false;
		}
	}
	// default keyboard handling
	else
		switch(event.KeyInput.Key)
	{
		case KEY_END:
			{
				s32 p = Text.size();
				if (WordWrap || MultiLine)
				{
					p = getLineFromPos(CursorPos);
					p = BrokenTextPositions[p] + (s32)BrokenText[p].size();
					if (p > 0 && (Text[p-1] == L'\r' || Text[p-1] == L'\n' ))
						p-=1;
				}

				if (event.KeyInput.Shift)
				{
					if (MarkBegin == MarkEnd)
						newMarkBegin = CursorPos;

					newMarkEnd = p;
				}
				else
				{
					newMarkBegin = 0;
					newMarkEnd = 0;
				}
				CursorPos = p;
				BlinkStartTime = IRRdevice->getTimer()->getRealTime();
			}
			break;
		case KEY_HOME:
			{

				s32 p = 0;
				if (WordWrap || MultiLine)
				{
					p = getLineFromPos(CursorPos);
					p = BrokenTextPositions[p];
				}

				if (event.KeyInput.Shift)
				{
					if (MarkBegin == MarkEnd)
						newMarkBegin = CursorPos;
					newMarkEnd = p;
				}
				else
				{
					newMarkBegin = 0;
					newMarkEnd = 0;
				}
				CursorPos = p;
				BlinkStartTime = IRRdevice->getTimer()->getRealTime();

			}
			break;
		case KEY_RETURN:
			if (MultiLine)
			{
				sendGuiEvent( EGET_EDITBOX_ENTER );
				s32 lineStart = getLineFromPos(CursorPos);
				lineStart = BrokenTextPositions[lineStart];

				s32 lineEnd = CursorPos;

				inputChar(L'\n');

				for (int i=lineStart ; i<=lineEnd ; i++)
				{
					if( Text.subString(i,1) == irr::core::stringw(" ") )
					{
						inputChar(L' ');
					}
					else
						break;
				}

				return true;
			}
			else
			{
				sendGuiEvent( EGET_EDITBOX_ENTER );
			}
			break;
		case KEY_LEFT:

			if (event.KeyInput.Shift)
			{
				if (CursorPos > 0)
				{
					if (MarkBegin == MarkEnd)
						newMarkBegin = CursorPos;

					newMarkEnd = CursorPos-1;
				}
			}
			else
			{
				newMarkBegin = 0;
				newMarkEnd = 0;
			}

			if (CursorPos > 0)
				CursorPos--;
			BlinkStartTime = IRRdevice->getTimer()->getRealTime();
			break;

		case KEY_RIGHT:
			if (event.KeyInput.Shift)
			{
				if (Text.size() > (u32)CursorPos)
				{
					if (MarkBegin == MarkEnd)
						newMarkBegin = CursorPos;

					newMarkEnd = CursorPos+1;
				}
			}
			else
			{
				newMarkBegin = 0;
				newMarkEnd = 0;
			}

			if (Text.size() > (u32)CursorPos)
				CursorPos++;
			BlinkStartTime = IRRdevice->getTimer()->getRealTime();
			break;

		case KEY_UP:
			if (MultiLine || (WordWrap && BrokenText.size() > 1) )
			{
				s32 lineNo = getLineFromPos(CursorPos);
				s32 mb = (MarkBegin == MarkEnd) ? CursorPos : (MarkBegin > MarkEnd ? MarkBegin : MarkEnd);
				if (lineNo > 0)
				{
					s32 cp = CursorPos - BrokenTextPositions[lineNo];
					if ((s32)BrokenText[lineNo-1].size() < cp)
						CursorPos = BrokenTextPositions[lineNo-1] + (s32)BrokenText[lineNo-1].size()-1;
					else
						CursorPos = BrokenTextPositions[lineNo-1] + cp;
				}

				if (event.KeyInput.Shift)
				{
					newMarkBegin = mb;
					newMarkEnd = CursorPos;
				}
				else
				{
					newMarkBegin = 0;
					newMarkEnd = 0;
				}
				//Scrollbar->setPos(getLineFromPos(CursorPos));
			}
			else
			{
				return false;
			}
			break;

		case KEY_DOWN:
			if (MultiLine || (WordWrap && BrokenText.size() > 1) )
			{
				s32 lineNo = getLineFromPos(CursorPos);
				s32 mb = (MarkBegin == MarkEnd) ? CursorPos : (MarkBegin < MarkEnd ? MarkBegin : MarkEnd);
				if (lineNo < (s32)BrokenText.size()-1)
				{
					s32 cp = CursorPos - BrokenTextPositions[lineNo];
					if ((s32)BrokenText[lineNo+1].size() < cp)
						CursorPos = BrokenTextPositions[lineNo+1] + BrokenText[lineNo+1].size()-1;
					else
						CursorPos = BrokenTextPositions[lineNo+1] + cp;
				}

				if (event.KeyInput.Shift)
				{
					newMarkBegin = mb;
					newMarkEnd = CursorPos;
				}
				else
				{
					newMarkBegin = 0;
					newMarkEnd = 0;
				}
				//Scrollbar->setPos(getLineFromPos(CursorPos));

			}
			else
			{
				return false;
			}
			break;

		case KEY_BACK:
			if ( !this->IsEnabled )
				break;

			if (Text.size())
			{
				core::stringw s;

				if (MarkBegin != MarkEnd)
				{
					// delete marked text
					const s32 realmbgn = MarkBegin < MarkEnd ? MarkBegin : MarkEnd;
					const s32 realmend = MarkBegin < MarkEnd ? MarkEnd : MarkBegin;

					replaceText(realmbgn,realmend,L"");

					CursorPos = realmbgn;
				}
				else
				{
					// delete text behind cursor
					if (CursorPos>0)
						replaceText(CursorPos-1,CursorPos,L"");
					else
						replaceText(0,CursorPos,L"");
					--CursorPos;
				}

				if (CursorPos < 0)
					CursorPos = 0;

				BlinkStartTime = IRRdevice->getTimer()->getRealTime();
				newMarkBegin = 0;
				newMarkEnd = 0;
				textChanged = true;
			}
			break;

		case KEY_DELETE:
			if ( !this->IsEnabled )
				break;

			if (Text.size() != 0)
			{
				core::stringw s;

				if (MarkBegin != MarkEnd)
				{
					// delete marked text
					const s32 realmbgn = MarkBegin < MarkEnd ? MarkBegin : MarkEnd;
					const s32 realmend = MarkBegin < MarkEnd ? MarkEnd : MarkBegin;

					replaceText(realmbgn,realmend,L"");

					CursorPos = realmbgn;
				}
				else
				{
					// delete text before cursor
					replaceText(CursorPos,CursorPos+1,L"");
				}

				if (CursorPos > (s32)Text.size())
					CursorPos = (s32)Text.size();

				BlinkStartTime = IRRdevice->getTimer()->getRealTime();
				newMarkBegin = 0;
				newMarkEnd = 0;
				textChanged = true;
			}
			break;

		case KEY_TAB:
			/*inputChar(wchar_t(static_cast<char>( 9 )));
			//if (Text.size() > (u32)CursorPos)
			//	CursorPos+=3;*/
			inputChar(L' ');
			inputChar(L' ');
			inputChar(L' ');
			inputChar(L' ');
			break;
		case KEY_INSERT:
			Inserting = !Inserting;
			break;
		case KEY_ESCAPE:
		case KEY_SHIFT:
		case KEY_F1:
		case KEY_F2:
		case KEY_F3:
		case KEY_F4:
		case KEY_F5:
		case KEY_F6:
		case KEY_F7:
		case KEY_F8:
		case KEY_F9:
		case KEY_F10:
		case KEY_F11:
		case KEY_F12:
		case KEY_F13:
		case KEY_F14:
		case KEY_F15:
		case KEY_F16:
		case KEY_F17:
		case KEY_F18:
		case KEY_F19:
		case KEY_F20:
		case KEY_F21:
		case KEY_F22:
		case KEY_F23:
		case KEY_F24:
			// ignore these keys
			return false;

		default:
			// ` ' and " must be replaced to ' (lua interpreter does not recognize wrong chars)
			/*if(event.KeyInput.Char == 96 || event.KeyInput.Char == 180 || event.KeyInput.Char == 168)
				inputChar(L'"');
			else*/
				inputChar(event.KeyInput.Char);
			//printf("%s",(core::stringc)event.KeyInput.Char);
			return true;
	}

	// Set new text markers
	setTextMarkers( newMarkBegin, newMarkEnd );

	// break the text if it has changed
	if (textChanged)
	{
		breakText();
		sendGuiEvent(EGET_EDITBOX_CHANGED);
	}

	calculateScrollPos();

	return true;
}


//! draws the element and its children
void CGUIEditBoxIRB::draw()
{

	//calculateScrollPos();

	if (!IsVisible)
		return;

	const bool focus = Environment->hasFocus(this);

	IGUISkin* skin = Environment->getSkin();
	if (!skin)
		return;

	bool menuOpen = false;
	irr::core::list<IGUIElement*> children = getChildren();
	for (irr::core::list<IGUIElement*>::ConstIterator i = children.begin(); i != children.end(); i++) {
		if ((*i)->getType() == irr::gui::EGUIET_CONTEXT_MENU)
			menuOpen = true;
	}
	InMenu = menuOpen;

	// Redefinition seem not needed on MSVC
	//irr::core::rect<s32> myRect(s32 x, s32 y, s32 w, s32 h);

	FrameRect = AbsoluteRect;
	// draw the border

	if (Border)
	{
		skin->draw3DSunkenPane(this, BackgroundColor,
			false, true, this->myRect(FrameRect.UpperLeftCorner.X-1,FrameRect.UpperLeftCorner.Y-2,FrameRect.getWidth(),FrameRect.getHeight()), &AbsoluteClippingRect);

		FrameRect.UpperLeftCorner.X += skin->getSize(EGDS_TEXT_DISTANCE_X)+1;
		FrameRect.UpperLeftCorner.Y += skin->getSize(EGDS_TEXT_DISTANCE_Y)+1;
		FrameRect.LowerRightCorner.X -= skin->getSize(EGDS_TEXT_DISTANCE_X)+1;
		FrameRect.LowerRightCorner.Y -= skin->getSize(EGDS_TEXT_DISTANCE_Y)+1;

	}

	if (LineNumbering)
	{
		// Line count background
		skin->draw2DRectangle(this,LinecountBackColor,this->myRect(FrameRect.UpperLeftCorner.X,FrameRect.UpperLeftCorner.Y,50,FrameRect.getHeight()-4),&AbsoluteClippingRect);
		skin->draw2DRectangle(this,LinecountLineColor,this->myRect(FrameRect.UpperLeftCorner.X+45,FrameRect.UpperLeftCorner.Y,5,FrameRect.getHeight()-4),&AbsoluteClippingRect);
	}

	core::rect<s32> localClipRect = this->myRect(FrameRect.UpperLeftCorner.X+LeftSpace,FrameRect.UpperLeftCorner.Y,FrameRect.getWidth()-(LeftSpace/2),FrameRect.getHeight());

	localClipRect.clipAgainst(AbsoluteClippingRect);

	// draw the text

	IGUIFont* font = OverrideFont;
	if (!OverrideFont)
		font = skin->getFont();

	s32 cursorLine = 0;
	s32 charcursorpos = 0;

	if (font)
	{
		if (LastBreakFont != font)
		{
			breakText();
		}

		// calculate cursor pos

		core::stringw *txtLine = &Text;
		s32 startPos = 0;

		core::stringw s, s2;

		// get mark position
		const bool ml = (!PasswordBox && (WordWrap || MultiLine));
		const s32 realmbgn = MarkBegin < MarkEnd ? MarkBegin : MarkEnd;
		const s32 realmend = MarkBegin < MarkEnd ? MarkEnd : MarkBegin;
		const s32 hlineStart = ml ? getLineFromPos(realmbgn) : 0;
		const s32 hlineCount = ml ? getLineFromPos(realmend) - hlineStart + 1 : 1;
		//const s32 lineCount = ml ? BrokenText.size() : 1;
		lineCount = ml ? BrokenText.size() : 1;

		// Save the override color information.
		// Then, alter it if the edit box is disabled.
		const bool prevOver = OverrideColorEnabled;
		const video::SColor prevColor = OverrideColor;

		bool lineDrawingDone = false;

		SearchingForEndOfGroup = false;

		int numberStartY = -1;
		int numberEndY = -1;
		core::stringw numberText;

		if (Text.size())
		{
			if (!IsEnabled && !OverrideColorEnabled)
			{
				OverrideColorEnabled = true;
				OverrideColor = skin->getColor(EGDC_GRAY_TEXT);
			}

			for (s32 i=0; i < lineCount; ++i)
			{

				int height = font->getDimension(L"0").Height;

				setTextRect(i);
				while (CurrentTextRect.LowerRightCorner.Y < AbsoluteRect.UpperLeftCorner.Y)
				{
					setTextRect(++i);
				}
				if (numberStartY == -1)
					numberStartY = CurrentTextRect.LowerRightCorner.Y - height;
				if (i >= lineCount || CurrentTextRect.UpperLeftCorner.Y > AbsoluteRect.LowerRightCorner.Y)
					break;
				if (i+1 >= lineCount || CurrentTextRect.UpperLeftCorner.Y+height > AbsoluteRect.LowerRightCorner.Y) {
					numberEndY = CurrentTextRect.LowerRightCorner.Y;
				}

				numberText += i+1;
				numberText += L"\r\n";

				// get current line
				if (PasswordBox)
				{
					if (BrokenText.size() != 1)
					{
						BrokenText.clear();
						BrokenText.push_back(core::stringw());
					}
					if (BrokenText[0].size() != Text.size())
					{
						BrokenText[0] = Text;
						for (u32 q = 0; q < Text.size(); ++q)
						{
							BrokenText[0] [q] = PasswordChar;
						}
					}
					txtLine = &BrokenText[0];
					startPos = 0;
				}
				else
				{
					txtLine = ml ? &BrokenText[i] : &Text;
					startPos = ml ? BrokenTextPositions[i] : 0;
				}

				// Parse the TAB Character on Widestrings
				core::stringw txt_main = *txtLine;
				core::stringw temp=L"";
				for (int chart=0; chart < (int)txt_main.size(); chart++)
				{
					if (txt_main[chart] == static_cast<char>( 9 ))
					{	
						core::stringw temp1=txt_main.subString(0,chart-1);
						core::stringw temp2=txt_main.subString(chart+1,txt_main.size());
						temp=temp1.append(" ");
						temp.append(temp2);
					} 
				}
				if (temp.size()>0)
					txtLine = &temp;
				else
					txtLine = &txt_main;

				bool endFound = false;
				bool startFound = false;
				bool multiLine = false;
				int startPosX = 0;
				u32 startIndex = 0;
				irr::core::stringc endText = "";

				// Create an array of rects that say where the keywords are at
				irr::core::array<SKeywordOverlay> keywordOverlays;

				irr::core::stringc lineCStr = core::stringc(txtLine->c_str());
				if (!SearchingForEndOfGroup) {
					for (u32 u = 0; u < lineCStr.size(); u++) {
						// Keywords will be split by alpha numerical characters
						// Find the end of this current word
						u32 end = u;
						for (u32 v = u; v < lineCStr.size(); v++) {
							// keyword must be in english
							if ((int)lineCStr.c_str()[v]<0 || (int)lineCStr.c_str()[v]>128) {
								end = v;
								break;
							}
							// check if not alpha or digit
							if (!isalnum(lineCStr.c_str()[v])) {
								end = v;
								break;
							}
							if (v == lineCStr.size()-1)
								end = v+1;
						}

						int index1, index2, index3;
						index1 = index2 = index3 = 0;

						// We will be checking for all keywords together, the biggest come first
						for (u32 v = 0; v < KeywordList.size(); v++) {

							// Check normal keywords
							if (KeywordList[v].type == 0) {


								//Keywords[v] problem retrieve
								//core::stringc test = Keywords[v].text;

								if ((end-u) == Keywords[v].text.size()) {
									// Compare the strings
									bool same = true;
									for (u32 w = 0; w < end-u; w++) {
										if (lineCStr[w+u] != Keywords[v].text.c_str()[w]) {
											same = false;
										}
									}
									// If they are the same, add this text to the list
									if (same) {
										SKeywordOverlay overlay;
										overlay.color = Keywords[v].color;
										overlay.text = Keywords[v].text;
										irr::core::stringw prevText = L"";
										for (u32 w = 0; w < u; w++) {
											prevText += txtLine->c_str()[w];
										}
										int startX = font->getDimension(prevText.c_str()).Width;
										int width = font->getDimension(overlay.text.c_str()).Width;
										irr::core::recti textRect = CurrentTextRect;
										textRect.UpperLeftCorner.X += startX;
										textRect.LowerRightCorner.X = textRect.UpperLeftCorner.X+width;
										overlay.rect = textRect;
										if (textRect.UpperLeftCorner.X < AbsoluteRect.LowerRightCorner.X && textRect.LowerRightCorner.X > AbsoluteRect.UpperLeftCorner.X)
											keywordOverlays.push_back(overlay);
										u = end;
										if (u == lineCStr.size())
											break;
									}
								}
								index1++;
							}

							// Check group keywords
							else if (KeywordList[v].type == 1) {

								// Only check if there is enough space left for there to be the keyword
								if (u+GroupKeywords[index2].text.size() <= lineCStr.size()) {
									bool match = true;
									int begIndex = 0;
									for (u32 w = 0; w < GroupKeywords[index2].text.size(); w++) {
										begIndex = u+w;
										if (GroupKeywords[index2].text.c_str()[w] != lineCStr.c_str()[u+w]) {
											match = false;
											break;
										}
									}
									if (match) {
										multiLine = GroupKeywords[index2].multiLine;
										startFound = true;
										GroupColor = GroupKeywords[index2].color;
										endText = GroupKeywords[index2].endText;

										// Find where our text begins
										irr::core::stringw behind;
										for (u32 x = 0; x < begIndex-(GroupKeywords[index2].text.size()-1); x++) {
											behind += txtLine->c_str()[x];
										}
										startPosX = font->getDimension(behind.c_str()).Width;
										startIndex = begIndex-(GroupKeywords[index2].text.size()-1);

										// If we do find the end, set u to that + 1 so we don't mistake the end for the start if they are both the same string
										for (u32 w = begIndex+1; w < lineCStr.size(); w++) {
											// Look for the end
											bool matchEnd = true;
											u32 endIndex = u+1;
											for (u32 x = 0; x < GroupKeywords[index2].endText.size(); x++) {
												endIndex = w+x;
												if (GroupKeywords[index2].endText.c_str()[x] != lineCStr.c_str()[w+x]) {
													matchEnd = false;
													break;
												}
											}

											// If a match was found, create the overlay
											if (matchEnd) {
												endFound = true;
												SKeywordOverlay overlay;
												overlay.color = GroupKeywords[index2].color;
												irr::core::stringw prevText = L"";
												irr::core::stringw thisText = L"";
												for (u32 x = 0; x < endIndex+1; x++) {
													if (x < begIndex-(GroupKeywords[index2].text.size()-1))
														prevText += txtLine->c_str()[x];
													else
														thisText += txtLine->c_str()[x];
												}
												overlay.text = thisText;
												int startX = font->getDimension(prevText.c_str()).Width;
												int width = font->getDimension(overlay.text.c_str()).Width;
												irr::core::recti textRect = CurrentTextRect;
												textRect.UpperLeftCorner.X += startX;
												textRect.LowerRightCorner.X = textRect.UpperLeftCorner.X+width;
												overlay.rect = textRect;
												if (textRect.UpperLeftCorner.X < AbsoluteRect.LowerRightCorner.X && textRect.LowerRightCorner.X > AbsoluteRect.UpperLeftCorner.X)
													keywordOverlays.push_back(overlay);
												u = endIndex;

												startFound = false;
												endFound = false;

												break;

											}

										}

									}
								}

								index2++;
							}
							else if (KeywordList[v].type == 2) {
							// Only check if there is enough space left for there to be the keyword
								if (u+LineKeywords[index3].text.size() < lineCStr.size()) {
									bool match = true;
									for (u32 w = 0; w < LineKeywords[index3].text.size(); w++) {
										if (LineKeywords[index3].text.c_str()[w] != lineCStr.c_str()[u+w]) {
											match = false;
											break;
										}
									}
									// If a match was found, color the entire line
									if (match) {
										SKeywordOverlay overlay;
										overlay.color = LineKeywords[index3].color;
										irr::core::stringw prevText = L"";
										irr::core::stringw thisText = L"";
										for (u32 w = 0; w < lineCStr.size(); w++) {
											if (w < u)
												prevText += txtLine->c_str()[w];
											else
												thisText += txtLine->c_str()[w];
										}
										overlay.text = thisText;
										int startX = font->getDimension(prevText.c_str()).Width;
										int width = font->getDimension(overlay.text.c_str()).Width;
										irr::core::recti textRect = CurrentTextRect;
										textRect.UpperLeftCorner.X += startX;
										textRect.LowerRightCorner.X = textRect.UpperLeftCorner.X+width;
										overlay.rect = textRect;
										if (textRect.UpperLeftCorner.X < AbsoluteRect.LowerRightCorner.X && textRect.LowerRightCorner.X > AbsoluteRect.UpperLeftCorner.X)
											keywordOverlays.push_back(overlay);
										u = lineCStr.size()-1;
										break;
									}
								}
								index3++;
							}
						}






					}
				}




				// If we haven't found the end of the group we last had, keep looking
				if (SearchingForEndOfGroup) {
					bool totalMatch = false;
					for (u32 u = 0; u < lineCStr.size(); u++) {
						bool match = true;
						if (u+EndOfGroupText.size() <= lineCStr.size()) {
							for (u32 w = 0; w < EndOfGroupText.size(); w++) {
								if (EndOfGroupText.c_str()[w] != lineCStr.c_str()[u+w]) {
									match = false;
									break;
								}
							}
							if (match) {
								totalMatch = true;
								SKeywordOverlay overlay;
								overlay.color = GroupColor;
								irr::core::stringw prevText = L"";
								irr::core::stringw thisText = L"";
								for (u32 x = 0; x < u+EndOfGroupText.size(); x++) {
								    thisText += txtLine->c_str()[x];
								}
								overlay.text = thisText;
								int startX = 0;
								int width = font->getDimension(overlay.text.c_str()).Width;
								irr::core::recti textRect = CurrentTextRect;
								textRect.UpperLeftCorner.X += startX;
								textRect.LowerRightCorner.X = textRect.UpperLeftCorner.X+width;
								overlay.rect = textRect;
								if (textRect.UpperLeftCorner.X < AbsoluteRect.LowerRightCorner.X && textRect.LowerRightCorner.X > AbsoluteRect.UpperLeftCorner.X)
									keywordOverlays.push_back(overlay);
								SearchingForEndOfGroup = false;
								break;
							}
						}
						else
							match = false;
					}
					// If we didn't find a match, color the whole line. Otherwise, it will already have been added
					if (!totalMatch) {
						SKeywordOverlay overlay;
						overlay.color = GroupColor;
						overlay.text =txtLine->c_str();
						int startX = 0;
						int width = font->getDimension(txtLine->c_str()).Width;
						irr::core::recti textRect = CurrentTextRect;
						textRect.UpperLeftCorner.X += startX;
						textRect.LowerRightCorner.X = textRect.UpperLeftCorner.X+width;
						overlay.rect = textRect;
						if (textRect.UpperLeftCorner.X < AbsoluteRect.LowerRightCorner.X && textRect.LowerRightCorner.X > AbsoluteRect.UpperLeftCorner.X)
							keywordOverlays.push_back(overlay);
					}
				}

				if (startFound && !endFound) {
					// If we've matched and we're still here, we did not find the end, so color in what we have
					SKeywordOverlay overlay;
					overlay.color = GroupColor;
					irr::core::stringw thisText = L"";
					for (u32 x = startIndex; x < lineCStr.size(); x++) {
						thisText += txtLine->c_str()[x];
					}
					overlay.text = thisText;
					int width = font->getDimension(overlay.text.c_str()).Width;
					irr::core::recti textRect = CurrentTextRect;
					textRect.LowerRightCorner.X = textRect.UpperLeftCorner.X+width;
					textRect.UpperLeftCorner.X += startPosX;

					overlay.rect = textRect;
					if (textRect.UpperLeftCorner.X < AbsoluteRect.LowerRightCorner.X && textRect.LowerRightCorner.X > AbsoluteRect.UpperLeftCorner.X)
						keywordOverlays.push_back(overlay);
					if (multiLine)
						SearchingForEndOfGroup = true;
					EndOfGroupText = endText;
				}


				// Draw line numbering
				if (LineNumbering)
				{
					if (i+1 >= lineCount || CurrentTextRect.UpperLeftCorner.Y+height > AbsoluteRect.LowerRightCorner.Y) {
						s32 right = FrameRect.UpperLeftCorner.X+3;
						irr::core::recti drawRect(right,numberStartY,right+50,numberEndY);
						font->draw(numberText.c_str(),
							drawRect,
							LinecountTextColor,
							false, false, &AbsoluteClippingRect);
					}
				}
				// draw normal text
				//font->draw(core::stringw(txt_main.c_str()), CurrentTextRect,
				//   OverrideColorEnabled ? OverrideColor : skin->getColor(EGDC_BUTTON_TEXT),
				//video::SColor(255,255,0,0),
				//   false, true, &localClipRect);
				font->draw(txtLine->c_str(), CurrentTextRect,
					OverrideColorEnabled ? OverrideColor : skin->getColor(EGDC_BUTTON_TEXT),
					false, true, &localClipRect);

				// Draw keywords
				for (u32 u = 0; u < keywordOverlays.size(); u++) {
					font->draw(keywordOverlays[u].text.c_str(),keywordOverlays[u].rect,keywordOverlays[u].color,false,true,&localClipRect);
				}

				// draw mark and marked text
				if (InMenu || Environment->getFocus() == Scrollbar) {
					setTextMarkers(OldMarkBegin,OldMarkEnd);
				}
				if ((focus || InMenu || Environment->getFocus() == Scrollbar) && MarkBegin != MarkEnd && i >= hlineStart && i < hlineStart + hlineCount)
				{

					s32 mbegin = 0, mend = 0;
					s32 lineStartPos = 0, lineEndPos = txtLine->size();

					if (i == hlineStart)
					{
						// highlight start is on this line
						s = txtLine->subString(0, realmbgn - startPos);
						mbegin = font->getDimension(s.c_str()).Width;

						// deal with kerning
						mbegin += font->getKerningWidth(
							&((*txtLine)[realmbgn - startPos]),
							realmbgn - startPos > 0 ? &((*txtLine)[realmbgn - startPos - 1]) : 0);

						lineStartPos = realmbgn - startPos;
					}
					if (i == hlineStart + hlineCount - 1)
					{
						// highlight end is on this line
						s2 = txtLine->subString(0, realmend - startPos);
						mend = font->getDimension(s2.c_str()).Width;
						lineEndPos = (s32)s2.size();
					}
					else
						mend = font->getDimension(txtLine->c_str()).Width;

					CurrentTextRect.UpperLeftCorner.X += mbegin;
					CurrentTextRect.LowerRightCorner.X = CurrentTextRect.UpperLeftCorner.X + mend - mbegin;

					// draw mark
					irr::video::SColor SelColor =  SelectionBackColor;
						//selColor(255,70,70,180);
					if (InMenu)
						SelColor = SelectionBackColor1;
						//irr::video::SColor(255,130,130,180);

					skin->draw2DRectangle(this, SelColor, CurrentTextRect, &localClipRect);

					// draw marked text
					s = txtLine->subString(lineStartPos, lineEndPos - lineStartPos);

					if (s.size())
						font->draw(s.c_str(), CurrentTextRect,
						SelectionTextColor,
						false, true, &localClipRect);

				}
			}

			// Return the override color information to its previous settings.
			OverrideColorEnabled = prevOver;
			OverrideColor = prevColor;
		}

		// draw cursor

		if (WordWrap || MultiLine)
		{
			cursorLine = getLineFromPos(CursorPos);
			txtLine = &BrokenText[cursorLine];
			startPos = BrokenTextPositions[cursorLine];
		}
		s = txtLine->subString(0,CursorPos-startPos);
		// Ugly fix to a bug when the mouse was going outside the limit of the text in selections.
		// Checking that the "txtline" size is bigger than 5 to redefine the position of the cursor
		// Was crashing for a position NOT allocated in the line.
		if (txtLine->size()>0) {
			int index = std::min((int)txtLine->size(),CursorPos-startPos-1);
			charcursorpos = font->getDimension(s.c_str()).Width +
			font->getKerningWidth(L"_", CursorPos-startPos > 0 ? &((*txtLine)[index]) : 0);
		}

		//if (focus && (os::Timer::getTime() - BlinkStartTime) % 700 < 350)
		if (focus && (IRRdevice->getTimer()->getRealTime() - BlinkStartTime) % 1000 < 500)
		{
			setTextRect(cursorLine);
			CurrentTextRect.UpperLeftCorner.X += charcursorpos;
			Environment->getVideoDriver()->draw2DLine(core::position2di(CurrentTextRect.UpperLeftCorner.X,CurrentTextRect.UpperLeftCorner.Y),core::position2di(CurrentTextRect.UpperLeftCorner.X,CurrentTextRect.LowerRightCorner.Y),irr::video::SColor(255,0,0,0));
			if (Inserting) {
				irr::core::recti blinkRect = CurrentTextRect;
				blinkRect.LowerRightCorner.X = blinkRect.UpperLeftCorner.X + font->getDimension(L"_").Width;
				Environment->getVideoDriver()->draw2DRectangle(irr::video::SColor(100,100,100,100),blinkRect,&localClipRect);
			}

			/*font->draw(L"_", CurrentTextRect,
				OverrideColorEnabled ? OverrideColor : skin->getColor(EGDC_BUTTON_TEXT),
				false, true, &localClipRect);*/
		}  // To be fixed
	}

	// draw children
	IGUIElement::draw();
}


//! Sets the new caption of this element.
void CGUIEditBoxIRB::setText(const wchar_t* text)
{
	Text = text;
	if (u32(CursorPos) > Text.size())
		CursorPos = Text.size();
	HScrollPos = 0;
	breakText();
}


//! Enables or disables automatic scrolling with cursor position
//! \param enable: If set to true, the text will move around with the cursor position
void CGUIEditBoxIRB::setAutoScroll(bool enable)
{
	AutoScroll = enable;
}


//! Checks to see if automatic scrolling is enabled
//! \return true if automatic scrolling is enabled, false if not
bool CGUIEditBoxIRB::isAutoScrollEnabled() const
{
	_IRR_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX;
	return AutoScroll;
}


//! Gets the area of the text in the edit box
//! \return Returns the size in pixels of the text
core::dimension2du CGUIEditBoxIRB::getTextDimension()
{
	core::rect<s32> ret;

	setTextRect(0);
	ret = CurrentTextRect;

	for (u32 i=1; i < BrokenText.size(); ++i)
	{
		setTextRect(i);
		ret.addInternalPoint(CurrentTextRect.UpperLeftCorner);
		ret.addInternalPoint(CurrentTextRect.LowerRightCorner);
	}

	return core::dimension2du(ret.getSize());
}


//! Sets the maximum amount of characters which may be entered in the box.
//! \param max: Maximum amount of characters. If 0, the character amount is
//! infinity.
void CGUIEditBoxIRB::setMax(u32 max)
{
	Max = max;

	if (Text.size() > Max && Max != 0)
		Text = Text.subString(0, Max);
}


//! Returns maximum amount of characters, previously set by setMax();
u32 CGUIEditBoxIRB::getMax() const
{
	return Max;
}


bool CGUIEditBoxIRB::processMouse(const SEvent& event)
{
	s32 value = 0;
	switch(event.MouseInput.Event)
	{
	case irr::EMIE_MOUSE_WHEEL:
		value = (s32)-event.MouseInput.Wheel;

		if (MultiLine || (WordWrap && BrokenText.size() > 1) )
		{

			int realVal = value;
			if (Scrollbar->getPos()+value < Scrollbar->getMin())
				realVal = (Scrollbar->getMin()-Scrollbar->getPos());
			if (Scrollbar->getPos()+value > Scrollbar->getMax())
				realVal = (Scrollbar->getMax()-Scrollbar->getPos());
			Scrollbar->setPos(Scrollbar->getPos() + realVal);
			IGUISkin* skin = Environment->getSkin();
			IGUIFont* font = OverrideFont ? OverrideFont : skin->getFont();
			int height = (s32)font->getDimension(L"O").Height;
			VScrollPos=VScrollPos+height*(realVal);
			/*s32 lineNo = getLineFromPos(CursorPos);
			if (lineNo > 0)
			{
				s32 cp = CursorPos - BrokenTextPositions[lineNo];

				if ((lineNo+value)>=lineCount-1)
					value=0;

				if ((lineNo+value)<0)
					value=0;

				if (value!=0)
				{
					if ((s32)BrokenText[lineNo-1].size() < cp)
						CursorPos = BrokenTextPositions[lineNo+value] + (s32)BrokenText[lineNo+value].size()+value;
					else
						CursorPos = BrokenTextPositions[lineNo+value] + cp;
				}
			}
			if (value!=0)
				// Update the scrollbar
				//Scrollbar->setPos(getLineFromPos(CursorPos));
			calculateScrollPos();*/
			return true;

		}


		break;
	case irr::EMIE_LMOUSE_LEFT_UP:
		if (Environment->hasFocus(this))
		{
			if (!InMenu) {
				if (!DoubleClicked) {
					CursorPos = getCursorPos(event.MouseInput.X, event.MouseInput.Y);
					if (MouseMarking)
					{
						setTextMarkers( MarkBegin, CursorPos );
					}

					//Scrollbar->setPos(getLineFromPos(CursorPos));
					calculateScrollPos();

				}
				MouseMarking = false;
				DoubleClicked = false;
			}
			return true;
		}
		break;
	case irr::EMIE_MOUSE_MOVED:
		{
			if (MouseMarking)
			{
				CursorPos = getCursorPos(event.MouseInput.X, event.MouseInput.Y);
				setTextMarkers( MarkBegin, CursorPos );
				//TODO: calculateScrollPos seem to have bugs
				if (MarkBegin!=CursorPos)
					calculateScrollPos();
				/*IGUISkin* skin = Environment->getSkin();
				IGUIFont* font = OverrideFont ? OverrideFont : skin->getFont();
				int height = (s32)font->getDimension(L"O").Height;
				s32 lineCount = isMultiLineEnabled() ? BrokenText.size() : 1;
				float linesOnScreen = FrameRect.getHeight()/(float)height;
				Scrollbar->setPos(std::max(0,getLineFromPos(CursorPos-linesOnScreen)));*/
				return true;
			}
		}
		break;
	case irr::EMIE_RMOUSE_LEFT_UP:
		{
			if (!InMenu) {
				if (Environment->hasFocus(this))
				{
					if (!DoubleClicked) {
						CursorPos = getCursorPos(event.MouseInput.X, event.MouseInput.Y);
						if (MouseMarking)
						{
							setTextMarkers( MarkBegin, CursorPos );
						}

						//Scrollbar->setPos(getLineFromPos(CursorPos));
						calculateScrollPos();

					}
					MouseMarking = false;
					DoubleClicked = false;
				}
			}
			// Let's create a context menu if right mouse was clicked
			irr::core::recti rect(event.MouseInput.X,event.MouseInput.Y,event.MouseInput.X+1,event.MouseInput.Y);
			rect.UpperLeftCorner -= AbsoluteRect.UpperLeftCorner;
			rect.LowerRightCorner -= AbsoluteRect.UpperLeftCorner;
			menu = Environment->addContextMenu(rect,this);
			OldMarkBegin = MarkBegin;
			OldMarkEnd = MarkEnd;

			menu->addItem(menustring[CM_CUT].c_str());
			menu->addItem(menustring[CM_COPY].c_str());
			menu->addItem(menustring[CM_PASTE].c_str());
			menu->addItem(menustring[CM_DELETE].c_str());
			menu->addSeparator();
			menu->addItem(menustring[CM_UNDO].c_str());
			menu->addItem(menustring[CM_REDO].c_str());

			InMenu = true;
			return true;
			break;
		}
	case irr::EMIE_LMOUSE_DOUBLE_CLICK:
		{
			// Select the entire word
			// Words are only alpha numerical characters, so something like != would not be selected in its entirety (Maybe add an option for the user to say what limits the word?)
			CursorPos = getCursorPos(event.MouseInput.X, event.MouseInput.Y);

			// Now let's process the word
			// First, find the beginning
			int start, end;
			start = end = 0;

			for (int i = CursorPos; i > 0; i--) {
				char character;
				//wcstombs
				wcstombs(&character, &Text.c_str()[i],1);
				if (!isalnum(character)) {

					start = i+1;
					break;
				}
			}
			for (u32 i = CursorPos; i < Text.size(); i++) {
				char character;
				wcstombs(&character,&Text.c_str()[i],1);
				if (!isalnum(character)) {

					end = i;
					break;
				}
				if (i == Text.size()-1)
					end = i+1;
			}
			setTextMarkers(start,end);
			DoubleClicked = true;
			return true;
			break;
		}
	case irr::EMIE_LMOUSE_TRIPLE_CLICK:
		{
			// Select the entire line
			// Go from \n to \n
			CursorPos = getCursorPos(event.MouseInput.X, event.MouseInput.Y);

			// Now let's process the word
			// First, find the beginning
			int start, end;
			start = end = 0;

			for (int i = CursorPos; i > 0; i--) {
				if (Text.c_str()[i] == L'\n' || Text.c_str()[i] == L'\r') {
					start = i+1;
					break;
				}
			}
			for (u32 i = CursorPos; i < Text.size(); i++) {
				if (Text.c_str()[i] == L'\n' || Text.c_str()[i] == L'\r') {
					end = i;
					break;
				}
				if (i == Text.size()-1)
					end = i+1;
			}
			setTextMarkers(start,end);
			DoubleClicked = true;
			return true;
			break;
		}
	case EMIE_LMOUSE_PRESSED_DOWN:
		if (!InMenu) {
			if (!Environment->hasFocus(this))
			{
				BlinkStartTime = IRRdevice->getTimer()->getRealTime();
				MouseMarking = true;
				CursorPos = getCursorPos(event.MouseInput.X, event.MouseInput.Y);
				setTextMarkers(CursorPos, CursorPos );
				calculateScrollPos();
				return true;
			}
			else
			{
				if (!AbsoluteClippingRect.isPointInside(
					core::position2d<s32>(event.MouseInput.X, event.MouseInput.Y)))
				{
					return false;
				}
				else
				{
					// move cursor
					CursorPos = getCursorPos(event.MouseInput.X, event.MouseInput.Y);

					s32 newMarkBegin = MarkBegin;
					if (!MouseMarking)
						newMarkBegin = CursorPos;

					MouseMarking = true;
					setTextMarkers( newMarkBegin, CursorPos);
					calculateScrollPos();

				}
			}
		}

		return true;
	case EMIE_RMOUSE_PRESSED_DOWN:
		if (!InMenu && !isPointInSelection(core::position2di(event.MouseInput.X,event.MouseInput.Y))) {
			if (!Environment->hasFocus(this))
			{
				BlinkStartTime = IRRdevice->getTimer()->getRealTime();
				MouseMarking = true;
				CursorPos = getCursorPos(event.MouseInput.X, event.MouseInput.Y);
				setTextMarkers(CursorPos, CursorPos );
				calculateScrollPos();
				return true;
			}
			else
			{
				if (!AbsoluteClippingRect.isPointInside(
					core::position2d<s32>(event.MouseInput.X, event.MouseInput.Y)))
				{
					return false;
				}
				else
				{
					// move cursor
					CursorPos = getCursorPos(event.MouseInput.X, event.MouseInput.Y);

					s32 newMarkBegin = MarkBegin;
					if (!MouseMarking)
						newMarkBegin = CursorPos;

					MouseMarking = true;
					setTextMarkers( newMarkBegin, CursorPos);
					calculateScrollPos();

				}
			}
		}
		return true;
	default:
		break;
	}

	return false;
}


s32 CGUIEditBoxIRB::getCursorPos(s32 x, s32 y)
{
	IGUIFont* font = OverrideFont;
	IGUISkin* skin = Environment->getSkin();
	if (!OverrideFont)
		font = skin->getFont();

	const u32 lineCount = (WordWrap || MultiLine) ? BrokenText.size() : 1;

	core::stringw *txtLine=0;
	s32 startPos=0;
	x+=3;

	for (u32 i=0; i < lineCount; ++i)
	{
		setTextRect(i);
		if (i == 0 && y < CurrentTextRect.UpperLeftCorner.Y)
			y = CurrentTextRect.UpperLeftCorner.Y;
		if (i == lineCount - 1 && y > CurrentTextRect.LowerRightCorner.Y )
			y = CurrentTextRect.LowerRightCorner.Y;

		// is it inside this region?
		if (y >= CurrentTextRect.UpperLeftCorner.Y && y <= CurrentTextRect.LowerRightCorner.Y)
		{
			// we've found the clicked line
			txtLine = (WordWrap || MultiLine) ? &BrokenText[i] : &Text;
			startPos = (WordWrap || MultiLine) ? BrokenTextPositions[i] : 0;
			break;
		}
	}

	if (x < CurrentTextRect.UpperLeftCorner.X)
		x = CurrentTextRect.UpperLeftCorner.X;

	//s32 idx = font->getCharacterFromPos(Text.c_str(), x - CurrentTextRect.UpperLeftCorner.X);
	s32 idx = txtLine ? font->getCharacterFromPos(txtLine->c_str(), x - CurrentTextRect.UpperLeftCorner.X) : -1;

	// click was on or left of the line
	if (idx != -1)
		return idx + startPos;

	// click was off the right edge of the line, go to end.
	if (txtLine) {
		return std::max((int)0,(int)txtLine->size() + startPos-1);
	}
	else {
		return startPos;
	}
}


//! Breaks the single text line.
void CGUIEditBoxIRB::breakText()
{
	linenumber="";
	IGUISkin* skin = Environment->getSkin();

	if ((!WordWrap && !MultiLine) || !skin)
		return;

	BrokenText.clear(); // need to reallocate :/
	BrokenTextPositions.set_used(0);

	IGUIFont* font = OverrideFont;
	if (!OverrideFont)
		font = skin->getFont();

	if (!font)
		return;

	LastBreakFont = font;

	core::stringw line;
	core::stringw word;
	core::stringw whitespace;

	s32 lastLineStart = 0;
	s32 size = Text.size();
	s32 length = 0;
	//s32 elWidth = RelativeRect.getWidth() - 6;
	s32 elWidth = 0;
	// Redefine the line break depending on the display
	if (LineNumbering)
		elWidth = RelativeRect.getWidth() - 90;
	else
		elWidth = RelativeRect.getWidth() - 30;

	wchar_t c;

	for (s32 i=0; i<size; ++i)
	{
		c = Text[i];
		bool lineBreak = false;

		if (c == L'\r') // Mac or Windows breaks
		{
			lineBreak = true;
			c = ' ';
			if (Text[i+1] == L'\n') // Windows breaks
			{
				Text.erase(i+1);
				--size;
			}
		}
		else if (c == L'\n') // Unix breaks
		{
			lineBreak = true;
			c = ' ';
		}

		// don't break if we're not a multi-line edit box
		if (!MultiLine)
			lineBreak = false;

		if (c == L' ' || c == L'(' || c == L')' || c == 0 || i == (size-1))
		{
			if (word.size())
			{
				// here comes the next whitespace, look if
				// we can break the last word to the next line.
				s32 whitelgth = font->getDimension(whitespace.c_str()).Width;
				s32 worldlgth = font->getDimension(word.c_str()).Width;

				if (WordWrap && length + worldlgth + whitelgth > elWidth)
				{
					// break to next line
					length = worldlgth;
					BrokenText.push_back(line);
					BrokenTextPositions.push_back(lastLineStart);
					lastLineStart = i - (s32)word.size();
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

				BrokenText.push_back(line);
				linenumber += (core::stringw)BrokenText.size() + L"\n";
				BrokenTextPositions.push_back(lastLineStart);
				lastLineStart = i+1;
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

	line += whitespace;
	line += word;
	BrokenText.push_back(line);
	BrokenTextPositions.push_back(lastLineStart);

	s32 textframe = FrameRect.getHeight();
	u32 linesize = skin->getFont()->getDimension(L"O").Height;
	int maxLines = BrokenText.size();
	int linecount=int(textframe/linesize);

	//Calculation is wrong, Scrollbar max is not set at the moment, this could cause other problems.
	//This bug started to appear from revision 260. I wonder, how I haven't seen it sooner.(rev 315 now)

	// linecount calculation or how it's applied is wrong.
	// Keeping only the maxline now so the user can edit his full script.
	//Scrollbar->setMax(maxLines-linecount);
	Scrollbar->setMax(maxLines);

	linenumber += (core::stringw)BrokenText.size() + L"\n";
	//Linecounter->setText(linenumber.c_str());
	//Linecounter->move(core::vector2di(0,10));

}


void CGUIEditBoxIRB::setTextRect(s32 line)
{
	core::dimension2du d;

	IGUISkin* skin = Environment->getSkin();
	if (!skin)
		return;

	IGUIFont* font = OverrideFont ? OverrideFont : skin->getFont();

	if (!font)
		return;

	// get text dimension
	const u32 lineCount = (WordWrap || MultiLine) ? BrokenText.size() : 1;
	if (WordWrap || MultiLine)
	{
		// Must not return 0. Quick fix (should investigate further why I have some 0 value return sometimes)
		// For some reasons the line return the index and it's not allowed.
		//core::stringw textblock = BrokenText[line].c_str();
		//if (textblock.size()>0)
		//	d = font->getDimension(textblock.c_str());
		//else
			d = font->getDimension(L"O");
	}
	else
	{
		d = font->getDimension(Text.c_str());
		d.Height = AbsoluteRect.getHeight();
	}
	d.Height += font->getKerningHeight();

	// Failsafes in case of wrong calculation
	if (d.Height<8)
		d.Height=8;

	if (d.Width>8)
		d.Width=8;

	if (d.Height>96)
		d.Height=96;

	if (d.Width>2048)
		d.Width=2048;

	// justification
	switch (HAlign)
	{
	case EGUIA_CENTER:
		// align to h centre
		CurrentTextRect.UpperLeftCorner.X = (FrameRect.getWidth()/2) - (d.Width/2);
		CurrentTextRect.LowerRightCorner.X = (FrameRect.getWidth()/2) + (d.Width/2);
		break;
	case EGUIA_LOWERRIGHT:
		// align to right edge
		CurrentTextRect.UpperLeftCorner.X = FrameRect.getWidth() - d.Width;
		CurrentTextRect.LowerRightCorner.X = FrameRect.getWidth();
		break;
	default:
		// align to left edge
		CurrentTextRect.UpperLeftCorner.X = 0;
		CurrentTextRect.LowerRightCorner.X = d.Width;

	}

	switch (VAlign)
	{
	case EGUIA_CENTER:
		// align to v centre
		CurrentTextRect.UpperLeftCorner.Y =
			(FrameRect.getHeight()/2) - (lineCount*d.Height)/2 + d.Height*line;
		break;
	case EGUIA_LOWERRIGHT:
		// align to bottom edge
		CurrentTextRect.UpperLeftCorner.Y =
			FrameRect.getHeight() - lineCount*d.Height + d.Height*line;
		break;
	default:
		// align to top edge
		CurrentTextRect.UpperLeftCorner.Y = d.Height*line;
		break;
	}

	CurrentTextRect.UpperLeftCorner.X  -= HScrollPos;
	CurrentTextRect.LowerRightCorner.X -= HScrollPos;
	CurrentTextRect.UpperLeftCorner.Y  -= VScrollPos;
	CurrentTextRect.LowerRightCorner.Y = CurrentTextRect.UpperLeftCorner.Y + d.Height;

	CurrentTextRect += FrameRect.UpperLeftCorner;
	// Space
	CurrentTextRect.UpperLeftCorner.X+=LeftSpace;
	CurrentTextRect.UpperLeftCorner.Y+=UpperSpace;
	CurrentTextRect.LowerRightCorner.X-=RightSpace;
	CurrentTextRect.LowerRightCorner.Y-=LowerSpace;

}


s32 CGUIEditBoxIRB::getLineFromPos(s32 pos)
{
	if (!WordWrap && !MultiLine)
		return 0;

	s32 i=0;
	while (i < (s32)BrokenTextPositions.size())
	{
		if (BrokenTextPositions[i] > pos)
			return i-1;
		++i;
	}

	return (s32)BrokenTextPositions.size() - 1;
}


void CGUIEditBoxIRB::inputChar(wchar_t c)
{
	if (!IsEnabled)
		return;

	if (c != 0)
	{
		if (Text.size() < Max || Max == 0)
		{
			core::stringw input = L"";
			input += c;
			if (MarkBegin != MarkEnd)
			{
				// replace marked text
				const s32 realmbgn = MarkBegin < MarkEnd ? MarkBegin : MarkEnd;
				const s32 realmend = MarkBegin < MarkEnd ? MarkEnd : MarkBegin;

				replaceText(realmbgn,realmend,input);
				CursorPos = realmbgn+1;
			}
			else
			{
				// add new character
				if (Inserting && CursorPos+1 <= (s32)Text.size()-1)
					replaceText(CursorPos,CursorPos+1,input);
				else
					replaceText(CursorPos,CursorPos,input);
				++CursorPos;
			}

			BlinkStartTime = IRRdevice->getTimer()->getRealTime();
			setTextMarkers(0, 0);
		}
	}
	breakText();
	sendGuiEvent(EGET_EDITBOX_CHANGED);
	calculateScrollPos();
}


void CGUIEditBoxIRB::calculateScrollPos()
{
	if (!AutoScroll)
		return;

	// calculate horizontal scroll position
	s32 cursLine = getLineFromPos(CursorPos);
	setTextRect(cursLine);

	// don't do horizontal scrolling when wordwrap is enabled.
	if (!WordWrap)
	{
		// get cursor position
		IGUISkin* skin = Environment->getSkin();
		if (!skin)
			return;
		IGUIFont* font = OverrideFont ? OverrideFont : skin->getFont();
		if (!font)
			return;

		core::stringw *txtLine = MultiLine ? &BrokenText[cursLine] : &Text;
		s32 cPos = MultiLine ? CursorPos - BrokenTextPositions[cursLine] : CursorPos;

		s32 cStart = CurrentTextRect.UpperLeftCorner.X + HScrollPos +
			font->getDimension(txtLine->subString(0, cPos).c_str()).Width;

		s32 cEnd = cStart + font->getDimension(L"_ ").Width;

		if (FrameRect.LowerRightCorner.X < cEnd)
			HScrollPos = cEnd - FrameRect.LowerRightCorner.X;
		else if (FrameRect.UpperLeftCorner.X > cStart)
			HScrollPos = cStart - FrameRect.UpperLeftCorner.X;
		else
			HScrollPos = 0;

		// todo: adjust scrollbar
		//ScrollbarH->setVisible(true);
	}

	// New method of calculating the scrolling
	IGUISkin* skin = Environment->getSkin();
	IGUIFont* font = OverrideFont ? OverrideFont : skin->getFont();
	int height = (s32)font->getDimension(L"O").Height;
	int linepos=(int)getLineFromPos(CursorPos);
	int lines=FrameRect.getHeight()/height;
	s32 min=VScrollPos/height;
	s32 max=(VScrollPos+((lines-2)*height))/height;

	s32 lineCount = isMultiLineEnabled() ? BrokenText.size() : 1;
	float linesOnScreen = FrameRect.getHeight()/(float)height;
	lineCount -= (s32)floor(linesOnScreen);
	if (lineCount > 0)
		Scrollbar->setMax(lineCount+1);

	if (linepos<=min) {
		Scrollbar->setPos(Scrollbar->getPos()-(min-linepos));
		VScrollPos=VScrollPos-height*(min-linepos);
	}
	if (linepos>max) {
		Scrollbar->setPos(Scrollbar->getPos()+linepos-max);
		VScrollPos=VScrollPos+height*(linepos-max);
	}

	//printf("Here is the current pos %i, here is the current vscroll pos: %i, total lines: %i \n",linepos,VScrollPos,lines);
	// vertical scroll position
	/*if (FrameRect.LowerRightCorner.Y < CurrentTextRect.LowerRightCorner.Y + VScrollPos)
	VScrollPos = CurrentTextRect.LowerRightCorner.Y - FrameRect.LowerRightCorner.Y + VScrollPos;

	else if (FrameRect.UpperLeftCorner.Y > CurrentTextRect.UpperLeftCorner.Y + VScrollPos)
	VScrollPos = CurrentTextRect.UpperLeftCorner.Y - FrameRect.UpperLeftCorner.Y + VScrollPos;
	else
	VScrollPos = 0;*/



	// todo: adjust scrollbar
	//Scrollbar->setMax(BrokenText.size());
}

//! set text markers
void CGUIEditBoxIRB::setTextMarkers(s32 begin, s32 end)
{
	if ( begin != MarkBegin || end != MarkEnd )
	{
		MarkBegin = begin;
		MarkEnd = end;
		sendGuiEvent(EGET_EDITBOX_MARKING_CHANGED);
	}
}

//! add a keyword to highlight
void CGUIEditBoxIRB::addKeyword(irr::core::stringc keyword, irr::video::SColor color, bool matchCase) {
	addKeywordInfo(keyword.size(),0);
	SKeyword key;
	key.text = keyword;
	key.color = color;
	key.matchCase = matchCase;
	Keywords.push_back(key);
	for (u32 i = 0; i < Keywords.size(); i++) {
		if (key.text.size() >= Keywords[i].text.size()) {
			Keywords.insert(key,i);
			return;
		}
	}
	Keywords.push_back(key);
}

//! add a keyword to highlight lines
void CGUIEditBoxIRB::addLineKeyword(irr::core::stringc keyword, irr::video::SColor color, bool matchCase) {
	addKeywordInfo(keyword.size(),2);
	SKeyword key;
	key.text = keyword;
	key.color = color;
	key.matchCase = matchCase;
	for (u32 i = 0; i < LineKeywords.size(); i++) {
		if (key.text.size() >= LineKeywords[i].text.size()) {
			LineKeywords.insert(key,i);
			return;
		}
	}
	LineKeywords.push_back(key);

}

//! add a keyword to highlight groups
void CGUIEditBoxIRB::addGroupKeyword(irr::core::stringc keyword, irr::core::stringc endKeyword, irr::video::SColor color, bool matchCase, bool multiLine) {
	addKeywordInfo(keyword.size(),1);
	SGroupKeyword key;
	key.text = keyword;
	key.color = color;
	key.matchCase = matchCase;
	key.endText = endKeyword;
	key.multiLine = multiLine;
	for (u32 i = 0; i < GroupKeywords.size(); i++) {
		if (key.text.size() >= GroupKeywords[i].text.size()) {
			GroupKeywords.insert(key,i);
			return;
		}
	}
	GroupKeywords.push_back(key);
}

//! send some gui event to parent
void CGUIEditBoxIRB::sendGuiEvent(EGUI_EVENT_TYPE type)
{
	if ( Parent )
	{
		SEvent e;
		e.EventType = EET_GUI_EVENT;
		e.GUIEvent.Caller = this;
		e.GUIEvent.Element = 0;
		e.GUIEvent.EventType = type;

		Parent->OnEvent(e);
	}
}

//! Writes attributes of the element.
void CGUIEditBoxIRB::serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options=0) const
{
	// IGUIEditBox::serializeAttributes(out,options);

	out->addBool  ("OverrideColorEnabled",OverrideColorEnabled );
	out->addColor ("OverrideColor",       OverrideColor);
	// out->addFont("OverrideFont",OverrideFont);
	out->addInt   ("MaxChars",            Max);
	out->addBool  ("WordWrap",            WordWrap);
	out->addBool  ("MultiLine",           MultiLine);
	out->addBool  ("AutoScroll",          AutoScroll);
	out->addBool  ("PasswordBox",         PasswordBox);
	core::stringw ch = L" ";
	ch[0] = PasswordChar;
	out->addString("PasswordChar",        ch.c_str());
	out->addEnum  ("HTextAlign",          HAlign, GUIAlignmentNames);
	out->addEnum  ("VTextAlign",          VAlign, GUIAlignmentNames);

	//IGUIEditBoxIRB::serializeAttributes(out,options);
}


//! Reads attributes of the element
void CGUIEditBoxIRB::deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options=0)
{
	//IGUIEditBox::deserializeAttributes(in,options);

	setOverrideColor(in->getAttributeAsColor("OverrideColor"));
	enableOverrideColor(in->getAttributeAsBool("OverrideColorEnabled"));
	setMax(in->getAttributeAsInt("MaxChars"));
	setWordWrap(in->getAttributeAsBool("WordWrap"));
	setMultiLine(in->getAttributeAsBool("MultiLine"));
	setAutoScroll(in->getAttributeAsBool("AutoScroll"));
	core::stringw ch = in->getAttributeAsStringW("PasswordChar");

	if (!ch.size())
		setPasswordBox(in->getAttributeAsBool("PasswordBox"));
	else
		setPasswordBox(in->getAttributeAsBool("PasswordBox"), ch[0]);

	setTextAlignment( (EGUI_ALIGNMENT) in->getAttributeAsEnumeration("HTextAlign", GUIAlignmentNames),
		(EGUI_ALIGNMENT) in->getAttributeAsEnumeration("VTextAlign", GUIAlignmentNames));

	// setOverrideFont(in->getAttributeAsFont("OverrideFont"));
}

irr::core::recti CGUIEditBoxIRB::myRect(int x, int y, int w, int h)
{
	return irr::core::recti(x,y,x+w,y+h);
}

//! returns whether a point is within the selection
bool CGUIEditBoxIRB::isPointInSelection(core::position2di pos)
{
	return (getCursorPos(pos.X,pos.Y) > std::min(MarkBegin,MarkEnd) && getCursorPos(pos.X,pos.Y) < std::max(MarkBegin,MarkEnd));
}

//! clears all keywords
void CGUIEditBoxIRB::clearKeywords() {
	Keywords.clear();
	LineKeywords.clear();
	GroupKeywords.clear();
	KeywordList.clear();
}

//! adds C++ style keywords
void CGUIEditBoxIRB::addCppKeywords(irr::video::SColor keywordColor, irr::video::SColor stringColor, irr::video::SColor commentColor) {
	addKeyword("if",keywordColor,true);
	addKeyword("bool",keywordColor,true);
	addKeyword("char",keywordColor,true);
	addKeyword("int",keywordColor,true);
	addKeyword("float",keywordColor,true);
	addKeyword("double",keywordColor,true);
	addKeyword("signed",keywordColor,true);
	addKeyword("unsigned",keywordColor,true);
	addKeyword("long",keywordColor,true);
	addKeyword("short",keywordColor,true);
	addKeyword("while",keywordColor,true);
	addKeyword("for",keywordColor,true);
	addKeyword("do",keywordColor,true);
	addKeyword("return",keywordColor,true);
	addKeyword("break",keywordColor,true);
	addKeyword("void",keywordColor,true);
	addKeyword("class",keywordColor,true);
	addKeyword("enum",keywordColor,true);
	addKeyword("struct",keywordColor,true);
	addKeyword("true",keywordColor,true);
	addKeyword("false",keywordColor,true);
	addKeyword("new",keywordColor,true);
	addKeyword("delete",keywordColor,true);
	addKeyword("else",keywordColor,true);
	addKeyword("this",keywordColor,true);
	addKeyword("break",keywordColor,true);
	addKeyword("case",keywordColor,true);
	addKeyword("catch",keywordColor,true);
	addKeyword("switch",keywordColor,true);
	addKeyword("const",keywordColor,true);
	addKeyword("const_cast",keywordColor,true);
	addKeyword("continue",keywordColor,true);
	addKeyword("asm",keywordColor,true);
	addKeyword("dynamic_cast",keywordColor,true);
	addKeyword("export",keywordColor,true);
	addKeyword("friend",keywordColor,true);
	addKeyword("goto",keywordColor,true);
	addKeyword("inline",keywordColor,true);
	addKeyword("mutable",keywordColor,true);
	addKeyword("namespace",keywordColor,true);
	addKeyword("private",keywordColor,true);
	addKeyword("protected",keywordColor,true);
	addKeyword("public",keywordColor,true);
	addKeyword("register",keywordColor,true);
	addKeyword("reinterpret_cast",keywordColor,true);
	addKeyword("sizeof",keywordColor,true);
	addKeyword("static",keywordColor,true);
	addKeyword("static_cast",keywordColor,true);
	addKeyword("template",keywordColor,true);
	addKeyword("throw",keywordColor,true);
	addKeyword("try",keywordColor,true);
	addKeyword("typedef",keywordColor,true);
	addKeyword("typeid",keywordColor,true);
	addKeyword("typename",keywordColor,true);
	addKeyword("union",keywordColor,true);
	addKeyword("using",keywordColor,true);
	addKeyword("volatile",keywordColor,true);
	addKeyword("wchar_t",keywordColor,true);
	addLineKeyword("//",commentColor,true);
	addGroupKeyword("#"," ",keywordColor,true);
	addGroupKeyword("\"","\"",stringColor,true);
	addGroupKeyword("\'","\'",stringColor,true);
	addGroupKeyword("/*","*/",commentColor,true,true);
}
//! adds LUA style keywords
void CGUIEditBoxIRB::addLUAKeywords(irr::video::SColor keywordColor, irr::video::SColor stringColor, irr::video::SColor commentColor) {
	addKeyword("and",keywordColor,true);
	addKeyword("break",keywordColor,true);
	addKeyword("do",keywordColor,true);
	addKeyword("else",keywordColor,true);
	addKeyword("elseif",keywordColor,true);
	addKeyword("end",keywordColor,true);
	addKeyword("for",keywordColor,true);
	addKeyword("function",keywordColor,true);
	addKeyword("if",keywordColor,true);
	addKeyword("local",keywordColor,true);
	addKeyword("nil",keywordColor,true);
	addKeyword("not",keywordColor,true);
	addKeyword("or",keywordColor,true);
	addKeyword("repeat",keywordColor,true);
	addKeyword("return",keywordColor,true);
	addKeyword("then",keywordColor,true);
	addKeyword("until",keywordColor,true);
	addKeyword("while",keywordColor,true);
	addKeyword("print",keywordColor,true);
	addGroupKeyword("\"","\"",stringColor,true);
	addGroupKeyword("\'","\'",stringColor,true);
	addGroupKeyword("[[","]]",stringColor,true);
	addGroupKeyword("--[[","]]",commentColor,true,true);
	addLineKeyword("--",commentColor,true);
}

//! adds keyword info
void CGUIEditBoxIRB::addKeywordInfo(int size, int type)
{
	for (u32 i = 0; i < KeywordList.size(); i++) {
		if (size >= KeywordList[i].size) {
			SKeywordInfo inf;
			inf.size = size;
			inf.type = type;
			KeywordList.insert(inf,i);
			return;
		}
	}
	SKeywordInfo inf;
	inf.size = size;
	inf.type = type;
	KeywordList.push_back(inf);
}
//! Replace text for easy undo/redo and line updates
void CGUIEditBoxIRB::replaceText(int start, int end, irr::core::stringw newText) {

	STextAction action;
	action.newText = newText;
	action.oldText = Text.subString(start,(end-start));
	action.oldStart = std::min(start,end);
	action.oldEnd = std::max(end,start);
	UndoList.push_back(action);

	core::stringw s = Text.subString(0, start);
	s.append(newText.c_str());
	s.append( Text.subString(end, Text.size()) );
	Text = s;
}

void CGUIEditBoxIRB::undo() {
	if (UndoList.size() > 0) {
		STextAction action = UndoList[UndoList.size()-1];
		CursorPos = action.oldStart;
		Text = action.undo(Text);
		UndoList.erase(UndoList.size()-1);
		RedoList.push_back(action);
		breakText();
	}
}
void CGUIEditBoxIRB::redo() {
	if (RedoList.size() > 0) {
		STextAction action = RedoList[RedoList.size()-1];
		CursorPos = action.oldStart+action.newText.size();
		Text = action.redo(Text);
		RedoList.erase(RedoList.size()-1);
		UndoList.push_back(action);
		breakText();
	}
}

//! copies text between markers
void CGUIEditBoxIRB::copy() {
	// copy to clipboard
	if (!PasswordBox && Operator && MarkBegin != MarkEnd)
	{
		const s32 realmbgn = MarkBegin < MarkEnd ? MarkBegin : MarkEnd;
		const s32 realmend = MarkBegin < MarkEnd ? MarkEnd : MarkBegin;

		core::stringc s;
		s = Text.subString(realmbgn, realmend - realmbgn).c_str();
		Operator->copyToClipboard(s.c_str());
	}
}
//! cuts text between markers
void CGUIEditBoxIRB::cut() {
	if (!PasswordBox && Operator && MarkBegin != MarkEnd)
	{
		const s32 realmbgn = MarkBegin < MarkEnd ? MarkBegin : MarkEnd;
		const s32 realmend = MarkBegin < MarkEnd ? MarkEnd : MarkBegin;

		// copy
		core::stringc sc;
		sc = Text.subString(realmbgn, realmend - realmbgn).c_str();
		Operator->copyToClipboard(sc.c_str());

		CursorPos = realmbgn;


		if (IsEnabled)
		{
			// delete
			replaceText(realmbgn,realmend,"");

		}

		if (CursorPos > (s32)Text.size())
			CursorPos = (s32)Text.size();
		setTextMarkers(CursorPos,CursorPos);

		breakText();
		sendGuiEvent(EGET_EDITBOX_CHANGED);
	}

}
//! pastes text between markers
void CGUIEditBoxIRB::paste() {
	if ( !IsEnabled )
		return;

	// paste from the clipboard
	if (Operator)
	{
		const s32 realmbgn = MarkBegin < MarkEnd ? MarkBegin : MarkEnd;
		const s32 realmend = MarkBegin < MarkEnd ? MarkEnd : MarkBegin;

		// add new character
		const c8* p = Operator->getTextFromClipboard();
		if (p)
		{
			if (MarkBegin == MarkEnd)
			{
				replaceText(CursorPos,CursorPos,p);
				core::stringw s = p;
				CursorPos += s.size();
			}
			else
			{
				// replace text
				replaceText(realmbgn,realmend,p);
				core::stringw s = p;
				CursorPos = realmbgn + s.size();

			}
		}

		if (CursorPos > (s32)Text.size())
			CursorPos = (s32)Text.size();
		setTextMarkers(CursorPos,CursorPos);

		breakText();
		sendGuiEvent(EGET_EDITBOX_CHANGED);
	}

}
//! deletes text between markers
void CGUIEditBoxIRB::deleteText() {
	if ( !this->IsEnabled )
		return;

	if (Text.size() != 0)
	{
		core::stringw s;

		if (MarkBegin != MarkEnd)
		{
			// delete marked text
			const s32 realmbgn = MarkBegin < MarkEnd ? MarkBegin : MarkEnd;
			const s32 realmend = MarkBegin < MarkEnd ? MarkEnd : MarkBegin;

			replaceText(realmbgn,realmend,L"");

			CursorPos = realmbgn;
		}
		else
		{
			// delete text before cursor
			replaceText(CursorPos,CursorPos+1,L"");
		}

		if (CursorPos > (s32)Text.size())
			CursorPos = (s32)Text.size();

		setTextMarkers(CursorPos,CursorPos);

		BlinkStartTime = IRRdevice->getTimer()->getRealTime();
		breakText();
		sendGuiEvent(EGET_EDITBOX_CHANGED);
	}
}

} // end namespace gui
} // end namespace irr

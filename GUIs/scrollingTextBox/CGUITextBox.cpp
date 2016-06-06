#include "include/CGUITextBox.h"

namespace irr
{
namespace gui
{
CGUITextBox::CGUITextBox(irr::gui::IGUIFont * font, const wchar_t * text,
						 IGUIEnvironment* environment, core::rect<s32> rectangle,
						 IGUIElement* parent, s32 id)
		: IGUIElement(EGUIET_ELEMENT, environment, parent, id, rectangle)
{
	m_bScrollModeLines = true;
	m_bScrollbarRight = true;

	Text = text;

	m_pFont = font;

	m_pScrollbar = 0;
	m_pDummyText = 0;
	m_pStaticText = 0;

	reset();
}

CGUITextBox::~CGUITextBox()
{
}

//! Called if an event happened.
bool CGUITextBox::OnEvent(const SEvent& event)
{
	if (Parent)
		Parent->OnEvent(event);

	if(event.EventType == EET_MOUSE_INPUT_EVENT && event.MouseInput.Event == EMIE_MOUSE_WHEEL)
	{
		m_pScrollbar->setPos(m_pScrollbar->getPos() + (s32)event.MouseInput.Wheel*(-10));
		irr::s32 line = m_pScrollbar->getPos();
		setVisibleText(line);
	}

	if(event.EventType == EET_GUI_EVENT && event.GUIEvent.EventType == EGET_SCROLL_BAR_CHANGED)
	{
		IGUIScrollBar * bar = static_cast <irr::gui::IGUIScrollBar *>
			(event.GUIEvent.Caller);
		if(bar == m_pScrollbar)
		{
			irr::s32 line = m_pScrollbar->getPos();
			setVisibleText(line);
		}
	}
switch(event.EventType)
   {
   case EET_MOUSE_INPUT_EVENT:
      {
         switch(event.MouseInput.Event)
         {
         case EMIE_MOUSE_WHEEL:
            m_pScrollbar->setPos(m_pScrollbar->getPos() + (s32)event.MouseInput.Wheel*-10);
            irr::s32 line = m_pScrollbar->getPos();
            setVisibleText(line);
            return true;
         }
      }
      break;
   }
	return true;
}

void CGUITextBox::setScrollModeLines(bool bLines){
	m_bScrollModeLines = bLines;
	calculateScrollbarSize();
}
bool CGUITextBox::isScrollModeLines(void){
	return m_bScrollModeLines;
}

void CGUITextBox::setScrollbarRight(bool bRight)
{
	if(!m_pStaticText || !m_pScrollbar || !m_pDummyText)
		return;

	irr::s32 right, down;
	right = RelativeRect.getWidth();
	down = RelativeRect.getHeight();
	if(bRight)
	{
		m_pScrollbar->setRelativePosition(core::rect<s32>
			(right - 10, 0, right, down));
		m_pDummyText->setRelativePosition(core::rect<s32>
			(0, 0, right - 10, down));
		m_pStaticText->setRelativePosition(core::rect<s32>
			(0, 0, right - 10, down));
	}
	else
	{
		m_pScrollbar->setRelativePosition(core::rect<s32>
			(0, 0, 10, down));
		m_pDummyText->setRelativePosition(core::rect<s32>
			(10, 0, right, down));
		m_pStaticText->setRelativePosition(core::rect<s32>
			(10, 0, right, down));
	}
	m_bScrollbarRight = bRight;
}
bool CGUITextBox::isScrollbarRight(void){
	return m_bScrollbarRight;
}

void CGUITextBox::setText(const wchar_t* text)
{
	Text = text;
	m_pStaticText->setText(text);
	calculateScrollbarSize();
	m_pScrollbar->setPos(0);
	setVisibleText(0);
}

void CGUITextBox::setRelativePosition(const core::rect<s32> r)
{
	RelativeRect = r;
	updateAbsolutePosition();
	reset();
}

/////////////////////////////////
// Private

void CGUITextBox::setVisibleText(irr::s32 startLine)
{
	if(!m_pStaticText)
	{
		return;
	}

	// Calculate Current Position
	irr::s32 fontHeight = m_pFont->getDimension(L"A").Height;
	irr::s32 deltaX = 0;
	deltaX = (m_bScrollModeLines ? startLine * fontHeight : startLine);

	// Set the Position of the text
	irr::core::rect<irr::s32> dim = getRelativePosition();
	s32 left = (m_bScrollbarRight ? 0 : 10);
	s32 right = (m_bScrollbarRight ? -10 : 0);
	dim = irr::core::rect<irr::s32> (left, -deltaX, dim.getWidth()+right,dim.getHeight());
	m_pStaticText->setRelativePosition(dim);
}

irr::s32 CGUITextBox::calculateScrollbarSize(void)
{
	if(!m_pScrollbar)
	{
		return -1;
	}

	irr::core::rect<irr::s32> origRect = m_pStaticText->getRelativePosition();
	m_pStaticText->setRelativePosition(m_pDummyText->getRelativePosition());

	irr::s32 fontHeight = m_pFont->getDimension(L"A").Height;	// Height depending on font
	// Irrlicht serves us with giving the correct Text Height
	irr::s32 textHeight = m_pStaticText->getTextHeight();
	irr::s32 scrollbarSize = 0;
	if(m_bScrollModeLines)
	{
		irr::s32 iLineViewableCount = (origRect.getHeight() / fontHeight);
		scrollbarSize = textHeight / fontHeight - iLineViewableCount;
	}
	else
	{
		scrollbarSize = textHeight - origRect.getHeight();
	}

	// Revert to original Size and set the Max. Value of the Scrollbar
	m_pStaticText->setRelativePosition(origRect);
	m_pScrollbar->setMax(scrollbarSize);
	return scrollbarSize;
}

void CGUITextBox::reset(void)
{
	if(m_pScrollbar)
	{
		m_pScrollbar->remove();
		m_pScrollbar = 0;
	}
	if(m_pStaticText)
	{
		m_pStaticText->remove();
		m_pStaticText = 0;
	}
	if(m_pDummyText)
	{
		m_pDummyText->remove();
		m_pDummyText = 0;
	}

	irr::s32 right, down;
	right = RelativeRect.getWidth();
	down = RelativeRect.getHeight();

	// Default Scrollbar Position is on the right
	m_pScrollbar = Environment->addScrollBar(false,
		irr::core::rect<irr::s32>(right - 10, 0, right, down), this);

	m_pDummyText = Environment->addStaticText(L"",
		irr::core::rect<irr::s32>(0, 0, right - 10, down), true,
		true, this);
	m_pStaticText = Environment->addStaticText(L"",
		irr::core::rect<irr::s32>(0, 0, right - 10, down), false,
		true, this);
	m_pStaticText->setOverrideFont(m_pFont);

	setText(Text.c_str());
	setScrollbarRight(m_bScrollbarRight);
}


}	// end namespace gui
}	// end namespace irr

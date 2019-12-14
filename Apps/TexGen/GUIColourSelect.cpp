#include "StdAfx.h"
#include "GUIColourSelect.h"


CGUIColourSelect::CGUIColourSelect( gui::IGUIEnvironment* Env, gui::IGUIWindow* Parent, core::vector2d<s32> Pos )
: m_Parent( Parent )
, m_Colour( 255,0,0,0 )
, m_Enabled( true )
{
	// Check
	if (!m_Parent)
		return;

	s32 x1 = Pos.X, x2 = 35 + Pos.X, x3 = 40 + Pos.X, x4 = 170 + Pos.X, x5 = 175 + Pos.X, x6 = 200 + Pos.X, x7 = 205 + Pos.X, x8 = 270 + Pos.X;
	s32 y1 = Pos.Y, y2 = 15 + Pos.Y, y3 = 20 + Pos.Y, y4 = 35 + Pos.Y,  y5 = 40 + Pos.Y,  y6 = 55 + Pos.Y,  y7 = 60 + Pos.Y,  y8 = 75 + Pos.Y;

	// The colour sliders and labels
	Env->addStaticText( L"Alpha:",			R(x1, y1, x2, y2), false, false, m_Parent );  
	m_Alpha = Env->addScrollBar( true,		R(x3, y1, x4, y2), m_Parent );
	m_AlphaVal = Env->addStaticText( L"255",R(x5, y1, x6, y2), true, false, m_Parent );
	Env->addStaticText( L"Red:",			R(x1, y3, x2, y4), false, false, m_Parent );  
	m_Red = Env->addScrollBar( true,		R(x3, y3, x4, y4), m_Parent );
	m_RedVal = Env->addStaticText( L"0",	R(x5, y3, x6, y4), true, false, m_Parent );
	Env->addStaticText( L"Green:",			R(x1, y5, x2, y6), false, false, m_Parent );  
	m_Green = Env->addScrollBar( true,		R(x3, y5, x4, y6), m_Parent );
	m_GreenVal = Env->addStaticText( L"0",	R(x5, y5, x6, y6), true, false, m_Parent );
	Env->addStaticText( L"Blue:",			R(x1, y7, x2, y8), false, false, m_Parent );  
	m_Blue = Env->addScrollBar( true,		R(x3, y7, x4, y8), m_Parent );
	m_BlueVal = Env->addStaticText( L"0",	R(x5, y7, x6, y8), true, false, m_Parent );
	// The colour box
	m_ColourBox = Env->addStaticText( L"",	R(x7, y1, x8, y8), true, false, m_Parent );

	// Set the scrollbar ranges
	m_Alpha->setMax(255);	m_Red->setMax(255);	m_Green->setMax(255);	m_Blue->setMax(255);
	m_Alpha->setPos(255);	m_Red->setPos(0);	m_Green->setPos(0);		m_Blue->setPos(0);
	m_Alpha->setSmallStep(1);	m_Red->setSmallStep(1);	m_Green->setSmallStep(1);	m_Blue->setSmallStep(1);
	m_Alpha->setLargeStep(1);	m_Red->setLargeStep(1);	m_Green->setLargeStep(1);	m_Blue->setLargeStep(1);

}


CGUIColourSelect::~CGUIColourSelect(void)
{
	m_Parent = NULL;
}

/// Set the colour
void CGUIColourSelect::setColour( video::SColor &Colour )
{
	m_Colour = Colour;
	// Set the scroll bar positions, text values and colour box
	m_Alpha->setPos( m_Colour.getAlpha() );			m_Red->setPos( m_Colour.getRed() );			m_Green->setPos( m_Colour.getGreen() );			m_Blue->setPos( m_Colour.getBlue() );
	m_AlphaVal->setText( S(m_Colour.getAlpha()) );	m_RedVal->setText( S(m_Colour.getRed()) );	m_GreenVal->setText( S( m_Colour.getGreen()) );	m_BlueVal->setText( S(m_Colour.getBlue()) );

	m_ColourBox->setBackgroundColor( m_Colour );
}
/// Get the colour
video::SColor CGUIColourSelect::getColour( void ) const
{
	return m_Colour;
}

void CGUIColourSelect::setEnabled( bool Enabled )
{
	// Enable/Disable all the controls
	m_Alpha->setEnabled( Enabled );
	m_Red->setEnabled( Enabled );
	m_Green->setEnabled( Enabled );
	m_Blue->setEnabled( Enabled );

	m_Enabled = Enabled;
}
bool CGUIColourSelect::getEnabled( void ) const
{
	return m_Enabled;
}

/// A GUI event has arived, check if it's for us
bool CGUIColourSelect::OnGUIEvent( const SEvent &event )
{
	switch( event.GUIEvent.EventType )
	{
	case gui::EGET_SCROLL_BAR_CHANGED:
		// Scrollbar changed, check for one we handle
		if (event.GUIEvent.Caller == m_Alpha)
		{
			// Alpha changed
			m_Colour.setAlpha( m_Alpha->getPos() );
			m_AlphaVal->setText( S(m_Alpha->getPos()) );
			m_ColourBox->setBackgroundColor( m_Colour );
			return true;
		}
		if (event.GUIEvent.Caller == m_Red)
		{
			// Alpha changed
			m_Colour.setRed( m_Red->getPos() );
			m_RedVal->setText( S(m_Red->getPos()) );
			m_ColourBox->setBackgroundColor( m_Colour );
			return true;
		}
		if (event.GUIEvent.Caller == m_Green)
		{
			// Alpha changed
			m_Colour.setGreen( m_Green->getPos() );
			m_GreenVal->setText( S(m_Green->getPos()) );
			m_ColourBox->setBackgroundColor( m_Colour );
			return true;
		}
		if (event.GUIEvent.Caller == m_Blue)
		{
			// Alpha changed
			m_Colour.setBlue( m_Blue->getPos() );
			m_BlueVal->setText( S(m_Blue->getPos()) );
			m_ColourBox->setBackgroundColor( m_Colour );
			return true;
		}
		return false;
		break;
	default:
		return false;
		break;
	}
	
	return false;
}
#pragma once

using namespace irr;

class CGUIColourSelect
{
public:
	CGUIColourSelect( gui::IGUIEnvironment* Env, gui::IGUIWindow* Parent, core::vector2d<s32> Pos );
	~CGUIColourSelect(void);

	void setColour( video::SColor &Colour );
	video::SColor getColour( void ) const;

	void setEnabled( bool Enabled );
	bool getEnabled( void ) const;

	bool OnGUIEvent( const SEvent &event );

private:
	/// The parent window
	gui::IGUIWindow*		m_Parent;
	// The colour scroll bars
	gui::IGUIScrollBar*		m_Alpha;
	gui::IGUIScrollBar*		m_Red;
	gui::IGUIScrollBar*		m_Green;
	gui::IGUIScrollBar*		m_Blue;
	// The colour values
	gui::IGUIStaticText*	m_AlphaVal;
	gui::IGUIStaticText*	m_RedVal;
	gui::IGUIStaticText*	m_GreenVal;
	gui::IGUIStaticText*	m_BlueVal;
	/// The colour image box
	gui::IGUIStaticText*	m_ColourBox;

	/// The actual colour
	video::SColor			m_Colour;

	bool m_Enabled;		///< the Enabled flag

};


#pragma once

using namespace irr;

/// Class handling the GUI item for each image device.
class CGUIEditorControl :
	public IReferenceCounted
{
public:
	CGUIEditorControl( gui::IGUIEnvironment* Env, video::IVideoDriver* Driver );
	virtual ~CGUIEditorControl(void);

	bool Create( const io::path &BGTexture, u32 NumInputs, u32 NumOutputs, const core::stringw &Text, core::vector2d<s32> &Pos );

	bool isPointInside( const core::vector2d<s32> &Point ) const;
	s32 getInputIndex( const core::vector2d<s32> &Point ) const;
	s32 getOutputIndex( const core::vector2d<s32> &Point ) const;

	core::vector2d<s32> getInputPoint( u32 idx ) const;
	core::vector2d<s32> getOutputPoint( u32 idx ) const;

	bool isElement( const gui::IGUIElement* Element ) const;

	core::vector2d<s32> getPosition( void ) const;
	void setPosition( const core::vector2d<s32> &Pos );
	core::rect<s32> getRect( void ) const;

	void move( const core::vector2d<s32> &Pos );
	void moveTo( const core::vector2d<s32> &Pos );

protected:
	gui::IGUIEnvironment*	m_GUIEnv;	///< The GUI environment
	video::IVideoDriver*	m_Driver;	///< The video driver

	core::vector2d<s32>		m_Pos;		///< Location of the item in the workspace

	gui::IGUIImage*			m_BGImage;	///< The background image
	gui::IGUIStaticText*	m_Text;		///< The text item
	
	core::array<gui::IGUIImage*> m_InputImages;		///< Images for each input item
	core::array<gui::IGUIImage*> m_OutputImages;	///< Images for each output item
};


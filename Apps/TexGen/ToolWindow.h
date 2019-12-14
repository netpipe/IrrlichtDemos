#pragma once

using namespace irr;

/// The tool window class.
/// Handles the editors tool window
class CToolWindow
{
public:
	CToolWindow( gui::IGUIEnvironment* Env );
	~CToolWindow(void);

	bool Create( void );

	gui::IGUIWindow* getGUIWindow( void ) const;

	void setWindowRect( core::rect<s32> Rect );
	core::rect<s32> getWindowRect( void ) const;

	void setPreviewImage( video::ITexture* Tex );

	u32 addListItem( const c8* Text );
	void removeListItem( u32 Idx );

	bool OnEvent( SEvent& event );

private:

	gui::IGUIEnvironment* m_GUIEnv;

	/// The gui elements for the tool window
	gui::IGUIWindow*	m_Window;	///< The actual window
	gui::IGUIImage*		m_Preview;	///< The preview image box
	gui::IGUIListBox*	m_List;		///< The list box of the devices
};


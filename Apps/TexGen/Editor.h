#pragma once

#include "CGUITTFont.h"
#include "ToolWindow.h"
#include "Workspace.h"

/// The main editor class
class CEditor :
	public IEventReceiver
{
public:
	CEditor(void);
	~CEditor(void);

	// Create the editor
	bool Create( void );

	// The main loop
	int Main( void );

	// Get the tool window
	CToolWindow* getToolWindow( void ) const;

	// Update the property window
	void updatePropertyWindow( io::IAttributes* Attribs );

	// The window has been resized
	void OnResize( void );

	// The event function
	virtual bool OnEvent(const SEvent& event);

private:

	// Create the editors GUI
	bool loadFont( void );
	bool createGUI( void );
	bool createEditorTextures( void );
	void createMenu( void );
	void createToolbar( void );

	// Window size functions
	void updateWindowRect( void );
	s32 getWindowWidth( void );
	s32 getWindowHeight( void );

	// One of the menu items has been selected
	bool OnGUIEvent( const SEvent& event );
	bool OnMouseEvent( const SEvent& event );
	bool OnKeyEvent( const SEvent& event );
	bool OnMenuItemSelected( gui::IGUIContextMenu* Menu );
	bool OnButtonSelected( gui::IGUIButton* Button );

	/// The Irrlict device
	IrrlichtDevice*		m_Device;

	// The other irrlicht objects
	gui::IGUIEnvironment*	m_GUIEnv;		///< The GUI environment
	video::IVideoDriver*	m_Driver;		///< The video driver
	io::IFileSystem*		m_FileSystem;	///< The file system
	scene::ISceneManager*	m_SceneManager;	///< The scene manager

	/// The editors workspace
	CWorkspace*			m_Workspace;
	/// The texture factory
	CTextureFactory*	m_TextureFactory;

	/// The editors size
	core::rect<s32>		m_WindowRect;

	// The GUI font
	gui::CGUITTFont*		m_GUIFont;
	//CGUIFreetypeFont*		m_GUIFont;	///< The font for the GUI
	//CGUITTFace*				m_GUIFace;	///< The font face

	// The GUI items
	gui::IGUIContextMenu*	m_MenuBar;		///< The menubar
	gui::IGUIToolBar*		m_Toolbar;		///< The toolbar
	gui::IGUIWindow*		m_PropertyBox;	///< The property box
	gui::IGUITable*			m_PropertyTable; ///< The property table

	CToolWindow*		m_ToolWindow;	///< The editors tool window

};


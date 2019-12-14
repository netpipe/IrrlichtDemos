#include "StdAfx.h"
#include "Editor.h"


CEditor::CEditor(void)
: m_Device( NULL )
, m_Driver( NULL )
, m_FileSystem( NULL )
, m_GUIEnv( NULL )
, m_SceneManager( NULL )
, m_GUIFont( NULL )
//, m_GUIFace( NULL )
, m_MenuBar( NULL )
, m_PropertyBox( NULL )
, m_TextureFactory( NULL )
, m_Toolbar( NULL )
, m_ToolWindow( NULL )
, m_Workspace( NULL )
{
}

CEditor::~CEditor(void)
{

	// Drop the workspace
	if (m_Workspace)
		m_Workspace->drop();

	// Drop the texture factory
	if (m_TextureFactory)
		m_TextureFactory->drop();

	// Drop the font
	if (m_GUIFont)
		m_GUIFont->drop();

	// Drop the Irrlicht objects
	if (m_Driver)
		m_Driver->drop();
	if (m_FileSystem)
		m_FileSystem->drop();
	if (m_GUIEnv)
		m_GUIEnv->drop();
	if (m_SceneManager)
		m_SceneManager->drop();

	// Drop the device
	if (m_Device)
		m_Device->drop();
}

/// Create the editor
bool CEditor::Create( void )
{

	LOG("Creating Irrlict device...");

	// Create the Irrlicht device
	m_Device = createDevice( video::EDT_OPENGL, core::dimension2d<u32>(1024, 768), 32, false, false, true, this);

	// Check for failure
	if (!m_Device)
	{
		LOG("Error creating Irrlicht device.");
		return false;
	}

	// Set the window caption
	m_Device->setWindowCaption(L"Procedural Texture Generator");
	m_Device->setResizable( true );

	// Get the other irrlicht objects
	m_Driver = m_Device->getVideoDriver();
	m_Driver->grab();
	m_FileSystem = m_Device->getFileSystem();
	m_FileSystem->grab();
	m_GUIEnv = m_Device->getGUIEnvironment();
	m_GUIEnv->grab();
	m_SceneManager = m_Device->getSceneManager();
	m_SceneManager->grab();

	// Create the texture factory
	LOG("Creating texture factory...");
	m_TextureFactory = new CTextureFactory( m_Driver, m_FileSystem );
	if (!m_TextureFactory)
	{
		LOG("Error creating texture factory.");
		return false;
	}

	// Load up the font unsed in the editor
	LOG("Loading font...");
	if (!loadFont())
		return false;

	// Setup the workspace
	LOG("Creating workspace...");
	m_Workspace = new CWorkspace( this, m_Device, m_TextureFactory );
	if (!m_Workspace)
		return false;

	// Create the GUI
	LOG("Creating the GUI...");
	if (!createGUI())
		return false;

	// Load up the default workspace layout
	m_Workspace->loadWorkspace( "resources/default.xml" );

	return true;
}

// The main loop
int CEditor::Main( void )
{

	int lastFPS = 0;

	// Save the old width and height
	core::rect<s32> oldR = m_WindowRect;

	while(m_Device->run())
	{
		m_Driver->beginScene(true, true, video::SColor(255,100,101,140));

		m_SceneManager->drawAll();

		// Draw the connection lines
		m_Workspace->drawLinks();

        m_GUIEnv->drawAll();

		m_Driver->endScene();

		// Check to see if the main window has been resized, this is because there is no resize event
		updateWindowRect();
		if (m_WindowRect != oldR)
		{
			OnResize();
			// Save the old width and height
			oldR = m_WindowRect;
		}

		int fps = m_Driver->getFPS();

        if (lastFPS != fps)
        {
			core::stringw tmp(L"Procedural Texture Generator [");
            tmp += m_Driver->getName();
            tmp += L"] fps: ";
            tmp += fps;

            m_Device->setWindowCaption(tmp.c_str());
            lastFPS = fps;
		}

	}

	// Save the default workspace
	m_Workspace->saveWorkspace( "resources/default.xml" );

	return 0;
}

// Get the tool window
CToolWindow* CEditor::getToolWindow( void ) const
{
	return m_ToolWindow;
}

/// Update the property window with the details from an attribute object
void CEditor::updatePropertyWindow( io::IAttributes* Attribs )
{
	// Check
	if (!Attribs)
		return;

	// Clear the property list
	m_PropertyTable->clearRows();

	// Loop through all attributes
	for (u32 i = 0; i < Attribs->getAttributeCount(); i++ )
	{
		u32 row = m_PropertyTable->addRow(i);
		// Set the data
		m_PropertyTable->setCellText( row, 0, core::stringc(Attribs->getAttributeName(i) ) );

		video::SColor col;

		// find out the type
		switch (Attribs->getAttributeType( i ) )
		{
		case io::EAT_COLOR:
			col = Attribs->getAttributeAsColor( i );
			if ( video::SColor(255,200,200,200) < col )
				col = video::SColor(255,0,0,0);
			m_PropertyTable->setCellText( row, 1, core::stringw(Attribs->getAttributeAsStringW( i ) ), col );
			break;
		default:
			m_PropertyTable->setCellText( row, 1, core::stringw(Attribs->getAttributeAsStringW( i ) ) );
			break;
		}
	}
}

/// The main editor window has been resized
void CEditor::OnResize( void )
{
	// Resize the property window
	core::rect<s32> PRect = R( getWindowWidth() - 200, 50, getWindowWidth(), getWindowHeight() );
	core::rect<s32> TRect = R( 4, 24, 196, getWindowHeight() - 56 );

	m_PropertyBox->setRelativePosition( PRect );
	m_PropertyTable->setRelativePosition( TRect );

	m_ToolWindow->setWindowRect( R( 0, 50, 138, getWindowHeight() ) );

	// Resize the workspace
	core::rect<s32> WRect = R( 138, 50, getWindowWidth() - 200, getWindowHeight() );
	m_Workspace->OnResize( WRect );
}

/// Load up the TTF we need for the editor.
bool CEditor::loadFont( void )
{
	// Create the font
	//m_GUIFont = new CGUIFreetypeFont( m_Driver );
	//m_GUIFace = new CGUITTFace();

	// Get the environment variable for the window dir
	//char* pBuf;
	//size_t len;

	//_dupenv_s( &pBuf, &len, "windir" );
	//core::stringc fontpath = core::stringc( pBuf );
	//fontpath += "/Fonts/segoeui.ttf";
	//free( pBuf );
	core::stringc fontpath = "./resources/fonts/ColabReg.otf";

	// Create the font
	m_GUIFont = gui::CGUITTFont::createTTFont( m_GUIEnv, fontpath, 14 );

	if (!m_GUIFont)
	{
		LOG("Could not load font. Using default.");
	}
	else
	{
		LOG("Loaded font segoeui.ttf");
		// Set the GUI to the new font
		for ( u32 i = 0; i < gui::EGDF_COUNT; i++ )
		{
			m_GUIEnv->getSkin()->setFont( m_GUIFont, (gui::EGUI_DEFAULT_FONT)i );
		}
	}
	/*
	if ( m_GUIFace->load( fontpath ) )
	{
		LOG("Loaded font segoeui.ttf");

		m_GUIFont->attach( m_GUIFace, 12 );
		// Set the GUI to the new font
		for ( u32 i = 0; i < gui::EGDF_COUNT; i++ )
		{
			m_GUIEnv->getSkin()->setFont( m_GUIFont, (gui::EGUI_DEFAULT_FONT)i );
		}
	}
	else
	{
		LOG("Could not load font. Using default.");
	}
	*/
	return true;
}

/// Create the editors GUI
bool CEditor::createGUI( void )
{
	// Set transparencys
	for ( u32 i = 0; i < gui::EGDC_COUNT ; i++ )
    {
		video::SColor col = m_GUIEnv->getSkin()->getColor((gui::EGUI_DEFAULT_COLOR)i);
		col.setAlpha(255);
		m_GUIEnv->getSkin()->setColor((gui::EGUI_DEFAULT_COLOR)i, col);
	}

	// Create the editors textures
	if (!createEditorTextures())
	{
		LOG("Error creating editors textures.");
		return false;
	}

	// Get the size of the window
	updateWindowRect();

	// Create the menu
	createMenu();

	// Create the toolbar
	createToolbar();

	// Create the toolbox
	m_ToolWindow = new CToolWindow( m_GUIEnv );
	if (!m_ToolWindow)
		return false;
	if (!m_ToolWindow->Create())
		return false;
	m_ToolWindow->setWindowRect( R( 0, 50, 138, getWindowHeight() ) );

	////////////////////////////////////
	// Setup the properties window

	m_PropertyBox = m_GUIEnv->addWindow( R( getWindowWidth() - 200, 50, getWindowWidth(), getWindowHeight() ),
												false,
												L"Properties",
												0,
												ID_GUI_PROPERTY_WINDOW );

	// Create the table
	m_PropertyTable = m_GUIEnv->addTable(	R( 4, 24, 196, getWindowHeight() - 56 ),
											m_PropertyBox,
											ID_GUI_PROPERTY_TABLE,
											true );
	// Add colunms to the table
	m_PropertyTable->addColumn( L"", 0 );
	m_PropertyTable->setColumnWidth( 0, 80 );
	m_PropertyTable->addColumn( L"", 1 );
	m_PropertyTable->setColumnWidth( 1, 110 );
	// no table ordering
	m_PropertyTable->setColumnOrdering( 0, gui::EGCO_NONE );

	// Update the window posistions and workspace rect
	OnResize();

	return true;
}

/// Create the textures for the editor
bool CEditor::createEditorTextures( void )
{
	// Check
	if (!m_TextureFactory)
		return false;

	LOG("Creating GUI images...");
	// Load up the device background images
	// Load up the texgen_gen.xml file to create a background image for the generator processes
	m_TextureFactory->loadProcesses( "resources/texgen_gen.xml" );
	m_TextureFactory->addTexture( "Gen", 650, 0, 256, 256 );
	// Load up the texgen_conv.xml file to create a bacground image for the converter processes
	m_TextureFactory->loadProcesses( "resources/texgen_conv.xml" );
	m_TextureFactory->addTexture( "Conv", 650, 0, 256, 256 );
	// Load up the texgen_link.xml file to create a background image for the link items
	m_TextureFactory->loadProcesses( "resources/texgen_link.xml" );
	m_TextureFactory->addTexture( "Link", 650, 0, 128, 128 );

	LOG("Creating toolbar images...");
	// Load up the toolbar images
	// Perlin device
	m_TextureFactory->loadProcesses( "resources/texgen_dev_perlin.xml" );
	m_TextureFactory->addTexture( "Device_Perlin", 600, 0, 20, 20 );
	// Gradient device
	m_TextureFactory->loadProcesses( "resources/texgen_dev_gradient.xml" );
	m_TextureFactory->addTexture( "Device_Gradient", 600, 0, 20, 20 );
	// Colourize device
	m_TextureFactory->loadProcesses( "resources/texgen_dev_colourize.xml" );
	m_TextureFactory->addTexture( "Device_Colourize", 650, 0, 20, 20 );
	// Advanced colourize device
	m_TextureFactory->loadProcesses( "resources/texgen_dev_colourize2.xml" );
	m_TextureFactory->addTexture( "Device_Colourize2", 650, 0, 20, 20 );
	// Blend device
	m_TextureFactory->loadProcesses( "resources/texgen_dev_colourize2.xml" );
	m_TextureFactory->addTexture( "Device_Blend", 650, 0, 20, 20 );

	return true;
}

/// Create the editors menubar
void CEditor::createMenu( void )
{
	// Add the menubar
	m_MenuBar = m_GUIEnv->addMenu( 0, ID_GUI_MENUBAR );

	// File menu items
	u32 filemenuid = m_MenuBar->addItem( L"File", ID_GUI_MENUBAR_FILE, true, true, false, false );
	gui::IGUIContextMenu* filemenu = m_MenuBar->getSubMenu( filemenuid );
	filemenu->addItem( L"New", ID_GUI_MENUBAR_FILE_NEW );
	filemenu->addSeparator();
	filemenu->addItem( L"Open", ID_GUI_MENUBAR_FILE_OPEN );
	filemenu->addSeparator();
	filemenu->addItem( L"Save", ID_GUI_MENUBAR_FILE_SAVE );
	filemenu->addItem( L"Save As", ID_GUI_MENUBAR_FILE_SAVE_AS );
	filemenu->addSeparator();
	filemenu->addItem( L"Export Texture Script", ID_GUI_MENUBAR_FILE_EXPORT );
	filemenu->addSeparator();
	filemenu->addItem( L"Export Large Texture", ID_GUI_MENUBAR_FILE_EXPORT_PREVIEW );
	filemenu->addSeparator();
	filemenu->addItem( L"Exit", ID_GUI_MENUBAR_FILE_EXIT );
	// Help menu items
	u32 helpmenuid = m_MenuBar->addItem( L"Help", ID_GUI_MENUBAR_HELP, true, true, false, false );
	gui::IGUIContextMenu* helpmenu = m_MenuBar->getSubMenu( helpmenuid );
	helpmenu->addItem( L"Webpage", ID_GUI_MENUBAR_HELP_WEB );
	helpmenu->addItem( L"About", ID_GUI_MENUBAR_HELP_ABOUT );

}
/// Create the editors toolbar
void CEditor::createToolbar( void )
{
	// Create the toolbar
	m_Toolbar = m_GUIEnv->addToolBar( 0, ID_GUI_TOOLBAR );

	video::ITexture* tex;
	// Perlin noise button
	tex = m_Driver->findTexture( "Device_Perlin" );
	m_Toolbar->addButton( ID_GUI_TOOLBAR_DEV_PERLIN, 0, L"Basic perlin noise", tex, 0, true, false );
	// Gradient button
	tex = m_Driver->findTexture( "Device_Gradient" );
	m_Toolbar->addButton( ID_GUI_TOOLBAR_DEV_GRADIENT, 0, L"Gradient", tex, 0, true, false );
	// Colourize button
	tex = m_Driver->findTexture( "Device_Colourize" );
	m_Toolbar->addButton( ID_GUI_TOOLBAR_DEV_COLOURIZE, 0, L"Colourize", tex, 0, true, false );
	// Advanced Colourize button
	tex = m_Driver->findTexture( "Device_Colourize2" );
	m_Toolbar->addButton( ID_GUI_TOOLBAR_DEV_COLOURIZE2, 0, L"Advanced Colourize", tex, 0, true, false );
	// Advanced Colourize button
	tex = m_Driver->findTexture( "Device_Blend" );
	m_Toolbar->addButton( ID_GUI_TOOLBAR_DEV_BLEND, 0, L"Blend", tex, 0, true, false );

}

/// Update the window rect
void CEditor::updateWindowRect( void )
{
	m_WindowRect = m_GUIEnv->getRootGUIElement()->getAbsolutePosition();
}

/// Get the width of the window
s32 CEditor::getWindowWidth( void )
{
	//updateWindowRect();
	return m_WindowRect.LowerRightCorner.X;
}
/// Get the height of the window
s32 CEditor::getWindowHeight( void )
{
	//updateWindowRect();
	return m_WindowRect.LowerRightCorner.Y;
}

// The event function
bool CEditor::OnEvent(const SEvent& event)
{

	switch (event.EventType)
	{
	case EET_GUI_EVENT:
		// GUI event
		return OnGUIEvent( event );
		break;
	case EET_MOUSE_INPUT_EVENT:
		// Mouse event
		return OnMouseEvent( event );
		break;
	case irr::EET_KEY_INPUT_EVENT:
		// Keyboard event
		return OnKeyEvent( event );
		break;
	default:
		return false;
		break;
	}

	return false;
}

/// GUI event recieved
bool CEditor::OnGUIEvent( const SEvent& event )
{

	// First pass it onto the workspace to handle
	// If it doesn't handle the event we'll try and handle it ourselves
	if (m_Workspace->OnGUIEvent( event ) )
		return true;

	// Not handled by the workspace, try and handle here

	// Check the type of event
	switch ( event.GUIEvent.EventType )
	{
	case gui::EGET_MENU_ITEM_SELECTED:
		// A menu item has been selected
		return OnMenuItemSelected( (gui::IGUIContextMenu*)event.GUIEvent.Caller );
		break;
	case gui::EGET_BUTTON_CLICKED:
		// A button has been clicked
		return OnButtonSelected( (gui::IGUIButton*)event.GUIEvent.Caller );
	default:
		return false;
		break;
	}

	return false;
}

/// Mouse event recieved
bool CEditor::OnMouseEvent( const SEvent& event )
{
	// Get the mouse pos
	core::vector2d<s32> MPos( event.MouseInput.X, event.MouseInput.Y );

	// Check the event
	switch ( event.MouseInput.Event )
	{
	case EMIE_LMOUSE_PRESSED_DOWN:
		// Left mouse clicked
		// Check to see if it's within the workspace
		if ( m_Workspace->isPointInside( MPos ) )
			return m_Workspace->OnLMouseDown( MPos );
		break;
	case EMIE_LMOUSE_LEFT_UP:
		// Left mouse released
		// check to see if it's within the workspace
		if ( m_Workspace->isPointInside( MPos ) )
			return m_Workspace->OnLMouseUp( MPos );
		break;
	case irr::EMIE_LMOUSE_DOUBLE_CLICK:
		// Left mouse doble clicked
		// Check to see if it's within the workspace
		if ( m_Workspace->isPointInside( MPos ) )
			return m_Workspace->OnLMouseDblClick( MPos );
		break;
	case EMIE_MOUSE_MOVED:
		// Mouse has been moved
		if ( m_Workspace->isPointInside( MPos ) )
			return m_Workspace->OnMouseMove( MPos, event.MouseInput.isLeftPressed(), event.MouseInput.isRightPressed() );
	default:
		break;
	}

	return false;

}

bool CEditor::OnKeyEvent( const SEvent& event )
{
	// check for the delete key
	if (event.KeyInput.Key == KEY_DELETE)
		return m_Workspace->OnDelKeyPressed();

	return false;
}

/// A menu item has been selected
bool CEditor::OnMenuItemSelected( gui::IGUIContextMenu* Menu )
{
	// Get the id of the selected item
	s32 id = Menu->getItemCommandId( Menu->getSelectedItem() );

	switch (id)
	{
	case ID_GUI_MENUBAR_FILE_NEW:
		// Load up the default workspace layout
		m_Workspace->loadWorkspace( "resources/default.xml" );
		return true;
		break;
	case ID_GUI_MENUBAR_FILE_EXPORT:
		// Export the TextureFactory to an XML file
		// Show a file save dialog box
		//m_GUIData->guienv->addFileOpenDialog( L"Export Texture Script", true, 0, ID_GUI_EXPORT_DIALOG );
		//dialog->ser

		// Export the CTextureFactory
		m_TextureFactory->saveProcesses( "export/export.xml" );
		break;
	case ID_GUI_MENUBAR_FILE_EXPORT_PREVIEW:
		// Save a preview image of the selected device
		m_Workspace->savePreviewImage( );
		return true;

		break;
	case ID_GUI_MENUBAR_FILE_EXIT:
		// File -> Exit command
		m_Device->closeDevice();
		return true;
		break;
	}

	return false;
}

/// A button has been pressed in the editor
bool CEditor::OnButtonSelected( gui::IGUIButton* Button )
{
	// Get the id of the button
	u32 id = Button->getID();

	// Untick all of the other toolbar buttons
	for (u32 i = ID_GUI_TOOLBAR + 1; i < ID_GUI_TOOLBAR_BUTTON_COUNT; i++ )
	{
		if (i != id)
		{
			gui::IGUIButton* b = (gui::IGUIButton*)Button->getParent()->getElementFromId( i, true );
			if (b)
				if (b->isPressed())
					b->setPressed(false);
		}
	}

	switch (id)
	{
	case ID_GUI_TOOLBAR_DEV_PERLIN:
		// Perlin noise button has been pressed
		// Check if the button was pressed
		if (Button->isPressed())
			m_Workspace->setAddDeviceMode( PT_PERLIN_NOISE );
		break;
	case ID_GUI_TOOLBAR_DEV_GRADIENT:
		// Gradient button has been pressed
		// Check if the button was pressed
		if (Button->isPressed())
			m_Workspace->setAddDeviceMode( PT_GRADIENT );
		break;
	case ID_GUI_TOOLBAR_DEV_COLOURIZE:
		// Colourize button has been pressed
		if (Button->isPressed())
			m_Workspace->setAddDeviceMode( PT_COLOURIZE );
		break;
	case ID_GUI_TOOLBAR_DEV_COLOURIZE2:
		// Advanced colourize button has been pressed
		if (Button->isPressed())
			m_Workspace->setAddDeviceMode( PT_COLOURIZE_ADVANCED );
		break;
	case ID_GUI_TOOLBAR_DEV_BLEND:
		// Advanced colourize button has been pressed
		if (Button->isPressed())
			m_Workspace->setAddDeviceMode( PT_BLEND );
		break;
	default:
		// Unknown button, don't handle it
		m_Workspace->setAddDeviceMode( PT_NONE );
		return false;
	}

	return false;
}

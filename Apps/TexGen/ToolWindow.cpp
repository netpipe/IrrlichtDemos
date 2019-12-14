#include "StdAfx.h"
#include "ToolWindow.h"

CToolWindow::CToolWindow( gui::IGUIEnvironment* Env )
: m_GUIEnv( Env )
{
	// Grab the GUI environment
	if (m_GUIEnv)
		m_GUIEnv->grab();
}

CToolWindow::~CToolWindow(void)
{
	// Drop the gui Environment
	if (m_GUIEnv)
		m_GUIEnv->drop();
}

/// Create the tool window
bool CToolWindow::Create( void )
{
	// Check
	if (!m_GUIEnv)
		return false;

	// Create the toolbox
	m_Window = m_GUIEnv->addWindow( R( 0, 50, 138, 300 ),
										false,
										L"Toolbox",
										0,
										ID_GUI_TOOL_WINDOW );

	// Create the preview box
	m_Preview = m_GUIEnv->addImage(	R( 4, 24, 132, 152 ),
										m_Window,
										ID_GUI_TOOL_PREVIEW,
										0 );
	m_Preview->setScaleImage( true );
	m_Preview->setUseAlphaChannel( true );

	// Create a list box
	m_List = m_GUIEnv->addListBox(	R(4, 350, 132, 580),
										m_Window,
										ID_GUI_TOOL_LIST,
										true );

	return true;
}

/// Return a pointer to the actual gui window
gui::IGUIWindow* CToolWindow::getGUIWindow( void ) const
{
	return m_Window;
}

/// Set the windows position
void CToolWindow::setWindowRect( core::rect<s32> Rect )
{
	m_Window->setRelativePosition( Rect );
}
/// Return the window's position
core::rect<s32> CToolWindow::getWindowRect( void ) const
{
	return m_Window->getRelativePosition();
}

/// Set the image in the preview box
void CToolWindow::setPreviewImage( video::ITexture* Tex )
{
	m_Preview->setImage( Tex );
}

/// Add an item to the list
/// \param Text The item to add
u32 CToolWindow::addListItem( const c8* Text )
{
	return m_List->addItem( S(Text) );
}

/// Remove an item from the list
/// \param Idx The index of the item to remove
void CToolWindow::removeListItem( u32 Idx )
{
	m_List->removeItem( Idx );
}

bool CToolWindow::OnEvent( SEvent& event )
{
	return false;
}

#include "StdAfx.h"
#include "Workspace.h"
#include "DeviceGradient.h"
#include "ImgGradient.h"

CDeviceGradient::CDeviceGradient( CWorkspace* workspace, IImageProcess* proc )
: IEditorDevice( workspace, proc )
{
}

CDeviceGradient::~CDeviceGradient(void)
{
}

/// Create a GUI element to display in the editor
bool CDeviceGradient::createGUIItem( core::vector2d<s32> pos )
{
	if (!m_EditorControl)
		return false;

	return m_EditorControl->Create( "Gen", m_pImageProc->getNumInputs(), m_pImageProc->getNumOutputs(), m_pImageProc->getName(), pos );
}

/// Create a GUI window to edit the parameters of this device
gui::IGUIWindow* CDeviceGradient::createGUIParamWindow( void )
{
	// Get the ID
	u32 id = m_pImageProc->getID();

	// Save the previous settings for this device
	m_pImageProc->serializeAttributes(m_PrevSettings);

	gui::IGUIEnvironment* guienv = m_Workspace->getGUIEnvironment();

	// Create the modal window
	m_PropWindow = guienv->addWindow( R(400,400,650,490), true, L"Properties", 0, id + 10 );
	// Angle slider 0 to 359
	guienv->addStaticText( L"Angle:", R(5,25,35,40), false, false, m_PropWindow, -1 );
	gui::IGUIScrollBar* A = guienv->addScrollBar( true, R(40,25,175,40), m_PropWindow, id + 11 );
	// Scale slider 0 to 100 (divide result by 10)
	guienv->addStaticText( L"Scale:", R(5,45,35,60), false, false, m_PropWindow, -1 );
	gui::IGUIScrollBar* S = guienv->addScrollBar( true, R(40,45,175,60), m_PropWindow, id + 12 );

	// Add the start and end colour boxes
	gui::IGUIStaticText* angle = guienv->addStaticText( L"", R(180,25,245,40), true, true, m_PropWindow, id + 13 );
	gui::IGUIStaticText* scale = guienv->addStaticText( L"", R(180,45,245,60), true, true, m_PropWindow, id + 14 );

	// Set the text of the angle and scale
	CImgGradient* proc = (CImgGradient*)m_pImageProc;
	angle->setText( core::stringw( proc->getAngle() ).c_str() );
	scale->setText( core::stringw( proc->getScale() ).c_str() );

	// Setup the scroll bars
	A->setMax(359);	S->setMax(100);
	A->setSmallStep( 1 ); S->setSmallStep( 1 );
	A->setLargeStep( 5 ); S->setLargeStep( 10 );
	A->setPos( proc->getAngle() );
	S->setPos( (s32)(proc->getScale() * 10) );

	// Add the OK / Cancel buttons
	guienv->addButton(R(25,65,120,85), m_PropWindow, id + 19, L"OK" );
	guienv->addButton(R(130,65,225,85), m_PropWindow, id + 20, L"Cancel" );

	return m_PropWindow;
}

/// We have a GUI event while the properies window was open
/// Handle the event here if we can
/// \return True if we handled the event, otherwise false
bool CDeviceGradient::OnGUIEvent( const irr::SEvent &event )
{
	// Get the ID
	u32 id = m_pImageProc->getID();

	// Check the event type
	if ( event.EventType == EET_GUI_EVENT )
	{
		// Get the scroll bars
		gui::IGUIScrollBar* A = (gui::IGUIScrollBar*)m_PropWindow->getElementFromId( id + 11, true );
		gui::IGUIScrollBar* S = (gui::IGUIScrollBar*)m_PropWindow->getElementFromId( id + 12, true );

		s32 a = A->getPos();	// Angle
		f32 s = (f32)(S->getPos()) / 10; // Scale

		CImgGradient* proc = (CImgGradient*)m_pImageProc;	// Image process

		// Get the two info boxes
		gui::IGUIStaticText* angle = (gui::IGUIStaticText*)m_PropWindow->getElementFromId( id + 13, true );
		gui::IGUIStaticText* scale = (gui::IGUIStaticText*)m_PropWindow->getElementFromId( id + 14, true );

		if ( event.GUIEvent.EventType == gui::EGET_SCROLL_BAR_CHANGED )
		{
			// A scroll bar was changed
			// Refresh the two data boxes
			angle->setText( S( a ) );
			scale->setText( S( s ) );

			// Set the precesses settings
			proc->setAngle( a );
			proc->setScale( s );

			// Update the preview window
			m_Workspace->showPreview( this );

			return true;
		}
		if ( event.GUIEvent.EventType == gui::EGET_BUTTON_CLICKED )
		{
			// One of the buttons was clicked
			if ( event.GUIEvent.Caller->getID() == (id + 19) )
			{
				// OK button, clear previous settings and remove the from
				m_PrevSettings->clear();
				// remove the form
				m_PropWindow->remove();
				// Clear the guidata items
				m_PropWindow = NULL;

				// Call the workspace's propwindow closed event
				m_Workspace->OnDevicePropWindowClose( this );

				return true;
			}
			if ( event.GUIEvent.Caller->getID() == (id + 20) )
			{
				// Cancel button, put the settings back to default
				proc->deserializeAttributes( m_PrevSettings );
				// Clear the previous settings
				m_PrevSettings->clear();
				// remove the modal form
				m_PropWindow->remove();
				m_PropWindow = NULL;

				// Call the workspace's propwindow closed event
				m_Workspace->OnDevicePropWindowClose( this );

				return true;
			}
		}
		if (event.GUIEvent.EventType == gui::EGDT_WINDOW_CLOSE)
		{
			// Close button, put the settings back to default
			proc->deserializeAttributes( m_PrevSettings );
			// Clear the previous settings
			m_PrevSettings->clear();

			m_PropWindow->remove();
			m_PropWindow = NULL;

			// Call the workspace's propwindow closed event
			m_Workspace->OnDevicePropWindowClose( this );

			return true;
		}
	}

	return false;
}

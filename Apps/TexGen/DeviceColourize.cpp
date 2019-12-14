#include "StdAfx.h"
#include "Workspace.h"
#include "DeviceColourize.h"

CDeviceColourize::CDeviceColourize( CWorkspace* workspace, IImageProcess *proc )
: IEditorDevice( workspace, proc )
{
}

CDeviceColourize::~CDeviceColourize(void)
{
}

/// Create a GUI element to display in the editor
bool CDeviceColourize::createGUIItem( core::vector2d<s32> pos )
{
	if (!m_EditorControl)
		return false;

	return m_EditorControl->Create( "Conv", m_pImageProc->getNumInputs(), m_pImageProc->getNumOutputs(), m_pImageProc->getName(), pos );
}

/// Create a GUI window to edit the parameters of this device
gui::IGUIWindow* CDeviceColourize::createGUIParamWindow( void )
{

	// Get the ID
	u32 id = m_pImageProc->getID();
	CImgColourize* proc = (CImgColourize*)m_pImageProc;	// Image process

	// Save the previous settings for this device
	m_pImageProc->serializeAttributes(m_PrevSettings);

	gui::IGUIEnvironment* guienv = m_Workspace->getGUIEnvironment();

	// Create the modal window
	m_PropWindow = guienv->addWindow( R(400,400,680,625), true, L"Properties", 0, id + 10 );

	// Create the colour select sliders
	m_CS_Start = new CGUIColourSelect( guienv, m_PropWindow, core::vector2d<s32>(5,25) );
	m_CS_End = new CGUIColourSelect( guienv, m_PropWindow, core::vector2d<s32>(5,110) );

///	m_CS_Start->setColour( proc->getStartColour() );
///	m_CS_End->setColour( proc->getEndColour() );

	// Add the OK / Cancel buttons
	guienv->addButton(R(35,195,135,220), m_PropWindow, id + 19, L"OK" );
	guienv->addButton(R(145,195,245,220), m_PropWindow, id + 20, L"Cancel" );

	return m_PropWindow;
}

/// We have a GUI event while the properies window was open
/// Handle the event here if we can
/// \return True if we handled the event, otherwise false
bool CDeviceColourize::OnGUIEvent( const irr::SEvent &event )
{
	// Get the ID
	u32 id = m_pImageProc->getID();
	CImgColourize* proc = (CImgColourize*)m_pImageProc;	// Image process

	// Send it to the two colour select boxes
	if (m_CS_Start->OnGUIEvent( event ))
	{
		// We've handled the event, get the new colour
		proc->setStartColour( m_CS_Start->getColour() );

		m_Workspace->showPreview( this );

		return true;
	}
	if (m_CS_End->OnGUIEvent( event ))
	{
		// We've handled the event get the new end colour
		proc->setEndColour( m_CS_End->getColour() );

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

			// Clean up
			delete m_CS_Start;
			delete m_CS_End;
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
			// Clean up
			delete m_CS_Start;
			delete m_CS_End;
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
		// Clean up
		delete m_CS_Start;
		delete m_CS_End;
		m_PropWindow = NULL;

		// Call the workspace's propwindow closed event
		m_Workspace->OnDevicePropWindowClose( this );

		return true;
	}

	return false;
}

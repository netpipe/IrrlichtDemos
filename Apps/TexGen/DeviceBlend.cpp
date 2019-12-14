#include "StdAfx.h"
#include "DeviceBlend.h"
#include "Workspace.h"

CDeviceBlend::CDeviceBlend( CWorkspace* workspace, IImageProcess *proc )
: IEditorDevice( workspace, proc )
{
}


CDeviceBlend::~CDeviceBlend(void)
{
}

/// Create a GUI element to display in the editor
bool CDeviceBlend::createGUIItem( core::vector2d<s32> pos )
{
	if (!m_EditorControl)
		return false;

	return m_EditorControl->Create( "Conv", m_pImageProc->getNumInputs(), m_pImageProc->getNumOutputs(), m_pImageProc->getName(), pos );
}

/// Create a GUI window to edit the parameters of this device
gui::IGUIWindow* CDeviceBlend::createGUIParamWindow( void )
{

	// Get the ID
	u32 id = m_pImageProc->getID();
	CImgBlend* proc = (CImgBlend*)m_pImageProc;	// Image process

	// Save the previous settings for this device
	m_pImageProc->serializeAttributes(m_PrevSettings);

	gui::IGUIEnvironment* guienv = m_Workspace->getGUIEnvironment();

	// Create the modal window
	m_PropWindow = guienv->addWindow( R(400,400,680,625), true, L"Properties", 0, id + 10 );

	// Add the blend mode combo box
	m_BlendMode = guienv->addComboBox( R(5,25,200,40), m_PropWindow, id + 11 );
	// Add the modes to the combo box
	for ( u32 i = 0; i < BM_COUNT; i++ )
		m_BlendMode->addItem( S(BM_String[i]), i );

	// Set the selected item
	m_BlendMode->setSelected( m_BlendMode->getIndexForItemData( (u32)proc->getBlendMode() ) );

	// Add the OK / Cancel buttons
	guienv->addButton(R(35,195,135,220), m_PropWindow, id + 19, L"OK" );
	guienv->addButton(R(145,195,245,220), m_PropWindow, id + 20, L"Cancel" );

	return m_PropWindow;
}

/// We have a GUI event while the properies window was open
/// Handle the event here if we can
/// \return True if we handled the event, otherwise false
bool CDeviceBlend::OnGUIEvent( const irr::SEvent &event )
{
	// Get the ID
	u32 id = m_pImageProc->getID();
	CImgBlend* proc = (CImgBlend*)m_pImageProc;	// Image process

	if ( event.GUIEvent.EventType == gui::EGET_COMBO_BOX_CHANGED )
	{
		// Change the blend mode
		BLEND_MODE Mode = (BLEND_MODE)m_BlendMode->getItemData( m_BlendMode->getSelected() );
		proc->setBlendMode( Mode );

		// Update the preview
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
		m_PropWindow = NULL;

		// Call the workspace's propwindow closed event
		m_Workspace->OnDevicePropWindowClose( this );

		return true;
	}

	return false;
}

#include "StdAfx.h"
#include "Workspace.h"
#include "DeviceColourize2.h"
//#include "imgcolourize2.h"

CDeviceColourize2::CDeviceColourize2( CWorkspace* workspace, IImageProcess *proc )
: IEditorDevice( workspace, proc )
{
}

CDeviceColourize2::~CDeviceColourize2(void)
{
}

/// Create a GUI element to display in the editor
bool CDeviceColourize2::createGUIItem( core::vector2d<s32> pos )
{
	if (!m_EditorControl)
		return false;

	return m_EditorControl->Create( "Conv", m_pImageProc->getNumInputs(), m_pImageProc->getNumOutputs(), m_pImageProc->getName(), pos );
}

/// Create a GUI window to edit the parameters of this device
gui::IGUIWindow* CDeviceColourize2::createGUIParamWindow( void )
{

	// Get the ID
	u32 id = m_pImageProc->getID();

	// Save the previous settings for this device
	m_pImageProc->serializeAttributes(m_PrevSettings);

	// Add the gradient and colourize processes to the texture factory.
	// We'll remove them when we close the property window
	//m_Grad = (CImgGradient*)m_pGUIData->TextureFactory->addProcess( PT_GRADIENT, 10000 );
	//m_Col2 = (CImgColourize2*)m_pGUIData->TextureFactory->addProcess( PT_COLOURIZE_ADVANCED, 10050 );
	// Add the link
	//m_pGUIData->TextureFactory->addLink( m_Grad, 0, m_Col2, 0 );
	//m_pGUIData->TextureFactory->addTexture( "Colourize2_Prev", m_Col2->getID(), 0, 256, 32 );
	gui::IGUIEnvironment* guienv = m_Workspace->getGUIEnvironment();

	// Create the modal window
	m_PropWindow = guienv->addWindow( R(400,400,775,580), true, L"Properties", 0, id + 10 );

	guienv->addStaticText( L"Colour Points:", R(5, 25, 95, 40), false, false, m_PropWindow, -1, false );
	m_PointList = guienv->addListBox( R(5, 45, 95, 150), m_PropWindow, id + 11, true );
	// The colour sliders
	m_CS = new CGUIColourSelect( guienv, m_PropWindow, core::vector2d<s32>(100, 25) );

	// Add a scroll bar for delta
	guienv->addStaticText( L"Delta:", R(100,105,135,130), false, false, m_PropWindow, -1 );
	m_DS = guienv->addScrollBar( true, R(140,105,270,120), m_PropWindow, id + 16 );
	m_DST = guienv->addStaticText( L"", R(275,105,340,120), true, false, m_PropWindow, id + 17 );

	// Add the Add / Remove point buttons
	m_AddP = guienv->addButton(R(100,125,215,145), m_PropWindow, id + 17, L"Add Point" );
	m_RemP = guienv->addButton(R(225,125,340,145), m_PropWindow, id + 18, L"Remove Point" );

	// Get the process
	CImgColourize2* proc = (CImgColourize2*)m_pImageProc;
	// Loop through all points and add them to the list
	for (u32 i = 0; i < proc->getNumColourPoints(); i++ )
	{
		// Get the colour
		video::SColor col = proc->getColourPointColour( i );

		core::stringw Str;
		Str = Str + L"Point ID:" + S(i);
		m_PointList->addItem( Str.c_str() );
	}

	// Disable the controls
	m_CS->setEnabled( false );
	m_DS->setEnabled( false );
	m_DS->setMin( 0 );
	m_DS->setMax( 100 );
	m_DS->setPos( 0 );
	m_DS->setSmallStep( 1 );
	m_DS->setLargeStep( 10 );

	m_RemP->setEnabled( false );

	// Add the OK / Cancel buttons
	guienv->addButton(R(25,155,120,175), m_PropWindow, id + 25, L"OK" );
	guienv->addButton(R(130,155,225,175), m_PropWindow, id + 26, L"Cancel" );

	return m_PropWindow;
}

/// We have a GUI event while the properies window was open
/// Handle the event here if we can
/// \return True if we handled the event, otherwise false
bool CDeviceColourize2::OnGUIEvent( const irr::SEvent &event )
{
	// Get the ID
	u32 id = m_pImageProc->getID();
	CImgColourize2* proc = (CImgColourize2*)m_pImageProc;	// Image process

	// Get the colours from the colour select box
	if ( event.GUIEvent.EventType == gui::EGET_LISTBOX_CHANGED )
	{
		// An item in the list box was selected
		m_SelPoint = m_PointList->getSelected();
		if (m_SelPoint >= 0)
		{
			// Enable the controls and get the colour for the color box and the delta values
			video::SColor col = proc->getColourPointColour( (u32)m_SelPoint );
			f32 delta = proc->getColourPointDelta( (u32)m_SelPoint );
			// Set the values of the scroll bars
			m_CS->setColour( col );
			m_DS->setPos( (s32)(delta * 100) );
			m_DST->setText( S(delta) );
			// Enable the remove button
			m_RemP->setEnabled( true );
			// Enable the colour sliders
			m_CS->setEnabled( true );
			m_DS->setEnabled( true );
		}
		return true;
	}
	if (m_CS->OnGUIEvent( event ) )
	{
		// The colour select handeled the event
		proc->setColourPointColour( (u32)m_SelPoint, m_CS->getColour() );
		// Show the preview
		m_Workspace->showPreview( this );
		return true;
	}
	else if ( event.GUIEvent.EventType == gui::EGET_SCROLL_BAR_CHANGED )
	{
		// A scroll bar was changed
		// Check for the delta scrollbar
		if ( event.GUIEvent.Caller == m_DS )
		{
			// refresh the delta box
			f32 delta = (f32)(m_DS->getPos()) / 100;
			m_DST->setText( S(delta) );
			// Save the changes
			proc->setColourPointDelta( (u32)m_SelPoint, delta );

			// Show the preview
			m_Workspace->showPreview( this );
			return true;
		}
	}
	if ( event.GUIEvent.EventType == gui::EGET_BUTTON_CLICKED )
	{
		// One of the buttons was clicked
		if ( event.GUIEvent.Caller == m_RemP )
		{
			// The remove point button
			if (m_SelPoint >= 0)
			{

				// remove the point
				proc->removeColourPoint( m_SelPoint );
				// Update the list
				m_PointList->clear();

				// Loop through all points and add them to the list
				for (u32 i = 0; i < proc->getNumColourPoints(); i++ )
				{
					// Get the colour
					video::SColor col = proc->getColourPointColour( i );

					core::stringw Str;
					Str = Str + L"Point ID:" + S(i);
					m_PointList->addItem( Str.c_str() );
				}

				// Show the preview
				m_Workspace->showPreview( this );
			}

			return true;
		}
		if ( event.GUIEvent.Caller == m_AddP )
		{

			// Add a new point
			proc->addColourPoint( video::SColor( 255, 128, 128, 128 ), 0.5f );
			// Update the list
			m_PointList->clear();

			// Loop through all points and add them to the list
			for (u32 i = 0; i < proc->getNumColourPoints(); i++ )
			{
				// Get the colour
				video::SColor col = proc->getColourPointColour( i );

				core::stringw Str;
				Str = Str + L"Point ID:" + S(i);
				m_PointList->addItem( Str.c_str() );
			}

			// Show the preview
			m_Workspace->showPreview( this );

			return true;

		}
		if ( event.GUIEvent.Caller->getID() == (id + 25) )
		{
			// OK button, clear previous settings and remove the from
			m_PrevSettings->clear();
			// remove the form
			m_PropWindow->remove();
			// Clear the guidata items
			m_PropWindow = NULL;
			delete m_CS;

			// Call the workspace's propwindow closed event
			m_Workspace->OnDevicePropWindowClose( this );

			return true;
		}
		if ( event.GUIEvent.Caller->getID() == (id + 26) )
		{
			// Cancel button, put the settings back to default
			proc->deserializeAttributes( m_PrevSettings );
			// Clear the previous settings
			m_PrevSettings->clear();
			// remove the modal form
			m_PropWindow->remove();
			m_PropWindow = NULL;
			delete m_CS;

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
		delete m_CS;

		// Call the workspace's propwindow closed event
		m_Workspace->OnDevicePropWindowClose( this );

		return true;
	}

	return false;
}

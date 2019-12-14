#include "StdAfx.h"
#include "Workspace.h"
#include "DevicePerlin.h"
#include "ImgPerlinNoise.h"

CDevicePerlin::CDevicePerlin( CWorkspace* workspace, IImageProcess* proc )
: IEditorDevice( workspace, proc )
{
}

CDevicePerlin::~CDevicePerlin( void )
{
}

/// Create a GUI element to display in the editor
bool CDevicePerlin::createGUIItem( core::vector2d<s32> pos )
{
	if (!m_EditorControl)
		return false;

	return m_EditorControl->Create( "Gen", m_pImageProc->getNumInputs(), m_pImageProc->getNumOutputs(), m_pImageProc->getName(), pos );
}

/// Create a GUI window to edit the parameters of this device
gui::IGUIWindow* CDevicePerlin::createGUIParamWindow( void )
{
	// Get the ID
	u32 id = m_pImageProc->getID();

	gui::IGUIEnvironment* guienv = m_Workspace->getGUIEnvironment();

	// Save the previous settings for this device
	m_pImageProc->serializeAttributes(m_PrevSettings);

	// Create the modal window
	m_PropWindow = m_Workspace->getGUIEnvironment()->addWindow( R(400,400,750,650), true, L"Properties", 0, id + 10 );

	// Seed value
	guienv->addStaticText( L"Seed:", R(5,25,70,40), false, false, m_PropWindow, -1 );
	gui::IGUIEditBox* seed = guienv->addEditBox( L"", R(75,25,175,40), true, m_PropWindow, id + 11 );
	guienv->addButton( R(180,25,245,40), m_PropWindow, id + 12, L"Random", L"Generate a random seed." );
	// Scale sliders 0 to 1000 (divide result by 100)
	guienv->addStaticText( L"Scale X:", R(5,50,70,65), false, false, m_PropWindow, -1 );
	gui::IGUIScrollBar* Sx = guienv->addScrollBar( true, R(75,50,225,65), m_PropWindow, id + 13 );
	gui::IGUIStaticText* ScaleX = guienv->addStaticText( L"", R(230, 50, 295, 65), true, false, m_PropWindow, id + 14 );
	guienv->addStaticText( L"Scale Y:", R(5,70,70,85), false, false, m_PropWindow, -1 );
	gui::IGUIScrollBar* Sy = guienv->addScrollBar( true, R(75,70,225,85), m_PropWindow, id + 15 );
	gui::IGUIStaticText* ScaleY = guienv->addStaticText( L"", R(230, 70, 295, 85), true, false, m_PropWindow, id + 16 );
	guienv->addCheckBox( true, R(300,60,345,75), m_PropWindow, id + 17, L"Link");
	// Octaves
	guienv->addStaticText( L"Octaves:", R(5,95,70,110), false, false, m_PropWindow, -1 );
	gui::IGUIScrollBar* octaves = guienv->addScrollBar( true, R(75,95,225,110), m_PropWindow, id + 18 );
	gui::IGUIStaticText* o = guienv->addStaticText( L"", R(230,95,295,110), true, false, m_PropWindow, id + 19 );
	// Persistance
	guienv->addStaticText( L"Lacunarity:", R(5,120,70,135), false, false, m_PropWindow, -1 );
	gui::IGUIScrollBar* lac = guienv->addScrollBar( true, R(75,120,225,135), m_PropWindow, id + 20 );
	gui::IGUIStaticText* l = guienv->addStaticText( L"", R(230,120,295,135), true, false, m_PropWindow, id + 21 );
	// Gain
	guienv->addStaticText( L"Gain:", R(5,145,70,160), false, false, m_PropWindow, -1 );
	gui::IGUIScrollBar* gain = guienv->addScrollBar( true, R(75,145,225,160), m_PropWindow, id + 22 );
	gui::IGUIStaticText* g = guienv->addStaticText( L"", R(230,145,295,160), true, false, m_PropWindow, id + 23 );
	// Threshold
	guienv->addStaticText( L"Threshold:", R(5, 170, 70, 185), false, false, m_PropWindow, -1 );
	m_ThresholdScroll = guienv->addScrollBar( true, R(75,170,225,185), m_PropWindow, id + 25 );
	m_ThresholdText = guienv->addStaticText( L"", R(230,170,295,185), true, false, m_PropWindow, id + 26 );
	// Type
	guienv->addStaticText( L"Type:", R(5,195,70,210), false, false, m_PropWindow, -1 );
	gui::IGUIComboBox* type = guienv->addComboBox( R(75,195,225,210), m_PropWindow, id + 24 );

	//CColorDialog
	// Get the image process
	CImgPerlinNoise* proc = (CImgPerlinNoise*)m_pImageProc;

	// Set the seed value
	seed->setText( S( proc->getSeed() ) );
	seed->setMax( 9 );
	// Set the scale text values
	ScaleX->setText( S( proc->getScale().X ) );
	ScaleY->setText( S( proc->getScale().Y ) );
	// Octaves/Persistance/Gain
	o->setText( S( proc->getOctaves() ) );
	l->setText( S( proc->getLacunarity() ) );
	g->setText( S( proc->getGain() ) );
	// Threshold text
	m_ThresholdText->setText( S( proc->getThreshold() ) );

	// Setup the scroll bars
	Sx->setMax(8000);		Sy->setMax(8000);
	Sx->setMin(5);			Sy->setMin(5);
	Sx->setSmallStep( 1 );	Sy->setSmallStep( 1 );
	Sx->setLargeStep( 100 ); Sy->setLargeStep( 100 );
	Sx->setPos( (s32)(proc->getScale().X * 100) );
	Sy->setPos( (s32)(proc->getScale().Y * 100) );
	// Set the octaves scroll bar
	octaves->setMax(20); octaves->setMin(1);
	octaves->setSmallStep(1); octaves->setLargeStep(2);
	octaves->setPos( (s32)proc->getOctaves() );
	// Persistance scrollbar
	lac->setMax(10); lac->setMin(1);
	lac->setSmallStep(1); lac->setLargeStep(2);
	lac->setPos( (s32)proc->getLacunarity() );
	// Gain scrollbar
	gain->setMax(100); gain->setMin(1);
	gain->setSmallStep(1); gain->setLargeStep(10);
	gain->setPos( (s32)(proc->getGain() * 100) );
	// Threshold scrollbar
	m_ThresholdScroll->setMax(100);		m_ThresholdScroll->setMin(0);
	m_ThresholdScroll->setSmallStep(1);	m_ThresholdScroll->setLargeStep(10);
	m_ThresholdScroll->setPos( (s32)(proc->getThreshold() * 100 ) );

	// The type of Perlin noise
	type->addItem( L"Standard", PNT_STANDARD );
	type->addItem( L"Ridged", PNT_RIDGED );
	type->addItem( L"Billowy", PNT_BILLOWY );
	if (proc->getPType() == PNT_STANDARD)
		type->setSelected( 0 );
	if (proc->getPType() == PNT_RIDGED)
		type->setSelected( 1 );
	if (proc->getPType() == PNT_BILLOWY)
		type->setSelected( 2 );

	// Add the OK / Cancel buttons
	guienv->addButton(R(25,225,120,245), m_PropWindow, id + 38, L"OK" );
	guienv->addButton(R(130,225,225,245), m_PropWindow, id + 39, L"Cancel" );

	return m_PropWindow;
}

/// We have a GUI event while the properies window was open
/// Handle the event here if we can
/// \return True if we handled the event, otherwise false
bool CDevicePerlin::OnGUIEvent( const irr::SEvent &event )
{
	// Get the ID
	u32 id = m_pImageProc->getID();

	if (!m_PropWindow)
		return false;

	// Check the event type
	if ( event.EventType == EET_GUI_EVENT )
	{
		// Get the window controls
		// Seed
		gui::IGUIEditBox* seed = (gui::IGUIEditBox*)m_PropWindow->getElementFromId( id + 11, true );
		// ScaleX
		gui::IGUIScrollBar* Sx = (gui::IGUIScrollBar*)m_PropWindow->getElementFromId( id + 13, true );
		gui::IGUIStaticText* ScaleX = (gui::IGUIStaticText*)m_PropWindow->getElementFromId( id + 14, true );
		// ScaleY
		gui::IGUIScrollBar* Sy = (gui::IGUIScrollBar*)m_PropWindow->getElementFromId( id + 15, true );
		gui::IGUIStaticText* ScaleY = (gui::IGUIStaticText*)m_PropWindow->getElementFromId( id + 16, true );
		// Scale Link
		gui::IGUICheckBox* Slink = (gui::IGUICheckBox*)m_PropWindow->getElementFromId( id + 17, true );
		// Octaves
		gui::IGUIScrollBar* Octaves = (gui::IGUIScrollBar*)m_PropWindow->getElementFromId( id + 18, true );
		gui::IGUIStaticText* O = (gui::IGUIStaticText*)m_PropWindow->getElementFromId( id + 19, true );
		// Persistance
		gui::IGUIScrollBar* Lac = (gui::IGUIScrollBar*)m_PropWindow->getElementFromId( id + 20, true );
		gui::IGUIStaticText* La = (gui::IGUIStaticText*)m_PropWindow->getElementFromId( id + 21, true );
		// Gain
		gui::IGUIScrollBar* Gain = (gui::IGUIScrollBar*)m_PropWindow->getElementFromId( id + 22, true );
		gui::IGUIStaticText* G = (gui::IGUIStaticText*)m_PropWindow->getElementFromId( id + 23, true );

		// The perlin noise type
		gui::IGUIComboBox* Type = (gui::IGUIComboBox*)m_PropWindow->getElementFromId( id + 24, true );

		// Get the data
		// Get the seed value and convert to number
		core::stringc rs = core::stringc( seed->getText() );
		u32 seedval = (u32)atoi(rs.c_str());	// Seed
		seed->setText(S(seedval));
		// Get the scale
		f32 fscalex = (f32)(Sx->getPos()) / 100;
		f32 fscaley = (f32)(Sy->getPos()) / 100;
		// Get the octaves
		u32 oct = (u32)(Octaves->getPos());
		// Get the persistance
		f32 lac = (f32)(Lac->getPos());
		// Get the gain
		f32 gain = (f32)(Gain->getPos()) / 100;
		// Get the threshold
		f32 threshold = (f32)(m_ThresholdScroll->getPos()) / 100;

		// Get the process
		CImgPerlinNoise* proc = (CImgPerlinNoise*)m_pImageProc;	// Image process

		if ( event.GUIEvent.EventType == gui::EGET_SCROLL_BAR_CHANGED )
		{
			// A scroll bar was changed
			// Check the link value
			if (Slink->isChecked())
			{
				//The two sliders need to be linked
				if (event.GUIEvent.Caller->getID() == Sx->getID())
				{
					Sy->setPos( Sx->getPos() );
					fscaley = fscalex;
				}
				else if (event.GUIEvent.Caller->getID() == Sy->getID())
				{
					Sx->setPos( Sy->getPos() );
					fscalex = fscaley;
				}
			}
			// Refresh the text boxes
			ScaleX->setText( S( fscalex ) );
			ScaleY->setText( S( fscaley ) );
			O->setText( S( oct ) );
			La->setText( S( lac ) );
			G->setText( S( gain ) );
			m_ThresholdText->setText( S( threshold ) );

			// Save the data
			proc->setScale( fscalex, fscaley );
			proc->setOctaves( oct );
			proc->setLacunarity( lac );
			proc->setGain( gain );
			proc->setThreshold( threshold );

			// Update the preview window
			m_Workspace->showPreview( this );

			return true;
		}
		if ( event.GUIEvent.EventType == gui::EGET_CHECKBOX_CHANGED )
		{
			return true;
		}
		if ( event.GUIEvent.EventType == gui::EGET_EDITBOX_CHANGED )
		{
			// Edit box has changed
			if ( event.GUIEvent.Caller->getID() == (id + 11) )
			{
				// Save the seed
				proc->setSeed( seedval );
				// Update the preview window
				m_Workspace->showPreview( this );
			}
		}
		if ( event.GUIEvent.EventType == gui::EGET_COMBO_BOX_CHANGED )
		{
			if ( event.GUIEvent.Caller->getID() == (id + 24) )
			{
				// The perlin noise type box was changed
				PERLIN_NOISE_TYPE ptype = (PERLIN_NOISE_TYPE)Type->getItemData( Type->getSelected() );
				proc->setPType( ptype );
				// Update the preview window
				m_Workspace->showPreview( this );
			}
		}
		if ( event.GUIEvent.EventType == gui::EGET_BUTTON_CLICKED )
		{
			// One of the buttons was clicked
			if ( event.GUIEvent.Caller->getID() == (id + 12) )
			{
				// Random seed button
				// Generate a random number using the previous seed ORed with the time
				srand( ( seedval | (u32)time(NULL) ) );
				seedval = (u32)rand() % 0x7fffffff;
				// show in the edit box
				seed->setText( S(seedval) );

				// Save it to the process
				proc->setSeed( seedval );
				// Update the preview window
				m_Workspace->showPreview( this );

				return true;
			}
			if ( event.GUIEvent.Caller->getID() == (id + 38) )
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
			if ( event.GUIEvent.Caller->getID() == (id + 39) )
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

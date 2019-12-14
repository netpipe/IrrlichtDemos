#include "StdAfx.h"
#include "Workspace.h"
#include "Editor.h"
#include "DeviceColourize.h"
#include "DeviceColourize2.h"
#include "DeviceGradient.h"
#include "DevicePerlin.h"
#include "DeviceBlend.h"

/// Constructor
CWorkspace::CWorkspace( CEditor* Editor, IrrlichtDevice* Device, CTextureFactory* TexFactory )
: m_TextureFactory( TexFactory )
, m_Editor( Editor )
, m_SelectedDevice( NULL )
, m_Dragging( NULL )
{
	// Grab the irrlicht object
	if (Device)
	{
		// Get the video driver
		m_Driver = Device->getVideoDriver();
		m_Driver->grab();
		// File system
		m_FileSystem = Device->getFileSystem();
		m_FileSystem->grab();
		// GUI Environment
		m_GUIEnv = Device->getGUIEnvironment();
		m_GUIEnv->grab();
	}

	// Grab the texture factory
	if (m_TextureFactory)
		m_TextureFactory->grab();
}

CWorkspace::~CWorkspace(void)
{
	// Drop all the object references
	if (m_Driver)
		m_Driver->drop();
	if (m_FileSystem)
		m_FileSystem->drop();
	if (m_GUIEnv)
		m_GUIEnv->drop();

	if (m_TextureFactory)
		m_TextureFactory->drop();
}

/// Create an empty attributes item
io::IAttributes* CWorkspace::createAttributes( void ) const
{
	return m_FileSystem->createEmptyAttributes();
}

/// Create an editor control item
CGUIEditorControl* CWorkspace::createEditorControl( void ) const
{
	return new CGUIEditorControl( m_GUIEnv, m_Driver );
}

/// Return pointer to the texture factory
CTextureFactory* CWorkspace::getTextureFactory( void ) const
{
	return m_TextureFactory;
}

/// Return a pointer to the GUI environment
gui::IGUIEnvironment* CWorkspace::getGUIEnvironment( void ) const
{
	return m_GUIEnv;
}

void CWorkspace::addDevice( IEditorDevice* device, core::vector2d<s32> pos )
{
	// Add the device to the list
	m_Devices.push_back( device );

	// Create it's Editor Item
	if (!device->createGUIItem( pos ))
		LOG( "WARNING! Failed to create GUI Item." );

	// Add it to the device list and set the device's list index
	m_Editor->getToolWindow()->addListItem( device->getName() );
}

/// Get a device by ID
/// \param id The ID of the device to get
IEditorDevice* CWorkspace::getDevice( u32 id )
{
	for ( u32 i = 0; i < m_Devices.size(); i++ )
	{
		if ( m_Devices[i]->getID() == id )
			return m_Devices[i];
	}
	return NULL;
}

/// Remove a device
/// \param DeviceID The ID of the device to remove
/// \return True if succesfuly removed otherwise False
bool CWorkspace::removeDevice( u32 DeviceID )
{
	// Find the device
	IEditorDevice* dev = getDevice( DeviceID );
	if (!dev)
		return false;

	// Now remove it
	return removeDevice( dev );
}
/// Remove a device
/// \param Device The device to remove
/// \return True if succesfuly removed otherwise False
bool CWorkspace::removeDevice( IEditorDevice* Device )
{
	// Remove a device from the editor
	if (!Device)
		return false;

	// First remove any links we have for the device
	for (u32 i = 0; i < m_DeviceLinks.size(); i++)
	{
		if ( (m_DeviceLinks[i].Dest == Device) || (m_DeviceLinks[i].Source == Device) )
		{
			m_DeviceLinks.erase(i);
			i--;
		}
	}

	// Now remove it from the texture fatory
	m_TextureFactory->removeProcess( Device->getImgProcess() );

	// Now remove it from the list of devices
	for (u32 i = 0; i < m_Devices.size(); i++ )
	{
		if (m_Devices[i] == Device)
		{
			// Found the one we need to remove
			m_Devices[i]->drop();
			m_Devices.erase(i);
		}
	}

	return true;
}

/// Draw the links between devices
void CWorkspace::drawLinks( void )
{
	// Loop through all links between devices
	for (u32 i = 0; i < m_DeviceLinks.size(); i++)
	{
		// Get the two points
		if ((m_DeviceLinks[i].Source) && (m_DeviceLinks[i].Dest))
		{
			core::vector2d<s32> p1 = m_DeviceLinks[i].Source->getOutputLinkPoint( m_DeviceLinks[i].SourceIdx );
			core::vector2d<s32> p2 = m_DeviceLinks[i].Dest->getInputLinkPoint( m_DeviceLinks[i].DestIdx );

			// Create the points for the link lines
			core::vector2d<s32> p1a = p1;
			p1a.X += 15;
			core::vector2d<s32> p2a = p2;
			p2a.X -= 15;

			// Check for valid points
			if (p1 != p2)
			{
				m_Driver->draw2DLine( p1, p1a, video::SColor( 255, 0, 0, 0 ) );

				m_Driver->draw2DRectangle( video::SColor(255,0,0,0), core::rect<s32>( (p1a - 2), (p1a + 2) ) );
				m_Driver->draw2DRectangle( video::SColor(255,0,0,0), core::rect<s32>( (p2a - 2), (p2a + 2) ) );

				m_Driver->draw2DLine( p2, p2a, video::SColor( 255, 0, 0, 0 ) );
				m_Driver->draw2DLine( p1a, p2a, video::SColor(255,0,0,0) );
			}
		}
	}

	// Draw the current link we are building if we are in link mode
	if (m_EditorMode == EM_LINK_ELEMENT)
	{
		// Set both to the mouse position
		core::vector2d<s32> p1 = m_EditorMode.MousePos;
		core::vector2d<s32> p2 = m_EditorMode.MousePos;
		// Get the real start or destination for the link
		if (m_EditorMode.Link.Dest)
			p2 = m_EditorMode.Link.Dest->getInputLinkPoint( m_EditorMode.Link.DestIdx );
		if (m_EditorMode.Link.Source)
			p1 = m_EditorMode.Link.Source->getOutputLinkPoint( m_EditorMode.Link.SourceIdx );

		// Create the points for link lines
		core::vector2d<s32> p1a = p1;
		p1a.X += 15;
		core::vector2d<s32> p2a = p2;
		p2a.X -= 15;

		// Check for valid points
		if (p1 != p2)
		{
			m_Driver->draw2DLine( p1, p1a, video::SColor( 255, 0, 0, 0 ) );

			m_Driver->draw2DRectangle( video::SColor(255,0,0,0), core::rect<s32>( (p1a - 2), (p1a + 2) ) );
			m_Driver->draw2DRectangle( video::SColor(255,0,0,0), core::rect<s32>( (p2a - 2), (p2a + 2) ) );

			m_Driver->draw2DLine( p2, p2a, video::SColor( 255, 0, 0, 0 ) );
			m_Driver->draw2DLine( p1a, p2a, video::SColor(255,0,0,0) );
		}

	}

}

void CWorkspace::showPreview( IEditorDevice* device )
{
	// Get the image
	video::IImage* imgout = m_TextureFactory->getOutputImage( device->getImgProcess(), 0, PREVIEW_WIDTH, PREVIEW_HEIGHT );
	// Remove the old texture if it exists
	m_Driver->removeTexture( m_Driver->findTexture( "Preview" ) );
	// Create a new one from the image
	video::ITexture* t = m_Driver->addTexture( "Preview", imgout );
	// Show the output image in the preview window
	m_Editor->getToolWindow()->setPreviewImage( t );
}

/// Add a device mode
void CWorkspace::setAddDeviceMode( TexGen::PROCESS_TYPES DeviceType )
{
	if (DeviceType == TexGen::PT_NONE)
	{
		m_EditorMode.Mode = EM_NORMAL;
		m_EditorMode.PType = DeviceType;
	}
	else
	{
		m_EditorMode.Mode = EM_ADD_ELEMENT;
		m_EditorMode.PType = DeviceType;
	}
}

/// Save a large scale preview image
void CWorkspace::savePreviewImage( void )
{
	if (!m_SelectedDevice)
		return;

	video::IImage* img = m_TextureFactory->getOutputImage( m_SelectedDevice->getImgProcess(), 0, EXPORT_PREVIEW_WIDTH, EXPORT_PREVIEW_HEIGHT );
	m_Driver->writeImageToFile( img, "export/preview.png", 0 );

}

/// Create a new editor device
IEditorDevice* CWorkspace::createDevice( PROCESS_TYPES type, u32 ID )
{
	IEditorDevice* dev;
	IImageProcess* proc;

	// First create a new Perlin Noise image process
	proc = m_TextureFactory->addProcess( type, ID );
	if (!proc)
		return NULL;

	// Then create the editor device to wrap around it
	switch (type)
	{
	case PT_PERLIN_NOISE:
		// Create a new editor device for this process
		dev = new CDevicePerlin( this, proc );
		break;
	case PT_GRADIENT:
		// Create a new gradient device
		dev = new CDeviceGradient( this, proc );
		break;
	case PT_COLOURIZE:
		// Create a new colourize device
		dev = new CDeviceColourize( this, proc );
		break;
	case PT_COLOURIZE_ADVANCED:
		// Create a new colourize device
		dev = new CDeviceColourize2( this, proc );
		break;
	case PT_BLEND:
		// Create a new blend device
		dev = new CDeviceBlend( this, proc );
		break;
	default:
		break;
	};

	// return the device
	return dev;
}

/// Clear the workspace completly
void CWorkspace::clearWorkspace( void )
{
	// Drop the devices
	for ( u32 i = 0; i < m_Devices.size(); i++ )
		m_Devices[i]->drop();
	// Clear the array
	m_Devices.clear();

	// Clear the list
	//list->clear();

	// Clear the device links
	m_DeviceLinks.clear();

	// Clear the devices from the texture factory
	m_TextureFactory->clear();
}

void CWorkspace::addLink( u32 SourceDevID, u32 SourceDevIdx, u32 DestDevID, u32 DestDevIdx )
{
	// Loop through all the devices getting the ID
	IEditorDevice* SourceDev = NULL;
	IEditorDevice* DestDev = NULL;

	// Check the number of devices we have, if none then we can't add a link
	if (m_Devices.size() < 2)
		return;

	for ( u32 i = 0; i < m_Devices.size(); i++ )
	{
		if (m_Devices[i]->getID() == SourceDevID)
			SourceDev = m_Devices[i];
		if (m_Devices[i]->getID() == DestDevID)
			DestDev = m_Devices[i];
	}

	// Add the link
	addLink( SourceDev, SourceDevIdx, DestDev, DestDevIdx );

}
void CWorkspace::addLink( IEditorDevice* SourceDev, u32 SourceDevIdx, IEditorDevice* DestDev, u32 DestDevIdx )
{
	// Check
	if (!SourceDev)
		return;
	if (!DestDev)
		return;

	// Add the link to the texture factory
	if (!m_TextureFactory->addLink( SourceDev->getID(), SourceDevIdx, DestDev->getID(), DestDevIdx ))
	{
		// Failed to add texture factory link... something is wrong
		LOG("Failed to create Texture Factory link!");
		return;
	}

	// Everything OK so far, continue to add the device links
	// Check for duplicate links and remove multiple inputs
	for (u32 i = 0; i < m_DeviceLinks.size(); i++ )
	{
		if ( (m_DeviceLinks[i].Dest == DestDev) && (m_DeviceLinks[i].DestIdx == DestDevIdx) )
		{
			// Multiple inputs not allowed
			// remove the link
			m_DeviceLinks.erase( i );
		}
	}

	SDeviceLink Dev;

	Dev.Source = SourceDev;
	Dev.SourceIdx = SourceDevIdx;
	Dev.Dest = DestDev;
	Dev.DestIdx = DestDevIdx;

	// Add to the array
	m_DeviceLinks.push_back( Dev );

}

void CWorkspace::addLink( SDeviceLink* Link )
{
	// Check
	if (!Link)
		return;
	// add the link
	addLink( Link->Source, Link->SourceIdx, Link->Dest, Link->DestIdx );
}

/// The workspace has been resized
bool CWorkspace::OnResize( core::rect<s32> &rect )
{
	// Save the new rect
	m_WorkspaceRect = rect;

	return true;
}

/// Mouse button has been pressed in the workspace
bool CWorkspace::OnLMouseDown( const core::vector2d<s32> &Pos )
{

	m_Dragging = NULL;	// Should be NULL anyway

	// If we are in normal editor mode
	if (m_EditorMode == EM_NORMAL)
	{
		// Loop through all the devices and find if the mouse is within the device's control rect
		for (u32 i = 0; i < m_Devices.size(); i++ )
		{
			if (m_Devices[i]->getEditorControl()->isPointInside( Pos ) )
			{
				// Point is inside, we have selected the item
				m_Dragging = m_Devices[i];
				return true;
			}
		}
	}

	return false;
}

bool CWorkspace::OnLMouseUp( const core::vector2d<s32> &Pos )
{

	// Mouse released, clear dragging
	m_Dragging = NULL;

	// If we are in normal editor mode
	if (m_EditorMode == EM_NORMAL)
	{
		// Set the selected item to NULL
		m_SelectedDevice = NULL;

		// Loop through all the devices and find if the mouse is within the device's control rect
		for (u32 i = 0; i < m_Devices.size(); i++ )
		{
			if (m_Devices[i]->getEditorControl()->isPointInside( Pos ) )
			{
				// Point is inside, we have selected the item
				m_SelectedDevice = m_Devices[i];

				// Get the preview image
				showPreview( m_SelectedDevice );

				// Update the property window
				io::IAttributes* attr = createAttributes();
				m_SelectedDevice->serializeAttributes( attr );
				m_Editor->updatePropertyWindow( attr );
				attr->drop();

				// Check to see if we clicked on a link node
				if (m_SelectedDevice->getEditorControl()->getInputIndex( Pos ) > -1)
				{
					// We have clicked on a valid input index
					m_EditorMode.Mode = EM_LINK_ELEMENT;
					m_EditorMode.Link.Dest = m_SelectedDevice;
					m_EditorMode.Link.DestIdx = m_SelectedDevice->getEditorControl()->getInputIndex( Pos );
					m_EditorMode.MousePos = Pos;
				}
				if (m_SelectedDevice->getEditorControl()->getOutputIndex( Pos ) > -1)
				{
					// We have clicked on a valid output index
					m_EditorMode.Mode = EM_LINK_ELEMENT;
					m_EditorMode.Link.Source = m_SelectedDevice;
					m_EditorMode.Link.SourceIdx = m_SelectedDevice->getEditorControl()->getOutputIndex( Pos );
					m_EditorMode.MousePos = Pos;
				}

				return true;
			}
		}
	}

	// If we are in link mode
	if (m_EditorMode == EM_LINK_ELEMENT)
	{
		// Loop through all devices until we find the one we are over
		IEditorDevice* dev = NULL;
		u32 i = 0;
		while ( ( i < m_Devices.size() ) && (!dev) )
		{
			if (m_Devices[i]->getEditorControl()->isPointInside( Pos ) )
			{
				dev = m_Devices[i];
			}
			i++;
		}
		// Check
		if (!dev)
		{
			// Not a valid device, didn't click on one?
			m_EditorMode.reset();
			return true;
		}

		// Check this device and other device arn't the same
		if ( (dev == m_EditorMode.Link.Dest) || (dev == m_EditorMode.Link.Source) )
		{
			// Save device, can't do that
			m_EditorMode.reset();
			return true;
		}

		// Not the same
		// Check what we need
		if (m_EditorMode.Link.Dest)
		{
			// We need the source
			// Check for a valid source index
			if (dev->getEditorControl()->getOutputIndex( Pos ) == -1)
			{
				// Invalid index, didn't click on one
				m_EditorMode.reset();
				return true;
			}
			// Valid index
			m_EditorMode.Link.Source = dev;
			m_EditorMode.Link.SourceIdx = (u32)dev->getEditorControl()->getOutputIndex( Pos );
		}
		else if (m_EditorMode.Link.Source)
		{
			// We need an input index
			if (dev->getEditorControl()->getInputIndex( Pos ) == -1)
			{
				m_EditorMode.reset();
				return true;
			}
			// Valid index
			m_EditorMode.Link.Dest = dev;
			m_EditorMode.Link.DestIdx = (u32)dev->getEditorControl()->getInputIndex( Pos );
		}

		// We now have all the data availble to create a new link
		addLink( &m_EditorMode.Link );

		// Reset the mode
		m_EditorMode.reset();
		return true;
	}

	// If we are in add a new element mode...
	if (m_EditorMode == EM_ADD_ELEMENT)
	{
		// Add a new device at the mouse coords of the selected type
		IEditorDevice* dev = createDevice( m_EditorMode.PType );
		addDevice( dev, Pos );

		// Set back to editing mode
		m_EditorMode.reset();
		return true;
	}

	return false;

}

bool CWorkspace::OnLMouseDblClick( const core::vector2d<s32> &Pos )
{

	// Check to see if we are in normal editor mode
	if (m_EditorMode != EM_NORMAL)
		return false;

	// check for a selected item
	if (m_SelectedDevice)
	{
		// Check to see if the mouse is within the selected items rect
		if (m_SelectedDevice->getEditorControl()->isPointInside( Pos ) )
		{
			// Show the device's parameter window
			m_SelectedDevice->createGUIParamWindow( );

			// Set mode to edit mode
			m_EditorMode = EM_EDIT_ELEMENT;
		}
	}

	return true;
}

bool CWorkspace::OnMouseMove( const core::vector2d<s32> & Pos, bool LButtonDown, bool RButtonDown )
{
	// Mouse has been moved within the workspace
	static core::vector2d<s32> lastPos;

	if (m_EditorMode == EM_LINK_ELEMENT)
	{
		m_EditorMode.MousePos = Pos;
		return false;
	}

	// Check to see if we are in normal editor mode
	if (m_EditorMode != EM_NORMAL)
		return false;

	// Check to see if we are dragging a device's control around
	if (m_Dragging)
	{
		// Get the device's rect
		core::rect<s32> DRect = m_Dragging->getEditorControl()->getRect();

		// Move it based upon mouse coords
		DRect.operator+= ( Pos - lastPos );
		// Constrain the rect to the workspace rect
		DRect.constrainTo( m_WorkspaceRect );

		// And move the control to this new point
		m_Dragging->getEditorControl()->moveTo( DRect.UpperLeftCorner );
	}

	// Save the mouse position
	lastPos = Pos;

	return true;

}

/// A GUI event has occoured
/// \param event The event
/// \return True if we've handeled the event, false otherwise
bool CWorkspace::OnGUIEvent( const SEvent &event )
{
	// Check the mode
	if (m_EditorMode == EM_EDIT_ELEMENT)
	{
		// We have a device's paramter window open
		// Pass the event onto the selected device's event function
		return m_SelectedDevice->OnGUIEvent( event );
	}

	return false;
}

/// A device's property window has closed
bool CWorkspace::OnDevicePropWindowClose( IEditorDevice* Device )
{
	// check
	if (!Device)
		return false;

	// Update the preview
	showPreview( Device );

	// Set the editor mode back
	m_EditorMode = EM_NORMAL;

	return true;
}

/// User pressed the delete key - delete the selected element
bool CWorkspace::OnDelKeyPressed( void )
{

	// Make sure we don't have a proterty window open
	if (m_EditorMode != EM_EDIT_ELEMENT)
	{
		// Remove the seleced device
		if (!m_SelectedDevice)
			return false;

		// Reset the mode to edit mode
		m_EditorMode.reset();

		// Remove the selected device
		removeDevice( m_SelectedDevice );
		return true;
	}

	return false;
}

/// Check to see if a point is within the workspace rect
bool CWorkspace::isPointInside( const core::vector2d<s32> &Pos )
{
	return m_WorkspaceRect.isPointInside( Pos );
}

/// Save the workspace to a file
/// \param filename The name of the file to save to
bool CWorkspace::saveWorkspace( const io::path& filename )
{
	// Check
	if (!m_FileSystem)
		return false;

	// Create the file
	io::IWriteFile*	file = m_FileSystem->createAndWriteFile( filename, false );
	// Check
	if (!file)
		return false;

	bool ret = saveWorkspace( file );

	// Drop the file
	file->drop();

	return ret;
}

/// Save the workspace to a file
/// \param file The file to write to
bool CWorkspace::saveWorkspace( io::IWriteFile* file )
{
	// Check
	if ( (!m_FileSystem) || (!file) )
		return false;

	// Create the XML writer
	io::IXMLWriter* writer = m_FileSystem->createXMLWriter( file );
	// Check
	if (!writer)
		return false;

	// Write out the XML header
	writer->writeXMLHeader( );
	writer->writeLineBreak();
	writer->writeLineBreak();

	// Write the workspace element
	writer->writeElement( L"Workspace", false );
	writer->writeLineBreak();

	// Create this workspace's attributes
	io::IAttributes* ws_attr = m_FileSystem->createEmptyAttributes( );
	serializeAttributes( ws_attr );
	// Save them
	ws_attr->write( writer );

	//----------------------------------------------------------
	writer->writeLineBreak();
	// Write the devices element
	writer->writeElement( L"Devices", false );
	writer->writeLineBreak();

	// Now we loop through all the devices
	for (u32 i = 0; i < m_Devices.size(); i++ )
	{
		writer->writeElement( L"Device", false, L"ID", S(m_Devices[i]->getID()), L"TYPE", S(m_Devices[i]->getType()) );
		writer->writeLineBreak();

		// Create an attributes item
		io::IAttributes* attr = m_FileSystem->createEmptyAttributes( );
		// Get the processes attributes
		m_Devices[i]->serializeAttributes( attr );
		// Save them
		attr->write( writer );

		// Drop the attributes
		attr->drop();

		writer->writeLineBreak();

		writer->writeClosingTag( L"Device" );
		writer->writeLineBreak();
		writer->writeLineBreak();
	}

	// Close the Processes elemt
	writer->writeClosingTag( L"Devices" );
	writer->writeLineBreak();

	// Write the links element
	writer->writeElement( L"DeviceLinks", false );
	writer->writeLineBreak();

	// Now write out the links between the processes
	for (u32 i = 0; i < m_DeviceLinks.size(); i++ )
	{
		writer->writeElement( L"DeviceLink", false );
		writer->writeLineBreak();
		writer->writeLineBreak();

		// Create an attribute item
		io::IAttributes* attr = m_FileSystem->createEmptyAttributes( );
		// Add the link items
		attr->addInt( "SourceDevice", m_DeviceLinks[i].Source->getID());
		attr->addInt( "SourceIdx", m_DeviceLinks[i].SourceIdx );
		attr->addInt( "DestDevice", m_DeviceLinks[i].Dest->getID());
		attr->addInt( "DestIdx", m_DeviceLinks[i].DestIdx );
		// Save them
		attr->write( writer );
		// Drop the attributes
		attr->drop();

		writer->writeLineBreak();
		writer->writeClosingTag( L"DeviceLink" );
		writer->writeLineBreak();
		writer->writeLineBreak();
	}

	// Close the links element
	writer->writeClosingTag( L"DeviceLinks" );
	writer->writeLineBreak();

	//----------------------------------------------------------
	writer->writeClosingTag( L"Workspace" );
	writer->writeLineBreak();

	writer->drop();

	return true;
}

/// Load the workspace from a file
/// \param filename The name of the file to load
bool CWorkspace::loadWorkspace( const io::path& filename )
{
	// Check
	if (!m_FileSystem)
		return false;

	// Create the read file
	io::IReadFile* read = m_FileSystem->createAndOpenFile( filename );
	// Check
	if (!read)
		return false;

	// Read the file
	bool ret = loadWorkspace( read );

	// Drop the file
	read->drop();

	return ret;
}

/// Load the workspace from a file
/// \param file The file to load from
bool CWorkspace::loadWorkspace( io::IReadFile* file )
{
	// Check
	if ( (!m_FileSystem) || (!file) )
		return false;

	// Create the XML reader
	io::IXMLReader* reader = m_FileSystem->createXMLReader( file );
	// Check
	if (!reader)
		return false;

	// Clear the current workspace if we have any
	clearWorkspace();

	// Loop through all processes and read them in
	while (reader->read())
	{
		if (!wcscmp( L"Workspace", reader->getNodeName()))
		{
			// We have the workspace node
			while (reader->read())
			{
				bool endreached = false;
				// Check the element type
				switch (reader->getNodeType())
				{
				case io::EXN_ELEMENT_END:
					endreached = true;
					break;
				case io::EXN_ELEMENT:
					// Check the element name
					if (!wcscmp( L"attributes", reader->getNodeName() ) )
					{
						// read the attributes
						io::IAttributes* attr = m_FileSystem->createEmptyAttributes();
						attr->read( reader, true );
						// deserialize the attributes
						deserializeAttributes( attr );
						// drop the attributes
						attr->drop();
					}
					else if (!wcscmp( L"Devices", reader->getNodeName()))
					{
						// Read the devices
						readDevices( reader );
					}
					else if (!wcscmp( L"DeviceLinks", reader->getNodeName()))
					{
						// Read the links
						readLinks( reader );
					}
					else
					{
						LOG("Unknown element found in XML file.");
					}
					break;
				default:
					break;
				}

				if (endreached)
					break;
			}
		}
	}

	reader->drop();

	return true;
}

/// Read in the list of devices from the file
void CWorkspace::readDevices( io::IXMLReader* reader )
{
	// Check
	if (!reader)
		return;

	// Read the devices
	while (reader->read())
	{
		bool endreached = false;

		switch (reader->getNodeType())
		{
		case io::EXN_ELEMENT_END:
			endreached = true;
			break;
		case io::EXN_ELEMENT:
			// Check for element type
			if (!wcscmp( L"Device", reader->getNodeName() ) )
			{
				// Read the process
				readDevice( reader );
			}
			else
			{
				// Found an unknown element
				std::cout << "Unknown element found in file." << std::endl;
			}
			break;
		default:
			break;
		}

		if (endreached)
			break;
	}
}

/// Read a device in from a file
void CWorkspace::readDevice( io::IXMLReader* reader )
{
	// Check
	if (!reader)
		return;

	// Get the type of node
	io::EXML_NODE nodeType = reader->getNodeType();

	if ((nodeType == io::EXN_NONE) || (nodeType == io::EXN_UNKNOWN) || (nodeType == io::EXN_ELEMENT_END))
		return;

	if (!wcscmp( L"Device", reader->getNodeName()))
	{
		// Find the process type
		PROCESS_TYPES ptype = (PROCESS_TYPES)reader->getAttributeValueAsInt( L"TYPE" );
		u32 ID = reader->getAttributeValueAsInt( L"ID" );

		// Create the device
		IEditorDevice* dev = createDevice( ptype, ID );
		// Add it to the list
		addDevice( dev, core::vector2d<s32>( 0, 0 ) );

		// read the attributes
		while (reader->read())
		{
			bool endreached = false;

			switch (reader->getNodeType())
			{
			case io::EXN_ELEMENT_END:
				endreached = true;
				break;
			case io::EXN_ELEMENT:
				// Check for attributes
				if (!wcscmp( L"attributes", reader->getNodeName() ) )
				{
					// Read the attributes
					io::IAttributes* attr = m_FileSystem->createEmptyAttributes( );
					attr->read( reader, true );

					if (dev)
						dev->deserializeAttributes( attr );

					attr->drop();
				}
				else
				{
					// Found an unknown element
				}
				break;
			default:
				break;
			}

			if (endreached)
				break;
		}

	}
}

/// Read the device links in from a file
void CWorkspace::readLinks( io::IXMLReader* reader )
{
	// Check
	if (!reader)
		return;

	// Read the links
	while (reader->read())
	{
		bool endreached = false;

		switch (reader->getNodeType())
		{
		case io::EXN_ELEMENT_END:
			endreached = true;
			break;
		case io::EXN_ELEMENT:
			// Check for element type
			if (!wcscmp( L"DeviceLink", reader->getNodeName() ) )
			{
				// read the link
				readLink( reader );
			}
			else
			{
				// Found an unknown element
				std::cout << "Unknown element found in file." << std::endl;
			}
			break;
		default:
			break;
		}

		if (endreached)
			break;
	}
}

void CWorkspace::readLink( io::IXMLReader* reader )
{
	// Check
	if (!reader)
		return;

	// Get the type of node
	io::EXML_NODE nodeType = reader->getNodeType();

	if ((nodeType == io::EXN_NONE) || (nodeType == io::EXN_UNKNOWN) || (nodeType == io::EXN_ELEMENT_END))
		return;

	if (!wcscmp( L"DeviceLink", reader->getNodeName()))
	{

		// read the attributes
		while (reader->read())
		{
			bool endreached = false;

			switch (reader->getNodeType())
			{
			case io::EXN_ELEMENT_END:
				endreached = true;
				break;
			case io::EXN_ELEMENT:
				// Check for attributes
				if (!wcscmp( L"attributes", reader->getNodeName() ) )
				{
					// Read the attributes
					io::IAttributes* attr = m_FileSystem->createEmptyAttributes( );
					attr->read( reader, true );

					// Get the attribute info
					u32 SourceDevID = attr->getAttributeAsInt( "SourceDevice" );
					u32 SourceDevIdx = attr->getAttributeAsInt( "SourceIdx" );
					u32 DestDevID = attr->getAttributeAsInt( "DestDevice" );
					u32 DestDevIdx = attr->getAttributeAsInt( "DestIdx" );

					// Add the link
					addLink( SourceDevID, SourceDevIdx, DestDevID, DestDevIdx );

					// Drop the attributes
					attr->drop();
				}
				else
				{
					// Found an unknown element
				}
				break;
			default:
				break;
			}

			if (endreached)
				break;
		}

	}
}

void CWorkspace::serializeAttributes( io::IAttributes *out, io::SAttributeReadWriteOptions *options) const
{

}

void CWorkspace::deserializeAttributes( io::IAttributes *in, io::SAttributeReadWriteOptions *options)
{

}

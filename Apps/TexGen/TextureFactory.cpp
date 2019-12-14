#include "StdAfx.h"
#include "TextureFactory.h"

namespace TexGen
{

CTextureFactory::CTextureFactory( video::IVideoDriver* driver, io::IFileSystem* fs )
: m_Driver( driver )
, m_FileSystem( fs )
, m_NextProcID( 600 )
{
	if (m_Driver)
		m_Driver->grab();

	if (m_FileSystem)
		m_FileSystem->grab();
}

CTextureFactory::~CTextureFactory(void)
{
	// Cleanup
	clear();

	if (m_Driver)
		m_Driver->drop();
	if (m_FileSystem)
		m_FileSystem->drop();
}

/// Create a new image process.
/// \param type The type of image process to create
/// \return A pointer to the new image process
IImageProcess* CTextureFactory::addProcess( PROCESS_TYPES type, u32 id )
{
	IImageProcess* proc;

	// Check the id
	u32 pid = id;
	if (!pid)
		pid = m_NextProcID;

	// Check the type
	switch (type)
	{
	case PT_PERLIN_NOISE:
		// Create a new perlin noise type
		proc = new CImgPerlinNoise( m_Driver, pid );
		break;
	case PT_GRADIENT:
		// Create a new gradient type
		proc = new CImgGradient( m_Driver, pid );
		break;
	case PT_COLOURIZE:
		// Create a new colourize type
		proc = new CImgColourize( m_Driver, pid );
		break;
	case PT_COLOURIZE_ADVANCED:
		// Create the advanced colourize process
		proc = new CImgColourize2( m_Driver, pid );
		break;
	case PT_BLEND:
		// Create the blend mode process
		proc = new CImgBlend( m_Driver, pid );
		break;
	default:
		return NULL;
	}

	// Add it to the array
	m_Procs.push_back( proc );

	// Check to get the highest ID
	if (m_NextProcID <= pid)
		m_NextProcID = pid + 50;

	// return the process
	return proc;

}

bool CTextureFactory::addLink( u32 SourceProcID, u32 SourceIdx, u32 DestProcID, u32 DestIdx )
{
	// Get the image processes
	IImageProcess* SourceProc = getProcessByID( SourceProcID );
	if (!SourceProc)
		return false;
	IImageProcess* DestProc = getProcessByID( DestProcID );
	if (!DestProc)
		return false;

	// Add the links
	return addLink( SourceProc, SourceIdx, DestProc, DestIdx );
}

/// Add a link between two processes
bool CTextureFactory::addLink( IImageProcess *SourceProc, irr::u32 SourceIdx, IImageProcess *DestProc, irr::u32 DestIdx )
{
	// check
	if (!SourceProc)
		return false;
	if (!DestProc)
		return false;

	// Check for source and Dest the same
	if (SourceProc == DestProc)
		return false;

	// Check the input and output indexes
	if (SourceProc->getNumOutputs() <= SourceIdx)
		return false;
	if (DestProc->getNumInputs() <= DestIdx)
		return false;

	// Check for duplicate links and remove multiple inputs
	for (u32 i = 0; i < m_Links.size(); i++ )
	{
		if ( ( m_Links[i]->SourceProc == SourceProc ) && ( m_Links[i]->DestProc == DestProc) )
		{
			// Might be a duplicate link, check the inputs are different
			if ( ( m_Links[i]->SourceIdx == SourceIdx) && ( m_Links[i]->DestIdx == DestIdx ) )
			{
				// This is a duplicate link, fail
				return false;
			}
		}
		if ( (m_Links[i]->DestProc == DestProc) && (m_Links[i]->DestIdx == DestIdx) )
		{
			// Multiple inputs not allowed
			// remove the link
			removeLink( i );
		}
	}

	// Create the link
	SLink* link = new SLink;
	link->SourceProc = SourceProc;
	link->SourceIdx = SourceIdx;
	link->DestProc = DestProc;
	link->DestIdx = DestIdx;
	// Add to the list
	m_Links.push_back( link );

	// Do the mapping
	DestProc->setInput( DestIdx, SourceProc, SourceIdx );

	return true;
}

/// Remove a link between two devices
void CTextureFactory::removeLink( u32 LinkIdx )
{
	if (LinkIdx < m_Links.size() )
	{
		// We need to tell the destination process that this input is no longer valid
		if (m_Links[LinkIdx]->DestProc)
			m_Links[LinkIdx]->DestProc->removeInput( m_Links[LinkIdx]->DestIdx );
		
		delete m_Links[LinkIdx];
		m_Links.erase( LinkIdx );
	}
}

/// Get the number of processes in the factory
u32 CTextureFactory::getNumProcesses( void )
{
	return m_Procs.size();
}

/// Get a processes based upon the index
IImageProcess* CTextureFactory::getProcess( u32 index )
{
	if (m_Procs.size() >= index)
		return m_Procs[index];

	return NULL;
}
/// Get a processes based upon it's ID
IImageProcess* CTextureFactory::getProcessByID( u32 ID )
{
	// Loop through the list and remove the matching pointer
	for ( u32 i = 0; i < m_Procs.size(); i++ )
	{
		if (m_Procs[i]->getID() == ID)
		{
			return m_Procs[i];
		}
	}
	return NULL;
}

/// Get an output image from an image process
/// \param procID The Id of the process to get the image from
/// \param width The width of the final image
/// \param height The height of the final image
/// \return A pointer to an IImage object containing the final image
video::IImage* CTextureFactory::getOutputImage(irr::u32 procID, u32 outputIdx, u32 width, u32 height)
{
	// Get the process from the ID
	IImageProcess* proc = getProcessByID( procID );
	if (!proc)
		return NULL;

	return getOutputImage( proc, outputIdx, width, height );
}
/// Get an output image from an image process
/// \param procID The Id of the process to get the image from
/// \param width The width of the final image
/// \param height The height of the final image
/// \return A pointer to an IImage object containing the final image
video::IImage* CTextureFactory::getOutputImage( IImageProcess* Process, u32 outputIdx, u32 width, u32 height )
{
	// Check for a valid output index
	if (outputIdx < Process->getNumOutputs())
	{
		
		// Set the output size for the image
		Process->setOutputSize( width, height );

		// Refresh the process
		Process->Refresh();
		
		return Process->getOutputImage( outputIdx );
	}

	return NULL;
}

/// Create a texture from the process
video::ITexture* CTextureFactory::addTexture( const io::path& name, u32 ProcessID,  u32 outputIdx,  u32 width,  u32 height )
{
	video::IImage* img = getOutputImage( ProcessID, outputIdx, width, height );
	return m_Driver->addTexture( name, img );
}

/// Remove a process from the list
void CTextureFactory::removeProcess( IImageProcess *proc )
{
	// Loop through the list and remove the matching pointer

	// First find any links for this device
	for ( u32 i = 0; i < m_Links.size(); i++ )
	{
		if ( (m_Links[i]->DestProc == proc) || (m_Links[i]->SourceProc == proc ) )
		{
			// Remove the link
			removeLink( i );
			i--;
		}
	}
	// Now remove the device
	for ( u32 i = 0; i < m_Procs.size(); i++ )
	{
		if (m_Procs[i] == proc)
		{
			m_Procs[i]->drop();
			m_Procs.erase( i );
			return;
		}
	}
}
void CTextureFactory::removeProcess( u32 index )
{
	m_Procs[index]->drop();
	m_Procs.erase(index);
}

/// Remove a process from the list based upon it's ID number
void CTextureFactory::removeProcessByID( u32 ID )
{
	// Loop through the list and remove the matching pointer
	for ( u32 i = 0; i < m_Procs.size(); i++ )
	{
		if (m_Procs[i]->getID() == ID)
		{
			m_Procs[i]->drop();
			m_Procs.erase( i );
			return;
		}
	}
}

// Clear all elements in the factory
void CTextureFactory::clear( void )
{
	// Loop through all image processes and drop them
	for ( u32 i = 0; i < m_Procs.size(); i++ )
	{
		m_Procs[i]->drop();
	}
	m_Procs.clear();

	// Clear the device links
	for (u32 i = 0; i < m_Links.size(); i++ )
	{
		delete m_Links[i];
	}
	m_Links.clear();

	// Reset the highest ID value
	m_NextProcID = 600;
}

/// Write the proceses out to a file
/// \param filename The name of the file to save to
bool CTextureFactory::saveProcesses( const io::path &filename )
{
	// Create the file
	io::IWriteFile*	file = m_FileSystem->createAndWriteFile( filename, false );

	// Check
	if (!file)
		return false;

	bool ret = saveProcesses( file );

	// Drop the file
	file->drop();

	return ret;

}

/// Save the proceses out to a file object
/// \param The file object to save to
/// \return True of the save worked otherwise false
bool CTextureFactory::saveProcesses( io::IWriteFile *file )
{
	// Check the file
	if (!file)
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

	// Write the processes element
	writer->writeElement( L"Processes", false );
	writer->writeLineBreak();

	// Now we loop through all the image processes
	for (u32 i = 0; i < m_Procs.size(); i++ )
	{
		writer->writeElement( L"Process", false, L"ID", S(m_Procs[i]->getID()), L"TYPE", S(m_Procs[i]->getType()) );
		writer->writeLineBreak();
		
		// Create an attributes item
		io::IAttributes* attr = m_FileSystem->createEmptyAttributes( );
		// Get the processes attributes
		m_Procs[i]->serializeAttributes( attr );
		// Save them
		attr->write( writer );

		// Drop the attributes
		attr->drop();
		
		writer->writeLineBreak();

		writer->writeClosingTag( L"Process" );
		writer->writeLineBreak();
		writer->writeLineBreak();
	}

	// Close the Processes elemt
	writer->writeClosingTag( L"Processes" );
	writer->writeLineBreak();

	// Write the links element
	writer->writeElement( L"Links", false );
	writer->writeLineBreak();

	// Now write out the links between the processes
	for (u32 i = 0; i < m_Links.size(); i++ )
	{
		writer->writeElement( L"Link", false );
		writer->writeLineBreak();
		writer->writeLineBreak();

		// Create an attribute item
		io::IAttributes* attr = m_FileSystem->createEmptyAttributes( );
		// Add the link items
		attr->addInt( "SourceProc", m_Links[i]->SourceProc->getID());
		attr->addInt( "SourceIdx", m_Links[i]->SourceIdx );
		attr->addInt( "DestProc", m_Links[i]->DestProc->getID());
		attr->addInt( "DestIdx", m_Links[i]->DestIdx );
		// Save them
		attr->write( writer );
		// Drop the attributes
		attr->drop();

		writer->writeLineBreak();
		writer->writeClosingTag( L"Link" );
		writer->writeLineBreak();
		writer->writeLineBreak();
	}

	// Close the links element
	writer->writeClosingTag( L"Links" );
	writer->writeLineBreak();

	writer->drop();

	return true;
}

/// Load processes from a file
/// \param filename The name of the file to load from
/// \return True if the load was succesful, otherwise false
bool CTextureFactory::loadProcesses( const  io::path &filename )
{
	// Create the read file
	io::IReadFile* read = m_FileSystem->createAndOpenFile( filename );
	// Check
	if (!read)
		return false;

	// Read the file
	bool ret = loadProcesses( read );

	// Drop the file
	read->drop();

	return ret;
}
/// Load the processes from a file object
/// \param file The file to read from.
/// \return True if the load was succesful, otherwise false
bool CTextureFactory::loadProcesses( io::IReadFile *file )
{
	// Check
	if (!file)
		return false;

	// Create the XML reader
	io::IXMLReader* reader = m_FileSystem->createXMLReader( file );
	// Check
	if (!reader)
		return false;

	// Clear the current processes if we have any
	clear();

	// Loop through all processes and read them in
	while (reader->read())
	{
		if (!wcscmp( L"Processes", reader->getNodeName()))
		{
			// Read the process
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
					if (!wcscmp( L"Process", reader->getNodeName() ) )
					{
						// Read the process
						readProcess( reader );
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
		if (!wcscmp( L"Links", reader->getNodeName()))
		{
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
					if (!wcscmp( L"Link", reader->getNodeName() ) )
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
	}

	reader->drop();

	return true;
}

/// Read a process from an XML file
void CTextureFactory::readProcess( io::IXMLReader *reader )
{
	// Check
	if (!reader)
		return;

	// Get the type of node
	io::EXML_NODE nodeType = reader->getNodeType();
	
	if ((nodeType == io::EXN_NONE) || (nodeType == io::EXN_UNKNOWN) || (nodeType == io::EXN_ELEMENT_END))
		return;

	if (!wcscmp( L"Process", reader->getNodeName()))
	{
		// File the process type and ID and create it
		u32 ID = reader->getAttributeValueAsInt( L"ID" );
		PROCESS_TYPES ptype = (PROCESS_TYPES)reader->getAttributeValueAsInt( L"TYPE" );

		// Create the process
		IImageProcess* proc = addProcess( ptype, ID );

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

					if (proc)
						proc->deserializeAttributes( attr );

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

void CTextureFactory::readLink( io::IXMLReader* reader )
{
	// Check
	if (!reader)
		return;

	// Get the type of node
	io::EXML_NODE nodeType = reader->getNodeType();
	
	if ((nodeType == io::EXN_NONE) || (nodeType == io::EXN_UNKNOWN) || (nodeType == io::EXN_ELEMENT_END))
		return;

	if (!wcscmp( L"Link", reader->getNodeName()))
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
					u32 SourceProcID = attr->getAttributeAsInt( "SourceProc" );
					u32 SourceIdx = attr->getAttributeAsInt( "SourceIdx" );
					u32 DestProcID = attr->getAttributeAsInt( "DestProc" );
					u32 DestIdx = attr->getAttributeAsInt( "DestIdx" );

					// Add the link
					addLink( SourceProcID, SourceIdx, DestProcID, DestIdx );
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

}
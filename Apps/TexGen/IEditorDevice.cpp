#include "StdAfx.h"
#include "IEditorDevice.h"
#include "Workspace.h"

IEditorDevice::IEditorDevice( CWorkspace* workspace, IImageProcess* process )
: m_Workspace( workspace )
, m_pImageProc( process )
, m_PrevSettings( NULL )
, m_PropWindow( NULL )
{
	// Create the previous settings attribue
	m_PrevSettings = m_Workspace->createAttributes();

	// Initalize the gui editor control
	m_EditorControl = m_Workspace->createEditorControl();

	// Check the process and grab it
	if (m_pImageProc)
		m_pImageProc->grab();
}

IEditorDevice::~IEditorDevice( void )
{
	
	// Drop the previous settings attribute
	if (m_PrevSettings)
		m_PrevSettings->drop();

	// Drop the image process
	if (m_pImageProc)
		m_pImageProc->drop();

	// Clear the editor control
	m_EditorControl->drop();

}

/// Get the ID number of this device
/// \return The ID number.
u32 IEditorDevice::getID( void )
{
	return m_pImageProc->getID();
}

/// Get the deice type
PROCESS_TYPES IEditorDevice::getType( void )
{
	return m_pImageProc->getType();
}

/// Get the device name
const c8* IEditorDevice::getName( void )
{
	return m_pImageProc->getName();
}

/// Get the image process linked with this device
IImageProcess* IEditorDevice::getImgProcess( void )
{
	return m_pImageProc;
}

core::vector2d<s32> IEditorDevice::getInputLinkPoint( u32 idx )
{
	if (m_EditorControl)
		return m_EditorControl->getInputPoint( idx );

	return core::vector2d<s32>(0,0);
}

core::vector2d<s32> IEditorDevice::getOutputLinkPoint( u32 idx )
{
	
	if (m_EditorControl)
		return m_EditorControl->getOutputPoint( idx );

	return core::vector2d<s32>(0,0);
}

/// Get the output image of the Image processor
/// \return pointer to the image
video::IImage* IEditorDevice::getImage( u32 width, u32 height )
{	

	// Use the TextureFactory to get the image
	m_pImageProc->setOutputSize( width, height );
	return m_pImageProc->getOutputImage( 0 );
}

u32 IEditorDevice::getListIndex( void )
{
	return m_ListIndex;
}

void IEditorDevice::setListIndex( u32 index )
{
	m_ListIndex = index;
}

/// Get the pointer to the editor control
CGUIEditorControl* IEditorDevice::getEditorControl( void ) const
{
	return m_EditorControl;
}

void IEditorDevice::serializeAttributes( io::IAttributes *out, io::SAttributeReadWriteOptions *options) const
{
	// Save the processes data
	m_pImageProc->serializeAttributes( out, options );
	// Save the location of the edit item
	out->addPosition2d( "Pos", m_EditorControl->getPosition() );
}

void IEditorDevice::deserializeAttributes( io::IAttributes *in, io::SAttributeReadWriteOptions *options)
{
	// Load the processes data
	m_pImageProc->deserializeAttributes( in, options );

	// Load up the edit item location
	m_EditorControl->setPosition( in->getAttributeAsPosition2d( "Pos" ) );
}
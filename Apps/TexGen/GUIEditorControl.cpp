#include "StdAfx.h"
#include "GUIEditorControl.h"

CGUIEditorControl::CGUIEditorControl( gui::IGUIEnvironment* Env, video::IVideoDriver* Driver )
: m_GUIEnv( Env )
, m_Driver( Driver )
, m_BGImage( NULL )
, m_Text( NULL )
{
	// Grab the GUI environment
	if (m_GUIEnv)
		m_GUIEnv->grab();

	// Grab the video Driver
	if (m_Driver)
		m_Driver->grab();

}

CGUIEditorControl::~CGUIEditorControl(void)
{
	// Drop the gui environment
	if (m_GUIEnv)
		m_GUIEnv->drop();

	// Drop the video driver
	if (m_Driver)
		m_Driver->drop();

	// Drop the controls
	if (m_BGImage)
		m_BGImage->remove();
}

/// Create the GUI items for the editor control
/// \param BGTexture The path to the background texture
/// \param NumInputs The number of inputs to create
/// \param NumOutputs The number of outputs to create
/// \param Text The text to place on the lable
/// \param Pos The position in the workspace for the GUI item
bool CGUIEditorControl::Create( const io::path &BGTexture, u32 NumInputs, u32 NumOutputs, const core::stringw &Text, core::vector2d<s32> &Pos )
{
	if (m_BGImage)
		return true;	// Already created

	// Check the environment and the driver
	if (!m_GUIEnv)
		return false;
	if (!m_Driver)
		return false;

	// Save the position
	m_Pos = Pos;
	
	// Get the textures required for the 
	video::ITexture* bgtex = m_Driver->findTexture( BGTexture );
	video::ITexture* link = m_Driver->findTexture( "Link" );

	// Check
	if (!bgtex)
		return false;
	if (!link)
		return false;

	// Now create a new item
	m_BGImage = m_GUIEnv->addImage(	R( m_Pos.X, m_Pos.Y, m_Pos.X + 150, m_Pos.Y + 30 ), 0, -1, 0 );
	m_BGImage->setImage( bgtex );
	m_BGImage->setScaleImage( true );
	
	// Add a label
	m_Text = m_GUIEnv->addStaticText( Text.c_str(), R( 20, 0, 130, 15 ), false, false, m_BGImage, -1, false );

	// Add a link item for each input and output
	for (u32 i = 0; i < NumInputs; i++)
	{
		gui::IGUIImage* img = m_GUIEnv->addImage( R( 0, 0 + (i*10), 10, 10 + (i*10)), m_BGImage, -1, 0 );
		img->setImage( link );
		img->setScaleImage( true );
		// Add it to the input array
		m_InputImages.push_back( img );
	}
	for (u32 i = 0; i < NumOutputs; i++)
	{
		gui::IGUIImage* img = m_GUIEnv->addImage( R( 140, 0 + (i*10), 150, 10 + (i*10)), m_BGImage, -1, 0 );
		img->setImage( link );
		img->setScaleImage( true );
		// Add it to the output array
		m_OutputImages.push_back( img );
	}
	
	return true;
}

/// Check's to see if a point is inside the GUI item
/// \param Point The point to test
/// \return True if the point is inside otherwise false
bool CGUIEditorControl::isPointInside( const core::vector2d<s32> &Point ) const
{
	if (!m_BGImage)
		return false;

	return m_BGImage->isPointInside( Point );
}

/// Get the index of the input image
/// \param Point The point to test
/// \return The index of the input, otherwise -1 if not inside or error
s32 CGUIEditorControl::getInputIndex( const core::vector2d<s32> &Point ) const
{
	if (!isPointInside( Point ) )
		return -1;

	// Loop through all input images and check to see if the point is inside one of them
	for (u32 i = 0; i < m_InputImages.size(); i++)
		if (m_InputImages[i])
			if (m_InputImages[i]->isPointInside( Point ) )
				return i;

	return -1;
}
/// Get the index of the output image
/// \param Point The point to test
/// \return The index of the output, otherwise -1 if not inside or error
s32 CGUIEditorControl::getOutputIndex( const core::vector2d<s32> &Point ) const
{
	if (!isPointInside( Point ) )
		return -1;

	// Loop through all output images and check to see if the point is inside one of them
	for (u32 i = 0; i < m_OutputImages.size(); i++)
		if (m_OutputImages[i])
			if (m_OutputImages[i]->isPointInside( Point ) )
				return i;

	return -1;
}

/// Get a point for an input index
core::vector2d<s32> CGUIEditorControl::getInputPoint( u32 idx ) const
{
	if (idx < m_InputImages.size())
		return m_InputImages[idx]->getRelativePosition().getCenter() + m_Pos;

	return core::vector2d<s32>(0,0);
}
/// Get a point for an output index
core::vector2d<s32> CGUIEditorControl::getOutputPoint( u32 idx ) const
{
	if (idx < m_OutputImages.size())
		return m_OutputImages[idx]->getRelativePosition().getCenter()+ m_Pos;

	return core::vector2d<s32>(0,0);
}

/// Check to see if this is a matching element for the BgImage
/// \param Element The element to match
bool CGUIEditorControl::isElement( const gui::IGUIElement* Element ) const
{
	if (m_BGImage == Element)
		return true;

	return false;
}

/// Get the position of the GUI item within the workspace
/// \return The items position
core::vector2d<s32> CGUIEditorControl::getPosition( void ) const
{
	return m_Pos;
}

/// Set the position of the GUI item within the workspace
/// \param Pos The new position to set
void CGUIEditorControl::setPosition( const core::vector2d<s32> &Pos )
{
	m_Pos = Pos;
	// Set the position of the GUI item
	if (m_BGImage)
		m_BGImage->setRelativePosition( Pos );
}

/// Get the rect of the GUI item within the workspace
/// \return The items rect
core::rect<s32> CGUIEditorControl::getRect( void ) const
{
	return m_BGImage->getRelativePosition();
}

/// Move the position of the GUI item within the workspace
/// \param Pos the amount to move by
void CGUIEditorControl::move( const core::vector2d<s32> &Pos )
{
	m_Pos += Pos;
	m_BGImage->move( Pos );
}

/// Move the position of the GUI to a new position
/// \param Pos The new position to set it to
void CGUIEditorControl::moveTo( const core::vector2d<s32> &Pos )
{
	m_Pos = Pos;
	m_BGImage->setRelativePosition( Pos );
}
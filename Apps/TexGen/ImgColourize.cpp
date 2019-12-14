#include "StdAfx.h"
#include "ImgColourize.h"

namespace TexGen
{

CImgColourize::CImgColourize( video::IVideoDriver *driver, u32 ID )
: IImageProcess( driver, ID )
, m_StartColour( 255, 255, 255, 255 )
, m_EndColour( 255, 0, 0, 0 )
{
	// Setup the arrays for the inputs and outputs
	for (u32 i = 0; i < getNumInputs(); i++)
		m_Inputs.push_back( SInputLink() );

	// Setup the array for the output images, currently NULL pointers as we don't
	// know what size the output images will be
	// TODO: We can do this in the setOutputSize function.
	//for ( u32 i = 0; i < getNumOutputs(); i++ )
	//	m_pOutputImages.push_back( NULL );
}

CImgColourize::~CImgColourize(void)
{
}

/// Get the process type
PROCESS_TYPES CImgColourize::getType( void ) const
{
	return PT_COLOURIZE;
}

/// Colourise the input image
/// \param width The width of the final output image
/// \param height The height of the final output image
void CImgColourize::RunProcess( )
{

	// Check we have an input to work with
	if (!m_Inputs[0].Proc)
		return;

	// Refresh our input process if needed
	m_Inputs[0].Proc->Refresh();

	// Get our input image
	video::IImage* input = m_Inputs[0].Proc->getOutputImage(m_Inputs[0].SourceIdx);

	// Pixel we are working on
	video::SColor pix;

	// Loop through each pixel on the image
	for ( u32 v = 0; v < m_OutputSize.Width; v++ )
	{
		for ( u32 u = 0; u < m_OutputSize.Height; u++ )
		{
			// Get the pixel color
			pix = input->getPixel( v, u );
			// Get the green channel and convert to a 0.0-1.0 float value
			f32 d = (f32)(pix.getGreen() / 255.0f);
			
			// Interpolate to the target colour
			video::SColor res = m_StartColour.getInterpolated( m_EndColour, d );

			// Set the value
			m_OutputImages[0]->setPixel( v, u, res );
		}
	}

	m_Processed = true;
	m_TimeStamp = time(NULL);

	//std::cout << "Rebuilt Colourized. TimeStamp now = " << m_TimeStamp << std::endl;
}

/// Get the number of input processes to this image process
/// \return The number of input processes
u32 CImgColourize::getNumInputs( void ) const
{
	return 1;	// 1 input to this process
}

/// Get the number of output processes to this image process
/// \return The number of out processes
u32 CImgColourize::getNumOutputs( void ) const
{
	return 1;	// 1 output
}

/// Save the attributes of this process
void CImgColourize::serializeAttributes( io::IAttributes *out, io::SAttributeReadWriteOptions *options) const
{
	IImageProcess::serializeAttributes( out, options );
	// Save out our specific settings
	out->addColor( "StartColour", m_StartColour );
	out->addColor( "EndColour", m_EndColour );
}
// Load the attributes of this process
void CImgColourize::deserializeAttributes( io::IAttributes *in, io::SAttributeReadWriteOptions *options)
{
	IImageProcess::deserializeAttributes( in, options );
	// Read in our specific settings
	setStartColour(in->getAttributeAsColor( "StartColour" ));
	setEndColour(in->getAttributeAsColor( "EndColour" ));
}

/// Set the starting colour for the colourize function
/// \param colour The starting colour
void CImgColourize::setStartColour( video::SColor colour )
{
	m_StartColour = colour;

	m_Processed = false;
}

/// Get the starting colour for the colourize function
/// \return The starting colour
video::SColor CImgColourize::getStartColour( void )
{
	return m_StartColour;
}

/// Set the ending colour for the colourize function
/// \param colour The ending colour
void CImgColourize::setEndColour( video::SColor colour )
{
	m_EndColour = colour;

	m_Processed = false;
}

/// Get the ending colour for the colourize function
/// \return The ending colour
video::SColor CImgColourize::getEndColour( void )
{
	return m_EndColour;

	m_Processed = false;
}

}
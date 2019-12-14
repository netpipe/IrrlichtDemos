#include "StdAfx.h"
#include "ImgGradient.h"

namespace TexGen
{

CImgGradient::CImgGradient( video::IVideoDriver *driver, u32 ID )
: IImageProcess( driver, ID )
, m_Angle( 0 )
, m_Scale( 1.0f )
{
	// Setup the arrays for the inputs and outputs
	for (u32 i = 0; i < getNumInputs(); i++)
		m_Inputs.push_back( SInputLink() );

	// Setup the array for the output images, currently NULL pointers as we don't
	// know what size the output images will be
	// TODO: We can do this in the setOutputSize function.
	//for ( u32 i = 0; i < getNumOutputs(); i++ )
	//	m_OutputImages.push_back( NULL );
}

CImgGradient::~CImgGradient(void)
{
}

/// Get the process type
PROCESS_TYPES CImgGradient::getType( void ) const
{
	return PT_GRADIENT;
}

/// Generate the gradient in the image
/// \param width The width of the final output image
/// \param height The height of the final output image
void CImgGradient::RunProcess( )
{
	
	// Calculate the angles
	f32 rangle = core::degToRad( (f32)m_Angle );	// angle in radians
	
	// calculate the height angle based on width of the image
	f32 ha = (256.0f / (f32)m_OutputSize.Width) * m_Scale;
	ha = atan( ha );

	// Temp position var
	core::vector2df pos = core::vector2df( 0, 0 );

	// Check to see if we have an input mask
	bool mask = false;
	video::IImage* input = NULL;
	if (m_Inputs[0].Proc)
	{
		// We do have a mask
		mask = true;

		// Refresh our input process if needed
		m_Inputs[0].Proc->Refresh();

		// Get the mask
		input = m_Inputs[0].Proc->getOutputImage(m_Inputs[0].SourceIdx);
	}

	// Loop through each pixel on the image
	for ( u32 v = 0; v < m_OutputSize.Width; v++ )
	{
		for ( u32 u = 0; u < m_OutputSize.Height; u++ )
		{
			// Rotate the curent position by the angle input
			// Use the Irrlicht engine's vector math to do this
			pos.X = (f32)v;
			pos.Y = (f32)u;
			pos.rotateBy( m_Angle, core::vector2df( (f32)(m_OutputSize.Width / 2), (f32)(m_OutputSize.Height / 2) ) );

			// Calculate the height of the pixel using the hight angle
			u32 h = (u32)(tan( ha ) * pos.X);

			// Check to see if we have a mask
			if (mask)
			{
				// Check
				video::SColor maskpix = input->getPixel(v, u);
				f32 m = (f32)(maskpix.getGreen() / 255.0f);
				
				// multiply the colour we want to set by the mask
				h = (s32)(h * m);
			}

			// Set the value
			m_OutputImages[0]->setPixel( v, u, video::SColor( 255, h, h, h ) );
		}
	}

	m_Processed = true;
	m_TimeStamp = time(NULL);

	//std::cout << "Rebuilt Gradient. Timestamp now = " << m_TimeStamp << std::endl;
}

/// Get the number of input processes to this image process
/// \return The number of input processes
u32 CImgGradient::getNumInputs( void ) const
{
	return 1;	// No inputs into this process
}

/// Get the number of output processes to this image process
/// \return The number of out processes
u32 CImgGradient::getNumOutputs( void ) const
{
	return 1;	// 1 output image
}

/// Save the attributes of this process
void CImgGradient::serializeAttributes( io::IAttributes *out, io::SAttributeReadWriteOptions *options) const
{
	IImageProcess::serializeAttributes( out, options );
	// Save out our specific settings
	out->addInt( "Angle", m_Angle );
	out->addFloat( "Scale", m_Scale );
}
// Load the attributes of this process
void CImgGradient::deserializeAttributes( io::IAttributes *in, io::SAttributeReadWriteOptions *options)
{
	IImageProcess::deserializeAttributes( in, options );
	// Read in our specific settings
	setAngle(in->getAttributeAsInt( "Angle" ));
	setScale(in->getAttributeAsFloat( "Scale" ));
}

/// Set the angle of the gradient
/// \param angle The angle
void CImgGradient::setAngle( s32 angle )
{
	m_Angle = angle;

	//check
	if (m_Angle < 0)
		m_Angle = 0;
	if (m_Angle > 359)
		m_Angle = 359;

	m_Processed = false;
}

/// Get the angle of the gradient
/// \return The angle
s32 CImgGradient::getAngle( void )
{
	return m_Angle;
}

/// Set the scale of the gradient
/// \param scale The scale
void CImgGradient::setScale( f32 scale )
{
	m_Scale = scale;

	// check
	if (m_Scale < 0.1f)
		m_Scale = 0.1f;
	if (m_Scale > 10.0f)
		m_Scale = 10.0f;

	m_Processed = false;
}

/// Get the scale of the gradient
/// \return The scale
f32 CImgGradient::getScale( void )
{
	return m_Scale;
}

}
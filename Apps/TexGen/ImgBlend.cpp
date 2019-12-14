#include "StdAfx.h"
#include "ImgBlend.h"

namespace TexGen
{

CImgBlend::CImgBlend( video::IVideoDriver *driver, u32 ID )
: IImageProcess( driver, ID )
, m_BlendMode( BM_ADD )
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


CImgBlend::~CImgBlend(void)
{
}

/// Get the process type
PROCESS_TYPES CImgBlend::getType( void ) const
{
	return PT_BLEND;
}

/// Colourise the input image
/// \param width The width of the final output image
/// \param height The height of the final output image
void CImgBlend::RunProcess( )
{

	// Check we have the two inputs to work with
	if ( (!m_Inputs[0].Proc) || (!m_Inputs[1].Proc) )
		return;

	// Refresh our input processes
	m_Inputs[0].Proc->Refresh();
	m_Inputs[1].Proc->Refresh();

	// Get our input images
	video::IImage* input0 = m_Inputs[0].Proc->getOutputImage(m_Inputs[0].SourceIdx);
	video::IImage* input1 = m_Inputs[1].Proc->getOutputImage(m_Inputs[1].SourceIdx);

	// Check
	if ( (!input0) || (!input1) )
		return;

	// Pixel we are working on
	video::SColor spix0, spix1;	// 2 Source pixels
	video::SColor pix;			// Dest pixel

	// Loop through each pixel on the image
	for ( u32 v = 0; v < m_OutputSize.Width; v++ )
	{
		for ( u32 u = 0; u < m_OutputSize.Height; u++ )
		{
			// Get the two source pixels
			spix0 = input0->getPixel( v, u );
			spix1 = input1->getPixel( v, u );

			u32 A = 0, R = 0, G = 0, B = 0;
			f32 alpha = 0.0f;
		
			// Find the type of blend mode
			switch (m_BlendMode)
			{
			case BM_ADD:
				// Add the 1st layer to the 2nd layer
				pix.setAlpha(	MIN( ( spix0.getAlpha()	+ spix1.getAlpha()	), 255 ) );
				pix.setRed(		MIN( ( spix0.getRed()	+ spix1.getRed()	), 255 ) );
				pix.setGreen(	MIN( ( spix0.getGreen()	+ spix1.getGreen()	), 255 ) );
				pix.setBlue(	MIN( ( spix0.getBlue()	+ spix1.getBlue()	), 255 ) );
				break;
			case BM_SUB:
				// Subtract the 1st layer from the 2nd layer
				pix.setAlpha(	spix0.getAlpha() );
				pix.setRed(		MAX( ( spix0.getRed()	- spix1.getRed()	), 0 ) );
				pix.setGreen(	MAX( ( spix0.getGreen()	- spix1.getGreen()	), 0 ) );
				pix.setBlue(	MAX( ( spix0.getBlue()	- spix1.getBlue()	), 0 ) );
				break;
			case BM_MUL:
				// Multiply the 1st layer by the 2nd layer and divice by 255
				A = (u32)(( spix0.getAlpha()	* spix1.getAlpha()) / 255 );
				R = (u32)(( spix0.getRed() * spix1.getRed()) / 255 );
				G = (u32)(( spix0.getGreen()	* spix1.getGreen()	) / 255);
				B = (u32)(( spix0.getBlue()	* spix1.getBlue()	) / 255);
				pix.setAlpha(	MIN( A , 255 ) );
				pix.setRed(		MIN( R , 255 ) );
				pix.setGreen(	MIN( G , 255 ) );
				pix.setBlue(	MIN( B, 255 ) );
				break;
			case BM_DIV:
				// Divide the 1st layer by the 2nd layer
				pix.setAlpha(	spix0.getAlpha() );
				if (spix1.getRed() > 0 )
					pix.setRed(		(u32)(spix0.getRed()	/ spix1.getRed()) );
				else
					pix.setRed( spix0.getRed() );
				if (spix1.getGreen() > 0 )
					pix.setGreen(	(u32)(spix0.getGreen()	/ spix1.getGreen()) );
				else
					pix.setGreen( spix0.getGreen() );
				if (spix1.getBlue() > 0 )
					pix.setBlue(	(u32)(spix0.getBlue()	/ spix1.getBlue()) );
				else
					pix.setBlue( spix0.getBlue() );
				break;
			case BM_NORM:
				// Add the 2nd layer onto the first layer based upon the 2nd layers alpha value
				alpha = spix1.getAlpha() / 255.0f;
				pix.setAlpha( spix0.getAlpha() );
				pix.setRed(		MIN( (u32)((spix0.getRed()		* ( 1.0f - alpha ) ) + (spix1.getRed()		* alpha)) , 255 ) );
				pix.setGreen(	MIN( (u32)((spix0.getGreen()	* ( 1.0f - alpha ) ) + (spix1.getGreen()	* alpha)) , 255 ));
				pix.setBlue(	MIN( (u32)((spix0.getBlue()		* ( 1.0f - alpha ) ) + (spix1.getBlue()		* alpha)) , 255 ));
				break;
			default:
				// Just use the source
				pix = spix0;
				break;
			}

			// Set the value
			m_OutputImages[0]->setPixel( v, u, pix );
		}
	}

	m_Processed = true;
	m_TimeStamp = time(NULL);

}

/// Get the number of input processes to this image process
/// \return The number of input processes
u32 CImgBlend::getNumInputs( void ) const
{
	return 2;	// 2 input to this process
}

/// Get the number of output processes to this image process
/// \return The number of out processes
u32 CImgBlend::getNumOutputs( void ) const
{
	return 1;	// 1 output
}

/// Save the attributes of this process
void CImgBlend::serializeAttributes( io::IAttributes *out, io::SAttributeReadWriteOptions *options) const
{
	IImageProcess::serializeAttributes( out, options );
	// Save out our specific settings
	out->addEnum( "BlendMode", m_BlendMode, BM_String );
}
// Load the attributes of this process
void CImgBlend::deserializeAttributes( io::IAttributes *in, io::SAttributeReadWriteOptions *options)
{
	IImageProcess::deserializeAttributes( in, options );
	// Read in our specific settings
	setBlendMode( (BLEND_MODE)in->getAttributeAsEnumeration( "BlendMode", BM_String ));
}

/// Set the blend mode
/// \param Mode The blend mode to set
void CImgBlend::setBlendMode( const BLEND_MODE Mode )
{
	m_BlendMode = Mode;

	m_Processed = false;
}
/// Get the blend mode
/// \return The current blend mode
BLEND_MODE CImgBlend::getBlendMode( void ) const
{
	return m_BlendMode;
}

} // Namespace TexGen
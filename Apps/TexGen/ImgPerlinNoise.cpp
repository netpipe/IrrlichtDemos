#include "StdAfx.h"
#include "ImgPerlinNoise.h"

namespace TexGen
{

CImgPerlinNoise::CImgPerlinNoise( video::IVideoDriver* driver, u32 ID )
: IImageProcess(driver, ID)
, m_pNoise(0)
, m_ScaleX(4.0f)
, m_ScaleY(4.0f)
, m_Octaves(4)
, m_Lacunarity(2.0f)
, m_Gain(0.5f)
, m_Threshold(0.0f)
, m_RandSeed(0)
, m_PType(PNT_STANDARD)
{
	// Setup the arrays for the inputs and outputs
	for (u32 i = 0; i < getNumInputs(); i++)
		m_Inputs.push_back( SInputLink() );
}

CImgPerlinNoise::~CImgPerlinNoise(void)
{
	// Delete the inputs and outputs
	m_Inputs.clear();
}

PROCESS_TYPES CImgPerlinNoise::getType( void ) const
{
	return PT_PERLIN_NOISE;
}

/// Get the number of input processes to this image process
/// \return The number of input processes
u32 CImgPerlinNoise::getNumInputs( void ) const
{
	return 1;	// 1 Input... the mask
}

/// Get the number of output processes to this image process
/// \return The number of out processes
u32 CImgPerlinNoise::getNumOutputs( void ) const
{
	return 1;	// 1 output image
}

/// Save the attributes of this process
void CImgPerlinNoise::serializeAttributes( io::IAttributes *out, io::SAttributeReadWriteOptions *options) const
{
	IImageProcess::serializeAttributes( out, options );
	// Save out our specific settings
	out->addFloat( "Gain", m_Gain );
	out->addInt( "Octaves", m_Octaves );
	out->addFloat( "Lacunarity", m_Lacunarity );
	out->addFloat( "Threshold", m_Threshold );
	out->addInt( "Seed", m_RandSeed );
	out->addFloat( "ScaleX", m_ScaleX );
	out->addFloat( "ScaleY", m_ScaleY );
	out->addEnum( "PType", m_PType, PNT_String );
	
}
// Load the attributes of this process
void CImgPerlinNoise::deserializeAttributes( io::IAttributes *in, io::SAttributeReadWriteOptions *options)
{
	IImageProcess::deserializeAttributes( in, options );
	// Read in our specific settings
	setGain(in->getAttributeAsFloat( "Gain" ));
	setOctaves(in->getAttributeAsInt( "Octaves" ));
	setLacunarity(in->getAttributeAsFloat( "Lacunarity" ));
	setThreshold(in->getAttributeAsFloat( "Threshold" ));
	setSeed(in->getAttributeAsInt( "Seed" ));
	
	f32 scalex = in->getAttributeAsFloat( "ScaleX" );
	f32 scaley = in->getAttributeAsFloat( "ScaleY" );
	setScale( scalex, scaley );

	//setPType( (PERLIN_NOISE_TYPE)in->getAttributeAsInt( "PType" ) );
	setPType( (PERLIN_NOISE_TYPE)in->getAttributeAsEnumeration( "PType", PNT_String ) );
}

/// Set the randon number generator seed
/// \param seed The seed value.
void CImgPerlinNoise::setSeed( u32 seed )
{
	m_RandSeed = seed;

	m_Processed = false;
}

/// Get the seed value
/// \return The seed value
u32 CImgPerlinNoise::getSeed( void )
{
	return m_RandSeed;
}

/// Set the scale of the perlin noise
/// \param X The X scale
/// \param Y The Y scale
void CImgPerlinNoise::setScale( f32 X, f32 Y )
{
	m_ScaleX = X;
	m_ScaleY = Y;

	m_Processed = false;
}

/// Set the scale of the perlin noise
/// \param scale Vector containing the scale
void CImgPerlinNoise::setScale( core::vector2df& scale )
{
	m_ScaleX = scale.X;
	m_ScaleY = scale.Y;

	m_Processed = false;
}

/// Get the scale of the perlin noise
/// \return A vector containing the scale
core::vector2df CImgPerlinNoise::getScale( void ) const
{
	core::vector2df vec( m_ScaleX, m_ScaleY );
	return vec;
}

/// Set the number of octaves in the fractal
void CImgPerlinNoise::setOctaves( u32 octaves )
{
	m_Octaves = octaves;

	m_Processed = false;
}
/// Get the number of octaves in the fractal
u32 CImgPerlinNoise::getOctaves( void ) const
{
	return m_Octaves;
}

/// Set the Lacunarity for the fractal
void CImgPerlinNoise::setLacunarity( f32 lacunarity )
{
	m_Lacunarity = lacunarity;

	m_Processed = false;
}
/// Get the lacunarity for the fractal
f32 CImgPerlinNoise::getLacunarity( void ) const
{
	return m_Lacunarity;
}

/// Set the gain for each octave
void CImgPerlinNoise::setGain( f32 gain )
{
	m_Gain = gain;

	m_Processed = false;
}
/// Get the gain for each octave
f32 CImgPerlinNoise::getGain( void ) const
{
	return m_Gain;
}

/// Set the threshold for the noise
void CImgPerlinNoise::setThreshold( f32 threshold )
{
	m_Threshold = threshold;

	m_Processed = false;
}
/// Get the threshold for the noise
f32 CImgPerlinNoise::getThreshold( void ) const
{
	return m_Threshold;
}

/// Set the type of perlin noise
void CImgPerlinNoise::setPType( PERLIN_NOISE_TYPE ptype )
{
	m_PType = ptype;

	m_Processed = false;
}
/// Get the type of perlin noise
PERLIN_NOISE_TYPE CImgPerlinNoise::getPType( void ) const
{
	return m_PType;
}

void CImgPerlinNoise::RunProcess(  )
{
	
	// Create the 3d perlin noise object
	if (m_pNoise)
		delete m_pNoise;
	m_pNoise = new C3DPerlinNoise(m_RandSeed);

	float result;
	s32 col = 0;
	video::SColor rgb = video::SColor();

	float x = 0, y = 0, z = 0;

	// Calculate the steps based upon the image size and the scale
	float xstep = (m_ScaleX / (float)m_OutputSize.Width);
	float ystep = (m_ScaleY / (float)m_OutputSize.Height);

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

	for (u32 v = 0; v < m_OutputSize.Width; v++)
	{
		x += xstep;
		y = 0;
		
		for (u32 u = 0; u < m_OutputSize.Height; u++)
		{
			if (m_PType == PNT_STANDARD)
			{
				result = (float)m_pNoise->fBm(x, y, z, m_Octaves, m_Lacunarity, m_Gain);
				// Scale from -1..1 to 0..1
				result = (result * 0.5f) + 0.5f;
				// Apply the threshold
				result = MAX( 0.0f, result - m_Threshold );
				// Convert to 256 colours
				col = (s32)(result * 256);
			}
			if (m_PType == PNT_RIDGED)
			{
				result = (float)m_pNoise->RidgedMF( x, y, z, m_Octaves, m_Lacunarity, m_Gain, 1.0f );
				// Scale currently 0 to 1
				// Apply the threshold
				result = MAX( 0.0f, result - m_Threshold );
				// Convert to 256 colours
				col = (s32)(result * 256);
			}
			if (m_PType == PNT_BILLOWY)
			{
				result = 1.0f - (float)m_pNoise->RidgedMF( x, y, z, m_Octaves, m_Lacunarity, m_Gain, 1.0f );
				// Scale currently 0 to 1
				// Apply the threshold
				result = MAX( 0.0f, result - m_Threshold );
				// Convert to 256 colours
				col = (s32)(result * 256);
			}
			
			if (col > 255) col = 255;
			if (col < 0) col = 0;
			
			// Check to see if we have a mask
			if (mask)
			{
				// Check
				video::SColor maskpix = input->getPixel(v, u);
				f32 m = (f32)(maskpix.getGreen() / 255.0f);
				
				// multiply the colour we want to set by the mask
				col = (s32)(col * m);
			}

			m_OutputImages[0]->setPixel(v, u, video::SColor( 255, col, col, col ));
	
			y += ystep;
		}
	}

	m_Processed = true;
	m_TimeStamp = time(NULL);

	//std::cout << "Rebuilt Perlin. TimeStamp now = " << m_TimeStamp << std::endl;
}

}
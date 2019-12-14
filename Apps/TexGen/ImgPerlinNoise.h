#pragma once
#include "IImageProcess.h"
#include "C3DPerlinNoise.h"

namespace TexGen
{

/// Perlin noise type enum
typedef enum PERLIN_NOISE_TYPES
{
	PNT_NONE = 0,
	PNT_STANDARD = 1,
	PNT_RIDGED = 2,
	PNT_BILLOWY = 3,

	PNT_COUNT

} PERLIN_NOISE_TYPE;

static const c8* const PNT_String[] =
{
	"None",
	"Standard",
	"Ridged",
	"Billowy",
	0
};

class CImgPerlinNoise :
	public IImageProcess
{
public:
	CImgPerlinNoise( video::IVideoDriver* driver, u32 ID );
	virtual ~CImgPerlinNoise(void);

	virtual PROCESS_TYPE getType( void ) const;

	virtual u32 getNumInputs( void ) const;
	virtual u32 getNumOutputs( void ) const;

	virtual void serializeAttributes( io::IAttributes *out, io::SAttributeReadWriteOptions *options = 0) const;
	virtual void deserializeAttributes( io::IAttributes *in, io::SAttributeReadWriteOptions *options = 0);

	void setSeed( u32 seed );
	u32 getSeed( void );

	void setScale( f32 X, f32 Y );
	void setScale( core::vector2df& scale );
	core::vector2df getScale( void ) const;

	void setOctaves( u32 octaves );
	u32 getOctaves( void ) const;

	void setLacunarity( f32 lacunarity );
	f32 getLacunarity( void ) const;

	void setGain( f32 gain );
	f32 getGain( void ) const;

	void setThreshold( f32 threshold );
	f32 getThreshold( void ) const;

	void setPType( PERLIN_NOISE_TYPE ptype );
	PERLIN_NOISE_TYPE getPType( void ) const;

protected:

	u32 m_RandSeed;

	C3DPerlinNoise* m_pNoise;

	f32 m_ScaleX;
	f32 m_ScaleY;

	u32 m_Octaves;
	f32 m_Lacunarity;
	f32 m_Gain;
	f32 m_Threshold;

	PERLIN_NOISE_TYPE m_PType;

	virtual void RunProcess( );

};

}

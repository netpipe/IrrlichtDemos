#pragma once
#include "IImageProcess.h"

namespace TexGen
{

/// The blend modes enum
typedef enum BLEND_MODES
{
	BM_ADD = 0,
	BM_SUB = 1,
	BM_MUL = 2,
	BM_DIV = 3,
	BM_NORM = 4,

	BM_COUNT

} BLEND_MODE;

/// The names of the blend modes
static const c8* const BM_String[] =
{
	"Add",
	"Subtract",
	"Multiply",
	"Divide",
	"Normal",
	0
};

class CImgBlend :
	public IImageProcess
{
public:
	CImgBlend( video::IVideoDriver* driver, u32 ID );
	virtual ~CImgBlend(void);

	virtual PROCESS_TYPES getType( void ) const;

	virtual u32 getNumInputs( void ) const;
	virtual u32 getNumOutputs( void ) const;

	virtual void serializeAttributes( io::IAttributes *out, io::SAttributeReadWriteOptions *options = 0) const;
	virtual void deserializeAttributes( io::IAttributes *in, io::SAttributeReadWriteOptions *options = 0);

	void setBlendMode( const BLEND_MODE Mode );
	BLEND_MODE getBlendMode( void ) const;

protected:

	BLEND_MODE	m_BlendMode;

	virtual void RunProcess( );
};

} // Namespace TexGen.


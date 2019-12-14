#pragma once
#include "IImageProcess.h"

namespace TexGen
{

/// Create a greyscale gradiant image
class CImgGradient :
	public IImageProcess
{
public:
	CImgGradient( video::IVideoDriver *driver, u32 ID );
	virtual ~CImgGradient(void);

	virtual PROCESS_TYPES getType( void ) const;

	virtual u32 getNumInputs( void ) const;
	virtual u32 getNumOutputs( void ) const;

	virtual void serializeAttributes( io::IAttributes *out, io::SAttributeReadWriteOptions *options = 0) const;
	virtual void deserializeAttributes( io::IAttributes *in, io::SAttributeReadWriteOptions *options = 0);

	void setAngle( s32 angle );
	s32 getAngle( void );

	void setScale( f32 scale );
	f32 getScale( void );

protected:

	s32 m_Angle;
	f32 m_Scale;

	virtual void RunProcess( );
};

}

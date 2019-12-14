#pragma once
#include "IImageProcess.h"

namespace TexGen
{

class CImgColourize :
	public IImageProcess
{
public:
	CImgColourize( video::IVideoDriver *driver, u32 ID );
	virtual ~CImgColourize(void);

	virtual PROCESS_TYPES getType( void ) const;

	virtual u32 getNumInputs( void ) const;
	virtual u32 getNumOutputs( void ) const;

	virtual void serializeAttributes( io::IAttributes *out, io::SAttributeReadWriteOptions *options = 0) const;
	virtual void deserializeAttributes( io::IAttributes *in, io::SAttributeReadWriteOptions *options = 0);

	void setStartColour( video::SColor colour );
	video::SColor getStartColour( void );
	void setEndColour( video::SColor colour );
	video::SColor getEndColour( void );

protected:

	video::SColor m_StartColour;
	video::SColor m_EndColour;

	virtual void RunProcess( );
};

}

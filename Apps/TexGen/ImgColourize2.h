#pragma once

#include "IImageProcess.h"

namespace TexGen
{

using namespace irr;

class CImgColourize2 :
	public IImageProcess
{
public:
	CImgColourize2( video::IVideoDriver *driver, u32 ID );
	virtual ~CImgColourize2(void);

	virtual PROCESS_TYPES getType( void ) const;

	virtual u32 getNumInputs( void ) const;
	virtual u32 getNumOutputs( void ) const;

	virtual void serializeAttributes( io::IAttributes *out, io::SAttributeReadWriteOptions *options = 0) const;
	virtual void deserializeAttributes( io::IAttributes *in, io::SAttributeReadWriteOptions *options = 0);

	u32 addColourPoint( video::SColor colour, f32 delta );
	u32 getNumColourPoints( void );

	void removeColourPoint( u32 idx );

	video::SColor getColourPointColour( u32 idx );
	void setColourPointColour( u32 idx, video::SColor colour );

	f32 getColourPointDelta( u32 idx );
	void setColourPointDelta( u32 idx, f32 delta );

private:

	typedef struct SColourPoint
	{
		video::SColor	Colour;
		f32				D;
	} SColourPoint;

	core::array<SColourPoint*> m_Colours;		///< The unsorted array of colours
	core::list<SColourPoint*> m_SortedColours;	///< The sorted list of colours

	bool m_Sorted;

	void SortColours( void );

	virtual void RunProcess( );
};

}	// namespace texgen

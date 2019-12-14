#pragma once

namespace TexGen
{

using namespace irr;

/// The device types
typedef enum PROCESS_TYPES
{
	PT_NONE = 0,			///< No Type
	PT_PERLIN_NOISE = 1,	///< Perlin Noise
	PT_GRADIENT = 2,		///< Gradient
	PT_COLOURIZE = 3,		///< Colourize process
	PT_COLOURIZE_ADVANCED = 4,	///< Advanced colourize process
	PT_BLEND = 5,

	PT_COUNT
} PROCESS_TYPE;
/// Device type names
static const c8* const PT_String[PT_COUNT +1] =
{
	"None",
	"Perlin_Noise",
	"Gradient",
	"Colourize",
	"Advanced_Colourize",
	"Blend",
	0
};

class IImageProcess :
	public io::IAttributeExchangingObject
{
public:
	IImageProcess( video::IVideoDriver* driver, u32 ID );
	virtual ~IImageProcess(void);

	u32 getID( void );
	const c8* getName( void ) const;

	virtual PROCESS_TYPE getType( void ) const = 0;

	virtual void setInput( u32 inputIdx, IImageProcess* sourceProc, u32 sourceIdx );
	virtual void removeInput( u32 inputIdx );

	virtual void setOutputSize( u32 width, u32 height );
	virtual video::IImage* getOutputImage( u32 idx );

	virtual void Refresh( bool force = false );

	virtual u32 getNumInputs( void ) const = 0;
	virtual u32 getNumOutputs( void ) const = 0;

	bool isProcessed( void );
	time_t getTimeStamp( void );

	virtual void serializeAttributes( io::IAttributes* out, io::SAttributeReadWriteOptions* options = 0 ) const;
	virtual void deserializeAttributes( io::IAttributes* in, io::SAttributeReadWriteOptions* options = 0 );

protected:

	typedef struct SInputLink
	{
		SInputLink( ) : Proc(NULL), SourceIdx(0) { };	///< Constructor to clear everything

		IImageProcess*	Proc;		///< The source process associated with this link
		u32				SourceIdx;	///< The output index of the source process
	} SInputLink;

	core::array<video::IImage*>	m_OutputImages;	///< The output images
	core::array<SInputLink>		m_Inputs;		///< The inputs into this process

	bool m_Processed;					///< Processed flag. False if the outputs need rebuilding.
	time_t m_TimeStamp;					///< Timestamp for when the image was rebuilt

	video::IVideoDriver* m_Driver;		///< Pointer to the Irrlicht video driver

	u32		m_ID;						///< The process' ID number

	core::dimension2d<u32> m_OutputSize;///< The output image size of the process

	virtual void RunProcess( ) = 0;

};

}

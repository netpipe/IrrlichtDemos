#pragma once

#include "ImgColourize.h"
#include "ImgGradient.h"
#include "ImgPerlinNoise.h"
#include "ImgColourize2.h"
#include "ImgBlend.h"

namespace TexGen
{

using namespace irr;

/// The texture factory class
class CTextureFactory :
	public IReferenceCounted
{
public:
	CTextureFactory( video::IVideoDriver* driver, io::IFileSystem* fs );
	~CTextureFactory(void);

	IImageProcess* addProcess( PROCESS_TYPES type, u32 id = 0 );

	bool addLink( u32 SourceProcID, u32 SourceIdx, u32 DestProcID, u32 DestIdx );
	bool addLink( IImageProcess* SourceProc, u32 SourceIdx, IImageProcess* DestProc, u32 DestIdx);

	u32 getNumProcesses( void );

	IImageProcess* getProcess( u32 index );
	IImageProcess* getProcessByID( u32 ID );

	video::IImage* getOutputImage( u32 ProcessID, u32 outputIdx, u32 width, u32 height );
	video::IImage* getOutputImage( IImageProcess* Process, u32 outputIdx, u32 width, u32 height );

	video::ITexture* addTexture( const io::path& name, u32 ProcessID, u32 outputIdx, u32 width, u32 height );

	void removeProcess( IImageProcess* proc );
	void removeProcess( u32 index );
	void removeProcessByID( u32 ID );

	void clear( void );

	bool saveProcesses( const io::path& filename );
	bool saveProcesses( io::IWriteFile* file );
	bool loadProcesses( const io::path& filename );
	bool loadProcesses( io::IReadFile* file );

private:

	/// A link between two processes
	typedef struct SLink
	{
		// From process...
		IImageProcess*	SourceProc;	///< Input process
		u32				SourceIdx;	///< Output ID on process A
		// ... to process
		IImageProcess*	DestProc;	///< Output process
		u32				DestIdx;	///< Input ID on process B
	} SLink;

	void readProcess( io::IXMLReader* reader );
	void readLink( io::IXMLReader* reader );

	void removeLink( u32 LinkIdx );

	video::IVideoDriver*		m_Driver;		///< The video driver pointer
	io::IFileSystem*			m_FileSystem;	///< The file system pointer
	
	core::array<IImageProcess*>	m_Procs;
	u32							m_NextProcID;

	core::array<SLink*> m_Links;				///< The link between the processes

};

}
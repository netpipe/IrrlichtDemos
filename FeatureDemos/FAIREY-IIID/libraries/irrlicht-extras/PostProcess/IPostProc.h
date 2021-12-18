// PostProcessing framework
// Created by David Evans, 2009
// Parts based on various code available on the Irrlicht forums

// Known bugs / limitations
//	* DirectX shaders not done (anyone?)
//	* Alpha channel gets messed up by most of the shaders. Not a concern unless you're trying to transfer data with it.
//	* You can't add an effect between 2 others easily (though it is possible). I'm not sure how I should implement this.

#ifndef __I_POST_PROC_H_INCLUDED__
#define __I_POST_PROC_H_INCLUDED__

// Irrlicht 1.6+ uses unsigned dimensions for textures, so we'll follow suit
#if (IRRLICHT_VERSION_MAJOR <= 1) && (IRRLICHT_VERSION_MINOR <= 5)
#define DIMENSION2D irr::core::dimension2d<irr::s32>
#else
#define DIMENSION2D irr::core::dimension2d<irr::u32>
#endif

#include "CShaderCallbacks.h"
#include "shaders.h"

enum POSTPROC_QUALITY {
	PPQ_CRUDE = 0,	// Lowest possible quality, best speed
	PPQ_FAST,		// Lower quality, decent looking
	PPQ_DEFAULT,	// Standard quality, general use
	PPQ_GOOD,		// Better quality, makes nicer graphics when not much going on
	PPQ_BEST		// Best quality, use sparingly
};

class IPostProc {
private:
	void init( DIMENSION2D sOut );
	bool pre;
	bool busy;
	bool fixed;
	IPostProc* follows[4];
	irr::video::ITexture* Tfollows[4];
	irr::video::SMaterial Tmats[4];
	bool requireSelfRender;
	
	struct PSConnector {
		IPostProc* obj;
		irr::u8 num;
		PSConnector( IPostProc* o, irr::u8 n ) { obj = o; num = n; }
	};
	
	irr::core::array<PSConnector> outs;
	irr::u32 lastRender;
	
protected:
	POSTPROC_QUALITY quality;
	CTexturesShaderCallback scTextures;
	irr::scene::ISceneManager* smgr;
	irr::scene::CMeshBuffer<irr::video::S3DVertex>* Mesh;
	irr::video::ITexture* output;
	irr::ITimer* timer;
	bool supported;
	bool enabled;
	bool firstRender;
	
	IPostProc( irr::scene::ISceneManager* sm, DIMENSION2D sOut = DIMENSION2D( 0, 0 ) );
	IPostProc( IPostProc* follow1, DIMENSION2D sOut = DIMENSION2D( 0, 0 ) );
	void requestInput( irr::u8 i );
	void requestInput( irr::u8 i, irr::video::ITexture* target );
	void copyEffect( irr::video::ITexture* target );
	
	virtual void renderEffect( irr::video::ITexture* target ) = 0;
	virtual void configureInput( irr::u8 n ) { }
	
public:
	virtual ~IPostProc( );
	void setTimer( irr::ITimer* t );
	irr::ITimer* getTimer( ) { return timer; }
	void render( ) { render( output, 0u, false ); }
	void render( irr::video::ITexture* target, irr::u32 rid = 0u, bool p = false );
	void preRender( );
	void setInput( irr::u8 n, IPostProc* o );
	void setInput( irr::u8 n, irr::video::ITexture* o );
	bool hasInput( irr::u8 n );
	virtual IPostProc* getInput( irr::u8 n = 0u );
	virtual irr::video::ITexture* getInputTexture( irr::u8 n = 0u );
	IPostProc* getRoot( irr::scene::ISceneManager* sm = NULL );
	irr::u8 countInputs( );
	irr::video::ITexture* getOutputTexture( ) { return output; }
	DIMENSION2D getOutputSize( ) { return output->getSize( ); }
	irr::scene::ISceneManager* getSceneManager( ) { return smgr; }
	bool isSupported( ) { return supported; }
	virtual void enable( bool yes = true ) { enabled = yes; }
	void disable( ) { enable( false ); }
	bool isEnabled( ) { return enabled; }
	virtual void setFixed( bool yes = true ) { fixed = yes; }
	bool isFixed( ) { return fixed; }
	virtual void requireFirstRender( ) { firstRender = true; }
	virtual void setQuality( POSTPROC_QUALITY q );
	POSTPROC_QUALITY getQuality( ) { return quality; }
	void setOverallQuality( POSTPROC_QUALITY q );
	
	// Don't use these functions - they are for internal use.
	void addOut( IPostProc* o, irr::u8 n );
	void removeOut( IPostProc* o, irr::u8 n );
	void renderAtTime( irr::u32 rid, bool p = false ) { render( output, rid, p ); }	// Function name is a bit messy, but this allows render( NULL ) to render to the screen.
};

#endif
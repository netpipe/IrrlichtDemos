// PostProcessing framework
// Created by David Evans, 2009
// Parts based on various code available on the Irrlicht forums

// Known bugs / limitations
//	* DirectX shaders not done (anyone?)
//	* Alpha channel gets messed up by most of the shaders. Not a concern unless you're trying to transfer data with it.
//	* You can't add an effect between 2 others easily (though it is possible). I'm not sure how I should implement this.

#ifndef __I_POST_PROC_H_INCLUDED__
#define __I_POST_PROC_H_INCLUDED__

#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#include "CShaderCallbacks.h"
#include "shaders.h"

class IPostProc {
private:
	void init( dimension2di sOut );
protected:
	ISceneManager* smgr;
	CMeshBuffer<S3DVertex>* Mesh;
	ITexture* output;
	IPostProc* follows[4];
	u8 nFollows;
	array<IPostProc*> outs;
	array<u8> outIs;
	u32 lastRender;
	bool supported;
	bool enabled;
	bool busy;
	
	IPostProc( ISceneManager* sm, dimension2di sOut = dimension2di( 0, 0 ) );
	IPostProc( IPostProc* follow1, dimension2di sOut = dimension2di( 0, 0 ) );
	void requestInput( u8 i );
	void requestInput( u8 i, ITexture* target );
	void copyEffect( ITexture* target );
	
	virtual void renderEffect( ITexture* target ) = 0;
	virtual void configureInput( u8 n ) = 0;
	
public:
	virtual ~IPostProc( );
	void render( ) { render( output, 0 ); }
	void render( ITexture* target, u32 rid = 0u );
	void setInput( u8 n, IPostProc* o );
	virtual IPostProc* getInput( u8 n = 0 );
	IPostProc* getRoot( ISceneManager* sm = NULL );
	u8 countInputs( ) { return nFollows; }
	ITexture* getOutputTexture( ) { return output; }
	ISceneManager* getSceneManager( ) { return smgr; }
	bool isSupported( ) { return supported; }
	virtual void enable( bool yes = true ) { enabled = yes; }
	void disable( ) { enable( false ); }
	bool isEnabled( ) { return enabled; }
	
	// Don't use these functions - they are for internal use.
	void addOut( IPostProc* o, u8 n );
	void removeOut( IPostProc* o, u8 n );
	void renderAtTime( u32 rid ) { render( output, rid ); }	// Function name is a bit messy, but this allows render( NULL ) to render to the screen.
};

#endif
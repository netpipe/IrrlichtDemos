// PostProcessing framework - Renderer
// Created by David Evans, 2009

#ifndef __C_RENDERER_POST_PROC_H_INCLUDED__
#define __C_RENDERER_POST_PROC_H_INCLUDED__

#include "IPostProc.h"

class CRendererPostProc : public IPostProc {
private:
	bool cbb;
	bool czb;
	irr::video::SColor c;
	
public:
	CRendererPostProc( irr::scene::ISceneManager* sm, DIMENSION2D sOut, bool clearBackBuffer = true, bool clearZBuffer = true, irr::video::SColor color = irr::video::SColor( 0u, 0u, 0u, 0u ) );
	virtual void renderEffect( irr::video::ITexture* target );
	bool isClearBackBuffer( ) { return cbb; }
	bool isClearZBuffer( ) { return czb; }
	irr::video::SColor getColor( ) { return c; }
	void setClearBackBuffer( bool on = true ) { cbb = on; }
	void setClearZBuffer( bool on = true ) { czb = on; }
	void setColor( irr::video::SColor to ) { c = to; c.setAlpha( 0u ); }
	void setParameters( bool cb, bool cz, irr::video::SColor col ) { setClearBackBuffer( cbb ); setClearZBuffer( cz ); setColor( col ); }
};

#endif
// PostProcessing framework - Renderer
// Created by David Evans, 2009

#ifndef __C_RENDERER_POST_PROC_H_INCLUDED__
#define __C_RENDERER_POST_PROC_H_INCLUDED__

#include "IPostProc.h"

class CRendererPostProc : public IPostProc {
private:
	bool cbb;
	bool czb;
	SColor c;
	
public:
	CRendererPostProc( ISceneManager* sm, dimension2di sOut, bool clearBackBuffer = true, bool clearZBuffer = true, SColor color = SColor( 0, 0, 0, 0 ) );
	virtual void configureInput( u8 n ) { }
	virtual void renderEffect( ITexture* target );
	bool isClearBackBuffer( ) { return cbb; }
	bool isClearZBuffer( ) { return czb; }
	SColor getColor( ) { return c; }
	void setClearBackBuffer( bool on = true ) { cbb = on; }
	void setClearZBuffer( bool on = true ) { czb = on; }
	void setColor( SColor to ) { c = to; }
};

#endif
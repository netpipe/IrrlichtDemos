// PostProcessing framework - Transition
// Created by David Evans, 2009

// Creates a transition from followA to followB using the given texture.
// blend = 0	=> followA visible
// blend = 1	=> followB visible
// in between, followA is visible where the texture red channel is > blend
// and followB is visible elsewhere. some interpolation is applied.

#ifndef __C_TRANSITION_POST_PROC_H_INCLUDED__
#define __C_TRANSITION_POST_PROC_H_INCLUDED__

#include "IPostProc.h"

class CTransitionPostProc : public IPostProc {
private:
	SMaterial mat;
	CShaderCallback shadercallback;
	f32 shaderparameters[8];
	ITexture* tmap;
	f32 ps;
	
public:
	CTransitionPostProc( IPostProc* followA, IPostProc* followB, dimension2di sOut, ITexture* m );
	virtual void configureInput( u8 n );
	virtual void renderEffect( ITexture* target );
	virtual void enable( bool yes = true );
	void setBlend( f32 p );
	f32 getBlend( ) { return ps; };
};

#endif
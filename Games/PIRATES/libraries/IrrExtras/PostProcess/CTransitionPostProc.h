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
	irr::video::SMaterial mat;
	irr::video::SMaterial matFast;
	CShaderCallback scTrans;
	irr::video::ITexture* tmap;
	irr::f32 ps;
	
protected:
	virtual void configureInput( irr::u8 n );
	
public:
	CTransitionPostProc( IPostProc* followA, IPostProc* followB, DIMENSION2D sOut, irr::video::ITexture* m );
	virtual void renderEffect( irr::video::ITexture* target );
	void setBlend( irr::f32 p );
	irr::f32 getBlend( ) { return ps; };
};

#endif
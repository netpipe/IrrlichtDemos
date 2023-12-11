// PostProcessing framework - Splitscreen
// Created by David Evans, 2009

// Creates a split screen with 1-4 screens

#ifndef __C_SPLIT_POST_PROC_H_INCLUDED__
#define __C_SPLIT_POST_PROC_H_INCLUDED__

#include "IPostProc.h"

class CSplitPostProc : public IPostProc {
private:
	void init( IPostProc* followA, IPostProc* followB, IPostProc* followC, IPostProc* followD, irr::core::rect<irr::f32> zone1, irr::core::rect<irr::f32> zone2, irr::core::rect<irr::f32> zone3, irr::core::rect<irr::f32> zone4 );
	irr::scene::CMeshBuffer<irr::video::S3DVertex>* m[4];
	irr::core::rect<irr::f32> z[4];
	bool bvspace;
	irr::video::SColor vspace;
	
protected:
	virtual void configureInput( irr::u8 n );
	
public:
	CSplitPostProc( IPostProc* followA, DIMENSION2D sOut, irr::core::rect<irr::f32> zone1 = irr::core::rect<irr::f32>( -1.0f, -1.0f, 1.0f, 1.0f ) );
	CSplitPostProc( IPostProc* followA, IPostProc* followB, DIMENSION2D sOut, irr::core::rect<irr::f32> zone1 = irr::core::rect<irr::f32>( -1.0f, -1.0f, 1.0f, 0.0f ), irr::core::rect<irr::f32> zone2 = irr::core::rect<irr::f32>( -1.0f, 0.0f, 1.0f, 1.0f ) );
	CSplitPostProc( IPostProc* followA, IPostProc* followB, IPostProc* followC, DIMENSION2D sOut, irr::core::rect<irr::f32> zone1 = irr::core::rect<irr::f32>( -0.5f, -1.0f, 0.5f, 0.0f ), irr::core::rect<irr::f32> zone2 = irr::core::rect<irr::f32>( -1.0f, 0.0f, 0.0f, 1.0f ), irr::core::rect<irr::f32> zone3 = irr::core::rect<irr::f32>( 0.0f, 0.0f, 1.0f, 1.0f ) );
	CSplitPostProc( IPostProc* followA, IPostProc* followB, IPostProc* followC, IPostProc* followD, DIMENSION2D sOut, irr::core::rect<irr::f32> zone1 = irr::core::rect<irr::f32>( -1.0f, -1.0f, 0.0f, 0.0f ), irr::core::rect<irr::f32> zone2 = irr::core::rect<irr::f32>( 0.0f, -1.0f, 1.0f, 0.0f ), irr::core::rect<irr::f32> zone3 = irr::core::rect<irr::f32>( -1.0f, 0.0f, 0.0f, 1.0f ), irr::core::rect<irr::f32> zone4 = irr::core::rect<irr::f32>( 0.0f, 0.0f, 1.0f, 1.0f ) );
	virtual ~CSplitPostProc( );
	virtual void renderEffect( irr::video::ITexture* target );
	void setZone( irr::u8 n, irr::core::rect<irr::f32> to );
	irr::core::rect<irr::f32> getZone( irr::u8 n ) { if( n < 4u ) return z[n]; return irr::core::rect<irr::f32>( 0.0f, 0.0f, 0.0f, 0.0f ); }
	void setVoidColor( ) { bvspace = false; }
	void setVoidColor( irr::video::SColor to ) { vspace = to; bvspace = true; }
	irr::video::SColor getVoidColor( ) { return vspace; };
};

#endif
// PostProcessing framework - Splitscreen
// Created by David Evans, 2009

// Creates a split screen with 1-4 screens

#ifndef __C_SPLIT_POST_PROC_H_INCLUDED__
#define __C_SPLIT_POST_PROC_H_INCLUDED__

#include "IPostProc.h"

class CSplitPostProc : public IPostProc {
private:
	void init( IPostProc* followA, IPostProc* followB, IPostProc* followC, IPostProc* followD, rect<float> zone1, rect<float> zone2, rect<float> zone3, rect<float> zone4 );
	CMeshBuffer<S3DVertex>* m[4];
	rect<float> z[4];
	bool bvspace;
	SColor vspace;

public:
	CSplitPostProc( IPostProc* followA, dimension2di sOut, rect<float> zone1 = rect<float>( -1.0f, -1.0f, 1.0f, 1.0f ) );
	CSplitPostProc( IPostProc* followA, IPostProc* followB, dimension2di sOut, rect<float> zone1 = rect<float>( -1.0f, -1.0f, 1.0f, 0.0f ), rect<float> zone2 = rect<float>( -1.0f, 0.0f, 1.0f, 1.0f ) );
	CSplitPostProc( IPostProc* followA, IPostProc* followB, IPostProc* followC, dimension2di sOut, rect<float> zone1 = rect<float>( -0.5f, -1.0f, 0.5f, 0.0f ), rect<float> zone2 = rect<float>( -1.0f, 0.0f, 0.0f, 1.0f ), rect<float> zone3 = rect<float>( 0.0f, 0.0f, 1.0f, 1.0f ) );
	CSplitPostProc( IPostProc* followA, IPostProc* followB, IPostProc* followC, IPostProc* followD, dimension2di sOut, rect<float> zone1 = rect<float>( -1.0f, -1.0f, 0.0f, 0.0f ), rect<float> zone2 = rect<float>( 0.0f, -1.0f, 1.0f, 0.0f ), rect<float> zone3 = rect<float>( -1.0f, 0.0f, 0.0f, 1.0f ), rect<float> zone4 = rect<float>( 0.0f, 0.0f, 1.0f, 1.0f ) );
	virtual void configureInput( u8 n );
	virtual void renderEffect( ITexture* target );
	void setZone( u8 n, rect<float> to );
	rect<float> getZone( u8 n ) { if( n < 4u ) return z[n]; return rect<float>( 0.0f, 0.0f, 0.0f, 0.0f ); }
	void setVoidColor( ) { bvspace = false; }
	void setVoidColor( SColor to ) { vspace = to; bvspace = true; }
	SColor getVoidColor( ) { return vspace; };
};

#endif

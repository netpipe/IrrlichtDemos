// PostProcessing framework - Lens Flare
// Created by David Evans, 2009

// Creates a single lens flare effect

#ifndef __C_LENS_FLARE_POST_PROC_H_INCLUDED__
#define __C_LENS_FLARE_POST_PROC_H_INCLUDED__

#include "IPostProc.h"

class CLensFlarePostProc : public IPostProc {
private:
	irr::video::SColorf mCol;
	irr::scene::CMeshBuffer<irr::video::S3DVertex>* mf;
	irr::core::vector3d<irr::f32> pos;
	irr::f32 rad;
	irr::scene::ISceneNode* track;
	irr::core::array<irr::f32> flP;
	irr::core::array<irr::f32> flR;
	irr::core::array< irr::core::rect<irr::f32> > flT;
	
	void init( IPostProc* follow, DIMENSION2D sOut, irr::video::ITexture* m );
	
public:
	CLensFlarePostProc( IPostProc* follow, DIMENSION2D sOut, irr::video::ITexture* m, irr::core::vector3d<irr::f32> p = irr::core::vector3d<irr::f32>( 0.0f, 0.0f, 0.0f ), irr::f32 r = 0.0f );
	CLensFlarePostProc( IPostProc* follow, DIMENSION2D sOut, irr::video::ITexture* m, irr::scene::ISceneNode* p, irr::f32 r = 0.0f );
	virtual ~CLensFlarePostProc( );
	virtual void renderEffect( irr::video::ITexture* target );
	void setPosition( irr::core::vector3d<irr::f32> p, irr::f32 r = -1.0f ) { pos = p; track = NULL; if( r >= 0.0f ) rad = r; }
	void setPosition( irr::scene::ISceneNode* p, irr::f32 r = -1.0f ) { track = p; if( r >= 0.0f ) rad = r; }
	void setRadius( irr::f32 r ) { rad = r; }
	void setColor( irr::video::SColorf to ) { mCol = to; }
	irr::video::SColorf getColor( ) { return mCol; }
	irr::core::vector3d<irr::f32> getPosition( ) { if( track == NULL ) return pos; return track->getAbsolutePosition( ); }
	irr::scene::ISceneNode* getTrackedNode( ) { return track; }
	irr::f32 getRadius( ) { return rad; }
};

#endif
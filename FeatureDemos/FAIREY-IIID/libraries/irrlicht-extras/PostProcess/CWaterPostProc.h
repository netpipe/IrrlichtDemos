// PostProcessing framework - Water
// Created by David Evans, 2009

// Creates a reflective surface with optional ripples

#ifndef __C_WATER_POST_PROC_H_INCLUDED__
#define __C_WATER_POST_PROC_H_INCLUDED__

#include "CRendererPostProc.h"

class CWaterPostProc : public IPostProc {
private:
	irr::f32 ylevel;
	irr::video::SMaterial mat;
	irr::video::SMaterial matUnder;
	irr::scene::ICameraSceneNode* refCam;
	CRendererPostProc* reflection;
	
	class CWaterShaderCallback : public irr::video::IShaderConstantSetCallBack {
	private:
		irr::f32 pCameraB[3];
		irr::f32 iSmooth;
		irr::f32 iVis;
		irr::f32 iLPen;
		irr::f32 rZero;
		irr::f32 iWaveSize;
		irr::f32 colSurfB[3];
		irr::f32 colDeepB[3];
	public:
		irr::f32 time;
		irr::core::vector3d<irr::f32> pCamera;
		irr::video::SColorf colSurf;
		irr::video::SColorf colDeep;
		irr::f32 ylevel;
		irr::f32 refraction;
		irr::f32 smooth;
		irr::f32 sharp;
		irr::f32 visibility;
		irr::f32 lightPen;
		irr::f32 waveHeight;
		irr::f32 waveSize;
		virtual void OnSetConstants( irr::video::IMaterialRendererServices* services, irr::s32 num ) {
			services->setPixelShaderConstant( "time", &time, 1 );
			pCameraB[ 0u ] = pCamera.X;
			pCameraB[ 1u ] = pCamera.Y;
			pCameraB[ 2u ] = pCamera.Z;
			services->setPixelShaderConstant( "pCamera", &pCameraB[ 0u ], 3 );
			services->setPixelShaderConstant( "ylevel", &ylevel, 1 );
			services->setPixelShaderConstant( "waveHeight", &waveHeight, 1 );
			iWaveSize = 1.0f / waveSize;
			services->setPixelShaderConstant( "iWaveSize", &iWaveSize, 1 );
			services->setPixelShaderConstant( "refraction", &refraction, 1 );
			rZero = ((1.0f - refraction) * (1.0f - refraction)) / ((1.0f + refraction) * (1.0f + refraction));
			services->setPixelShaderConstant( "rZero", &rZero, 1 );
			iSmooth = 1.0f / smooth;
			services->setPixelShaderConstant( "iSmooth", &iSmooth, 1 );
			iVis = 1.0f / visibility;
			services->setPixelShaderConstant( "iVis", &iVis, 1 );
			iLPen = 1.0f / lightPen;
			services->setPixelShaderConstant( "iLPen", &iLPen, 1 );
			colSurfB[ 0u ] = colSurf.getRed( );
			colSurfB[ 1u ] = colSurf.getGreen( );
			colSurfB[ 2u ] = colSurf.getBlue( );
			services->setPixelShaderConstant( "colSurf", &colSurfB[ 0u ], 3 );
			colDeepB[ 0u ] = colDeep.getRed( );
			colDeepB[ 1u ] = colDeep.getGreen( );
			colDeepB[ 2u ] = colDeep.getBlue( );
			services->setPixelShaderConstant( "colDeep", &colDeepB[ 0u ], 3 );
			
			irr::s32 t1 = 0, t2 = 1;
			services->setPixelShaderConstant( "texture1", (irr::f32*) &t1, 1 );
			services->setPixelShaderConstant( "texture2", (irr::f32*) &t2, 1 );
		}
	};
	CWaterShaderCallback scWater;
	
protected:
	virtual void configureInput( irr::u8 n );
	
public:
	CWaterPostProc( IPostProc* follow, DIMENSION2D sOut, irr::f32 depth = 0.0f );
	virtual ~CWaterPostProc( );
	virtual void renderEffect( irr::video::ITexture* target );
	void setDepth( irr::f32 depth ) { ylevel = depth; }
	irr::f32 getDepth( ) { return ylevel; }
};

#endif
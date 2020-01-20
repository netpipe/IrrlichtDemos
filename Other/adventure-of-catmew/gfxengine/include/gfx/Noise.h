// Copyright (C) 2007 Kevin van Leeuwen
// This file is part of the "LeeuwenGfx" Library.
// For conditions of distribution and use, see copyright notice in LeeuwenGfx.h

#ifndef _NoiseGfxHeader_
#define _NoiseGfxHeader_

#include "Vector.h"
#include "Color.h"
#include "ITexture.h"

namespace lew
{
	namespace gfx
	{
		namespace noise
		{
			extern float cos_interpolation(const float x, const float y, const float d);

			extern float noise1D(int x);
			extern float snoise1D(const int x); 
			extern float inoise1D(const float x); 
			extern float pnoise1D(const float x, const float p, const int o);

			extern float noise2D(const float x, const float y, 
				const float m = 1.0f, const float t = 1.0f);
			
			extern float snoise2D(const float x, const float y, 
				const float m = 1.0f, const float t = 1.0f);
			
			extern float inoise2D(const float x, const float y, 
				const float m = 1.0f, const float t = 1.0f);
			
			extern float pnoise2D(const float x, const float y, const float p, 
			const int o, const float m = 1.0f, const float t = 1.0f);
						
			//texture = texture which will hold the noise values
			//octaves = noise detail
			//persistence = how much "noise" it should contain
			//zoom = noise scale 
			//coords = coordinations of the noise. the noise is generated 
			//through frequencies which can be aligned on a grid, the coords value
			//defines where on the grid whe start to calculate the noise
			//contrast = define the noise contrast (e.g. define black/white parts)
			//modification = used for generating different noise effects 
			//turbulence = how smooth the noise will be 
			//error-correction = should pixel-"holes" be fixed
			//correction-tolerance = pixels which are x% different from the 
			//surrounding pixels will be interpolated 
			extern void createNoiseTexture(ITexture* pTex, const int pOct = 8, 
				const float pPersistence = 0.65f, const float pZoom = 0.02f,
				const Vector pCoords = Vector(0.0f, 0.0f),  
				const float pMod = 1.0f, const float pTurbulence = 1.0f,
				const bool pInvert = false);
				
			extern void createColorTexture(ITexture* pHeightmap, 
				ITexture* pColormap, ITexture* pNoisemap,
				const float pDesertExpansion = 150.0f, 
				const float pArcticExpansion = 200.0f, 
				const int pClimateIntensity = 1,
				const float pNoiseIntensity = 6,
				Color pDeepSea = Color(0.05f, 0.08f, 0.14f, 1.0f), 
				Color pSea = Color(0.05f, 0.08f, 0.14f, 1.0f),
				Color pOpenWater = Color(0.05f, 0.16f, 0.24f, 1.0f), 
				Color pShore = Color(0.10f, 0.32f, 0.48f, 1.0f),
				Color pSand = Color(0.85f, 0.8f, 0.38f, 1.0f),
				Color pGrass = Color(0.39f, 0.58f, 0.13f, 1.0f),
				Color pHills = Color(0.5f, 0.6f, 0.0f, 1.0f),
				Color pRocks = Color(0.70f, 0.65f, 0.35f, 1.0f),
				Color pMountains = Color(0.50f, 0.45f, 0.15f, 1.0f),
				Color pSnow = Color(1.0f, 1.0f, 1.0f, 1.0f),
				Color pArctic = Color(0.8f, 0.91f, 0.91f, 1.0f),
				Color pDesert = Color(0.85f, 0.8f, 0.38f, 1.0f));
		};
	};
};

#endif


#ifdef _WIN32
#pragma once
#endif

//********************************************************************************************
// File:   CWindGen.h
// Purpose:*sniggers* generate some wind *sniggers*, should be renamed CRoganJosh :)
// Started: 26th May 2011
// Last ed: 26th May 2011
// History:
//********************************************************************************************

#ifndef __CWINDGEN_H__
#define __CWINDGEN_H__


#include <irrlicht.h>
#include "VectorMath.h"
#include "perlin.h"

using namespace irr;
using namespace core;

class CWindGen
{
	vector3df	mindir;		// min direction
	vector3df	maxdir;		// max direction
	float		strength;	// strength of the wind
	float		variation;	// magnitude of the variation
	Perlin1D*	dir_pNoise;	// 1d perlin noise control for direction interpolation
	Perlin1D*	str_pNoise;	// 1d perlin noise for modulating the strength variation

public:

	CWindGen(vector3df dir, float var, Perlin1D* dir_pN, float strngth, float strvar, Perlin1D* str_pN)
		: mindir(dir), maxdir(dir), dir_pNoise(dir_pN), strength(strngth), variation(strvar), str_pNoise(str_pN)
	{
		mindir.rotateXZBy(-var * 0.5);
		maxdir.rotateXZBy(var * 0.5);
	}
	void get(float f,vector3df& windv)
	{
		float mag = strength + str_pNoise->pnoise1D(f) * variation;
		VectorLerp(mindir, maxdir, dir_pNoise->pnoise1D(f),windv);
		VectorMultiply(windv,mag,windv);
	}
	void get(float f,vector3df& windv, vector3df& winddir)
	{
		float mag = strength + str_pNoise->pnoise1D(f) * variation;
		VectorLerp(mindir, maxdir, dir_pNoise->pnoise1D(f),winddir);
		VectorMultiply(winddir,mag,windv);
	}
};


#endif

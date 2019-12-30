#ifdef _WIN32
#pragma once
#endif
//********************************************************************************************
// File:   perlin.h
// Purpose:simple perlin noise generator
// Author: Mr Perlin
// Started: 26th May 2011
// Last ed: 
// History: 
//http://creativecommons.org/licenses/by-sa/3.0/
//********************************************************************************************

#ifndef __PERLIN_H__
#define __PERLIN_H__

#include <stdlib.h>
#include <vector>
using namespace std;


#define s_curve(t) ( t * t * (3.0f - 2.0f * t) )
#define lerp(t, a, b) ( a + t * (b - a) )

//********************************************************************************************

class Perlin1D
{
	int				Size;	
	int				Octaves;
	float			Frequency;
	float			Amplitude;
	vector<int>		p;
	vector<float>	g1;

public:

	Perlin1D(int sz, int octs, float freq, float amp, int seed) :
		Size(sz), Octaves(octs), Frequency(freq), Amplitude(amp) , p(sz * 2 + 2), g1(sz * 2 + 2) 
	{
		int i, j, k;
		srand(seed);

		for(i = 0 ; i < Size ; i++)
		{
			p[i] = i;
			g1[i] = (float)((rand() % (Size + Size)) - Size) / Size;
		}
		while(--i)
		{
			k = p[i];
			p[i] = p[j = rand() % Size];
			p[j] = k;
		}
		for(i = 0 ; i < Size + 2 ; i++)
		{
			p[Size + i] = p[i];
			g1[Size + i] = g1[i];
		}
	}

//********************************************************************************************
// raw noise generator

	float noise1D(float arg)
	{
		int bx0, bx1;
		float rx0, rx1, sx, t, u, v;

		t = arg + 0x1000;
		bx0 = ((int)t) & (Size - 1);
		bx1 = (bx0+1) & (Size - 1);
		rx0 = t - (int)t;
		rx1 = rx0 - 1.0f;

		sx = s_curve(rx0);

		u = rx0 * g1[p[bx0]];
		v = rx1 * g1[p[bx1]];

		return lerp(sx, u, v);
	}

//********************************************************************************************
// modulated noise

	float pnoise1D(float arg)
	{
		int terms    = Octaves;
		float freq   = Frequency;
		float amp	=	Amplitude;
		float result = 0.0f;

		arg *= Frequency;

		for(int i = 0; i < terms; i++)
		{
			result += noise1D(arg) * amp;
			arg *= 2.0f;
			amp *= 0.5f;
		}
		return result;
	}

//********************************************************************************************
 
};

















#endif 

// *************************************************************************************************
//
// Horde3D
//   Next-Generation Graphics Engine
//
// Sample Application
// --------------------------------------
// Copyright (C) 2006-2008 Nicolas Schulz
//
//
// This sample source file is not covered by the LGPL as the rest of the SDK
// and may be used without any restrictions
//
// *************************************************************************************************

#ifndef _crowd_H_
#define _crowd_H_

#include "Horde3D.h"
#include <vector>
#include <string>
using namespace std;


struct Particle
{
	float			px, pz;		// Current postition
	float			dx, dz;		// Destination position
	float			fx, fz;		// Force on particle
	float			ox, oz;		// Orientation vector
	NodeHandle		node;
	float			animTime;


	Particle()
	{
		ox = 0; oz = 0;
		node = 0;
		animTime = 0;
	}
};


class CrowdSim
{
private:

	string				_contentDir;
	vector< Particle >	_particles;

	void chooseDestination( Particle &p );

public:
	CrowdSim( const string& contentDir ) : _contentDir( contentDir ) {}

	void init();
	void update( float fps );
};

#endif // _crowd_H_

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

#include "crowd.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "Horde3DUtils.h"


void CrowdSim::chooseDestination( Particle &p )
{
	// Choose random destination within a circle
	float ang = ((rand() % 360) / 360.0f) * 6.28f;
	float rad = (rand() % 20) * 1.0f;

	p.dx = sinf( ang ) * rad;
	p.dz = cosf( ang ) * rad;
}


void CrowdSim::init()
{
	// Init random generator
	srand( (unsigned int)time( NULL ) );
	
	// Load character with walk animation
	ResHandle characterRes = Horde3D::addResource( ResourceTypes::SceneGraph, "man.scene.xml", 0 );
	ResHandle characterWalkRes = Horde3D::addResource( ResourceTypes::Animation, "man.anim", 0 );
	Horde3DUtils::loadResourcesFromDisk( _contentDir.c_str() );
	
	// Add characters
	for( unsigned int i = 0; i < 100; ++i )
	{
		Particle p;
		
		// Add character to scene and apply animation
		p.node = Horde3D::addNodes( RootNode, characterRes );
		Horde3D::setupModelAnimStage( p.node, 0, characterWalkRes, "", false );
		
		// Characters start in a circle formation
		p.px = sinf( (i / 100.0f) * 6.28f ) * 10.0f;
		p.pz = cosf( (i / 100.0f) * 6.28f ) * 10.0f;

		chooseDestination( p );

		Horde3D::setNodeTransform( p.node, p.px, 0.02f, p.pz, 0, 0, 0, 1, 1, 1 );

		_particles.push_back( p );
	}
}


void CrowdSim::update( float fps )
{
	// Parameters for three repulsion zones
	float d1 = 0.25f, d2 = 2.0f, d3 = 4.5f;
	float f1 = 3.0f, f2 = 1.0f, f3 = 0.1f;
	
	for( unsigned int i = 0; i < _particles.size(); ++i )
	{
		Particle &p = _particles[i];
		
		// Reset force
		p.fx = 0; p.fz = 0;
		
		// Calculate distance to destination
		float dist = sqrtf( (p.dx - p.px)*(p.dx - p.px) + (p.dz - p.pz)*(p.dz - p.pz) );
		
		// If destination not reached walk to destination
		if( dist > 3.0f )
		{
			// Calculate normalized attraction force to destination
			float afx = (p.dx - p.px) / dist;
			float afz = (p.dz - p.pz) / dist;	

			p.fx += afx * 0.035f; p.fz += afz * 0.035f;

			// Repulsion forces from other particles
			for( unsigned int j = 0; j < _particles.size(); ++j )
			{
				if( j == i ) continue;
				
				Particle &p2 = _particles[j];
				
				float dist2 = sqrtf( (p.px - p2.px)*(p.px - p2.px) + (p.pz - p2.pz)*(p.pz - p2.pz) );
				float strength = 0;

				float rfx = (p.px - p2.px) / dist2;
				float rfz = (p.pz - p2.pz) / dist2;
				
				// Use three zones with different repulsion strengths
				if( dist2 <= d3 && dist2 > d2 )
				{
					float m = (f3 - 0) / (d2 - d3);
					float t = 0 - m * d3;
					strength = m * dist2 + t;
				}
				else if( dist2 <= d2 && dist2 > d1 )
				{
					float m = (f2 - f3) / (d1 - d2);
					float t = f3 - m * d2;
					strength = m * dist2 + t;
				}
				else if( dist2 <= d1 )
				{
					float m = (f1 - f2) / (0 - d1);
					float t = f2 - m * d1;
					strength = m * dist2 + t;
				}

				p.fx += rfx * strength; p.fz += rfz * strength;
			}
		}
		else
		{
			// Choose new destination
			chooseDestination( p );
		}

		// Make movement frame rate independence
		p.fx *= (30 / fps);
		p.fz *= (30 / fps);
		float vel = sqrtf( p.fx * p.fx + p.fz * p.fz );
		
		// Set new position
		p.px += p.fx; p.pz += p.fz;
		
		// Caluclate orientation
		p.ox = (p.ox + p.fx) / 2;
		p.oz = (p.oz + p.fz) / 2;

		// Get rotation from orientation
		float ry = 0;
		if( p.oz != 0 ) ry = atan2( p.ox, p.oz );
		ry *= 180 / 3.1415f;	// Convert from radians to degrees
		
		// Update character scene node position
		Horde3D::setNodeTransform( p.node, p.px, 0.02f, p.pz, 0, ry, 0, 1, 1, 1 );
		
		// Update animation
		p.animTime += vel * 35.0f;
		Horde3D::setModelAnimParams( p.node, 0, p.animTime, 1.0f );
	}
}

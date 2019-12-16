// *************************************************************************************************
// Horde3D Shader Utility Library
// --------------------------------------
//		- Particle functions -
//
// Copyright (C) 2006-2008 Nicolas Schulz
//
// You may use the following code in projects based on the Horde3D graphics engine.
//
// *************************************************************************************************

uniform 	vec3 parCorners[4];
uniform 	vec3 parPosArray[64];
uniform 	vec2 parSizeAndRotArray[64];
uniform 	vec4 parColorArray[64];

attribute 	float parCornerIdx;
attribute 	float parIdx;


vec4 getParticleColor()
{
	return parColorArray[int( parIdx )];
}

vec4 calcParticleViewPos()
{
	// Position
	vec3 pos = gl_Vertex.xyz + parPosArray[int( parIdx )] +
			   parCorners[int( parCornerIdx )] * parSizeAndRotArray[int( parIdx )].x;
	
	// Rotation
	float s = sin( parSizeAndRotArray[int( parIdx )].y * 0.0174532925 );
	float c = cos( parSizeAndRotArray[int( parIdx )].y * 0.0174532925 );
	mat4 rotMat = mat4( c, -s, 0, 0,
						s,  c, 0, 0,
						0,  0, 1, 0,
						0,  0, 0, 1 );
	vec4 mid = gl_ModelViewMatrix * vec4( parPosArray[int( parIdx )], 1 );
	vec4 pos2 = gl_ModelViewMatrix * vec4( pos, 1 );
	return rotMat * (pos2 - mid) + mid;
}
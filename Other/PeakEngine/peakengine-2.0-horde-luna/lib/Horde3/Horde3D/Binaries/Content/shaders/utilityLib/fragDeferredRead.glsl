// *************************************************************************************************
// Horde3D Shader Utility Library
// --------------------------------------
//		- Deferred shading functions -
//
// Copyright (C) 2006-2008 Nicolas Schulz
//
// You may use the following code in projects based on the Horde3D graphics engine.
//
// *************************************************************************************************

uniform 	sampler2D tex8;
uniform 	sampler2D tex9;
uniform 	sampler2D tex10;

float getMatID( const vec2 coord ) { return texture2D( tex8, coord ).a; }
vec3 getPos( const vec2 coord ) { return texture2D( tex8, coord ).rgb; }
vec3 getNormal( const vec2 coord ) { return texture2D( tex9, coord ).rgb; }
vec3 getAlbedo( const vec2 coord ) { return texture2D( tex10, coord ).rgb; }
float getSpecMask( const vec2 coord ) { return texture2D( tex10, coord ).a; }
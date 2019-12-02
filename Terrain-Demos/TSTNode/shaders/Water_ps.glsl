/*
 * Copyright (c) 2007, elvman
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY elvman ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <copyright holder> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

uniform vec3		CameraPosition;  // Camera position
uniform float		WaveHeight;

vec4				WaterColor;
uniform float		ColorBlendFactor;

uniform sampler2D	WaterBump; //coverage
uniform sampler2D	RefractionMap; //lightmap
uniform sampler2D	ReflectionMap;

void main( void ) 
{	
	//bump color
	vec4 bumpColor = texture2D(WaterBump, vec2(gl_TexCoord[0]));
	vec2 perturbation = WaveHeight*(bumpColor.rg - 0.5f);
	
	//refraction
	vec2 ProjectedRefractionTexCoords;
	ProjectedRefractionTexCoords.x = gl_TexCoord[1].x/gl_TexCoord[1].w/2.0f + 0.5f;
	ProjectedRefractionTexCoords.y = -gl_TexCoord[1].y/gl_TexCoord[1].w/2.0f + 0.5f;
	
	vec4 refractiveColor = texture2D(RefractionMap, ProjectedRefractionTexCoords + perturbation );
	
	//reflection
	vec2 ProjectedReflectionTexCoords;
    ProjectedReflectionTexCoords.x = gl_TexCoord[2].x/gl_TexCoord[2].w/2.0f + 0.5f;
    ProjectedReflectionTexCoords.y = -gl_TexCoord[2].y/gl_TexCoord[2].w/2.0f + 0.5f;
	
	vec4 reflectiveColor = texture2D(ReflectionMap, ProjectedReflectionTexCoords + perturbation );

	//fresnel
	vec3 eyeVector = normalize(CameraPosition - vec3(gl_TexCoord[3]));
	vec3 normalVector = vec3(0,1,0);
	
	float fresnelTerm = max( dot(eyeVector, normalVector), 0.0f );
	
	
	vec4 combinedColor = refractiveColor*fresnelTerm + reflectiveColor*(1-fresnelTerm);
	
	gl_FragColor= = ColorBlendFactor*WaterColor + (1-ColorBlendFactor)*combinedColor;
}


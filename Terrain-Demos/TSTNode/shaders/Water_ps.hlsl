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

float3		CameraPosition;  // Camera position
float		WaveHeight;

float4		WaterColor;
float		ColorBlendFactor;

sampler2D	WaterBump; //coverage
sampler2D	RefractionMap; //coverage
sampler2D	ReflectionMap; //coverage

// Pixel shader output structure
struct PS_OUTPUT
{
    float4 Color : COLOR0;  // Pixel color    
};

struct PS_INPUT
{
	float4 Position					: POSITION;   // vertex position
	
	float2 BumpMapTexCoord			: TEXCOORD0;
	float3 RefractionMapTexCoord	: TEXCOORD1;
	float3 ReflectionMapTexCoord	: TEXCOORD2;
		
	float3 Position3D				: TEXCOORD3;
};
	
PS_OUTPUT main( PS_INPUT Input ) 
{ 
	PS_OUTPUT Output;
	
	//bump color
	float4 bumpColor = tex2D(WaterBump, Input.BumpMapTexCoord);
	float2 perturbation = WaveHeight*(bumpColor.rg - 0.5f);
	
	//refraction
	float2 ProjectedRefractionTexCoords=saturate(Input.RefractionMapTexCoord.xy/Input.RefractionMapTexCoord.z + perturbation);
	float4 refractiveColor = tex2D(RefractionMap, ProjectedRefractionTexCoords );
	
	//reflection
	float2 ProjectedReflectionTexCoords=saturate(Input.ReflectionMapTexCoord.xy/Input.ReflectionMapTexCoord.z + perturbation);
	float4 reflectiveColor = tex2D(ReflectionMap, ProjectedReflectionTexCoords );

	//fresnel
	float3 eyeVector = normalize(CameraPosition - Input.Position3D);
	float3 normalVector = float3(0,1,0);
	
	float fresnelTerm = max( dot(eyeVector, normalVector), 0.0f );
	
	float4 combinedColor = refractiveColor*fresnelTerm + reflectiveColor*(1-fresnelTerm);
	
	Output.Color = ColorBlendFactor*WaterColor + (1-ColorBlendFactor)*combinedColor;

	return Output;
}


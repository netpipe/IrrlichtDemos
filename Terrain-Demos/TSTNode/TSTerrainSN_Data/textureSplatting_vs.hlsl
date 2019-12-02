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

extern float4x4 WorldViewProj;  // World * View * Projection transformation


struct VS_INPUT
{
	float4 Position   : POSITION;   // vertex position 
	
	float4 normal	  : NORMAL;
	
	float2 DetailMapCoord			: TEXCOORD0;
	float2 TextureOneCoord			: TEXCOORD1;
	float2 TextureTwoCoord			: TEXCOORD2;
	float2 TextureThreeCoord		: TEXCOORD3;
	float2 TextureFourCoord			: TEXCOORD4;
	float2 LightMapCoord			: TEXCOORD5;
	float2 ColorMapCoord			: TEXCOORD6;
	
};

// Vertex shader output structure
struct VS_OUTPUT
{
	float4 Position					: POSITION;   // vertex position 
	
	float2 DetailMapCoord			: TEXCOORD0;
	float2 TextureOneCoord			: TEXCOORD1;
	float2 TextureTwoCoord			: TEXCOORD2;
	float2 TextureThreeCoord		: TEXCOORD3;
	float2 TextureFourCoord			: TEXCOORD4;
	float2 LightMapCoord			: TEXCOORD5;
	float2 ColorMapCoord			: TEXCOORD6;
};


VS_OUTPUT main( VS_INPUT Input )
{
	VS_OUTPUT Output;

	// transform position to clip space 
	Output.Position = mul(Input.Position, WorldViewProj);

	Output.DetailMapCoord		= Input.DetailMapCoord;
	Output.TextureOneCoord		= Input.TextureOneCoord;
	Output.TextureTwoCoord		= Input.TextureTwoCoord;
	Output.TextureThreeCoord	= Input.TextureThreeCoord;
	Output.TextureFourCoord		= Input.TextureFourCoord;
	Output.LightMapCoord		= Input.LightMapCoord;
	Output.ColorMapCoord		= Input.ColorMapCoord;
	
	return Output;
}
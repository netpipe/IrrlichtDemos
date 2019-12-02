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

sampler2D	DetailMap;
sampler2D	TextureOne;
sampler2D	TextureTwo;
sampler2D	TextureThree;
sampler2D	TextureFour;
sampler2D	LightMap;
sampler2D	ColorMap;

// Pixel shader output structure
struct PS_OUTPUT
{
    float4 Color : COLOR0;  // Pixel color    
};

struct PS_INPUT
{
	float3 Position					: POSITION;   // vertex position

	float2 DetailMapCoord			: TEXCOORD0;
	float2 TextureOneCoord			: TEXCOORD1;
	float2 TextureTwoCoord			: TEXCOORD2;
	float2 TextureThreeCoord		: TEXCOORD3;
	float2 TextureFourCoord			: TEXCOORD4;
	float2 LightMapCoord			: TEXCOORD5;
	float2 ColorMapCoord			: TEXCOORD6;
};

PS_OUTPUT main( PS_INPUT Input ) 
{ 
	PS_OUTPUT Output;

	float4 detailMapColor	= tex2D(DetailMap, Input.DetailMapCoord);
	
	float4 textOneColor		= tex2D(TextureOne,		Input.TextureOneCoord);
	float4 textTwoColor		= tex2D(TextureTwo,		Input.TextureTwoCoord);
	float4 textThreeColor	= tex2D(TextureThree,	Input.TextureThreeCoord);
	float4 textFourColor	= tex2D(TextureFour,	Input.TextureFourCoord);
	
	float4 lightMapColor	= tex2D(LightMap,		Input.LightMapCoord);
	float4 colorMapColor	= tex2D(ColorMap,		Input.ColorMapCoord);
	
	textOneColor	= textOneColor		* detailMapColor.x;
	textTwoColor	= textOneColor		+ detailMapColor.y * (textTwoColor		- textOneColor);
	textThreeColor	= textTwoColor		+ detailMapColor.z * (textThreeColor	- textTwoColor);
	textFourColor	= textThreeColor	+ detailMapColor.w * (textFourColor		- textThreeColor);
	
	colorMapColor	= colorMapColor * 1.6;
	lightMapColor	= lightMapColor + 0.2;
	
	//Output.Color	= textFourColor * lightMapColor - colorMapColor;
	Output.Color	= textFourColor * lightMapColor * colorMapColor;

	return Output;
}


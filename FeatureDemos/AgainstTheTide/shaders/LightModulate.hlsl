sampler2D ColorMapSampler : register(s0);
sampler2D ScreenMapSampler : register(s1);

float4 pixelMain(float2 TexCoords : TEXCOORD0) : COLOR0
{		
	float4 finalCol = tex2D(ColorMapSampler, TexCoords);
	float4 lightCol = tex2D(ScreenMapSampler, TexCoords);

	return finalCol * lightCol;
}
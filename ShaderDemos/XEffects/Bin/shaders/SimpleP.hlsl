sampler2D ColorMapSampler : register(s0);

float4 pixelMain(float2 TexCoords : TEXCOORD0) : COLOR0
{		
	float4 finalCol = tex2D(ColorMapSampler, TexCoords);
	return finalCol;
}
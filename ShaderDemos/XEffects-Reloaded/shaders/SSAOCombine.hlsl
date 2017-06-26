sampler2D ColorMapSampler : register(s0);
sampler2D ScreenMapSampler : register(s1);

float4 pixelMain(float2 TexCoords : TEXCOORD0) : COLOR0
{		
	float4 screenCol = tex2D(ScreenMapSampler, TexCoords);
	float4 SSAOCol = tex2D(ColorMapSampler, TexCoords);

	return(screenCol * SSAOCol);
}
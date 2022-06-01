sampler2D ColorMapSampler : register(s0);
sampler2D ScreenMapSampler : register(s1);
sampler2D DepthMapSampler : register(s2);

float luminance(float4 c)
{
	return dot( c, float4(0.3, 0.59, 0.11, 0.0) );
}

float4 pixelMain(float2 TexCoords : TEXCOORD0) : COLOR0
{		
float4 finalCol = tex2D(ColorMapSampler,TexCoords);
float4 blurredCol = tex2D(ScreenMapSampler,TexCoords);

return ( finalCol + blurredCol * 1.8 ) / 2 +
				luminance(blurredCol) * 
				float4( 0.5, 0.5, 0.5, 0) +
				luminance(finalCol) *
				float4( 0.3, 0.3, 0.3, 0);
}
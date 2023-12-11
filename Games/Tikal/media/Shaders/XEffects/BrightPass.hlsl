sampler2D ColorMapSampler : register(s0);

float4 pixelMain ( float4 Texcoords : TEXCOORD0 ) : COLOR0
{
	float4 finalVal = tex2D(ColorMapSampler, Texcoords.xy);
	finalVal *= 2.0;
	finalVal = pow(finalVal, 8.0);

	return finalVal;
}
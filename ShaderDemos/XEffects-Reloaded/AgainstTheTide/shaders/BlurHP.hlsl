sampler2D ColorMapSampler : register(s0);

float4 pixelMain ( float4 Texcoords : TEXCOORD0 ) : COLOR0
{
	const float2 offsetArray[5] = 
	{
		float2(0,0),
		float2( 1.0 / SCREENX,0),
		float2( -1.0 / SCREENX,0),
		float2( 2.0 / SCREENX,0),
		float2( -2.0 / SCREENX,0)
	};

	float4 finalVal = float4(0.0, 0.0, 0.0, 0.0);

	for(int i = 0;i < 5;++i)
	{
		float4 interVal = tex2D(ColorMapSampler,Texcoords + offsetArray[i] * 2.33);
		finalVal += interVal; 
	}

	finalVal /= 5.0f;
	return finalVal;
}
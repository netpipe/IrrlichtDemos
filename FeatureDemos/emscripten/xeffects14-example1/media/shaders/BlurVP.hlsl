sampler2D ColorMapSampler : register(s0);

float4 pixelMain ( float4 Texcoords : TEXCOORD0 ) : COLOR0
{
	const float2 offsetArray[4] = 
	{
		float2(0, 1.0 / SCREENY),
		float2(0, -1.0 / SCREENY),
		float2(0, 2.0 / SCREENY),
		float2(0, -2.0 / SCREENY)
	};

	float4 finalVal = float4(0.0, 0.0, 0.0, 0.0);

	for(int i = 0;i < 4;++i)
		finalVal += tex2D(ColorMapSampler, clamp(Texcoords.xy + offsetArray[i] * 2.25, float2(0.0, 0.01), float2(1.0, 1.0)));

	finalVal /= 4.0f;
	return finalVal;
}
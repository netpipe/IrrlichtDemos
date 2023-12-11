sampler2D ColorMapSampler : register(s0);

float4 pixelMain(float4 Color: TEXCOORD0, float2 Texcoords: TEXCOORD1) : COLOR0
{
	float alpha = tex2D(ColorMapSampler, Texcoords).a;

	return float4(1.0, 1.0, 1.0, alpha);
}
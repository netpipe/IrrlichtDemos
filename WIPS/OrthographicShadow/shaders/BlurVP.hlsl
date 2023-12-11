sampler2D ColorMapSampler : register(s0);

float4 pixelMain ( float4 Texcoords : TEXCOORD0 ) : COLOR0
{

const float2 offsetArray[5] = 
{
	float2(0,0),
	float2(0, 1.0 / SCREENY),
	float2(0, -1.0 / SCREENY),
	float2(0, 2.0 / SCREENY),
	float2(0, -2.0 / SCREENY)
};

float4 finalVal = 0;

for(int i = 0;i < 5;++i)
{
	float4 interVal = tex2D(ColorMapSampler,Texcoords + offsetArray[i] * 2.5);
	finalVal += interVal; 
}

finalVal /= 5.0f;
return finalVal;
}

float4 pixelMain(
	float4 Position: POSITION0,
	float2 UV	: TEXCOORD0
	) : COLOR0
{
    float sv = 5.8*pow(max(0,UV.x),16);
    float tv = max(0,(UV.y));
    // return float4(sv.xxxx);
    return float4(sv*lerp(float3(1,1,1),float3(1,.7,.5),tv),1);

}
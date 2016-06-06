
sampler2D tex0 : register(s0) ;

float4 pixelMain(
	float2 UV	: TEXCOORD0,
	float4 Color: COLOR0,
	float4 specCol	: COLOR1
	) : COLOR0
{
	 float4 final = Color * tex2D(tex0, UV) + specCol;
    return(final);
 
}

sampler2D hmapSampler : register(s0) ;
sampler2D nmapSampler : register(s1) ;

float4 pixelMain(
	float2 HalfAngleTerms	: TEXCOORD0, // dot prods against half-angle
    float2 NormalAngleTerms	: TEXCOORD1, // dot-prods against normal
	float2 UV	: TEXCOORD0,
	float4 diffCol: COLOR0
	) : COLOR0
{
	float4 ht = tex2D(hmapSampler,HalfAngleTerms);
	float4 nt = tex2D(nmapSampler,NormalAngleTerms);
    float4 nspec = ht * nt * float4(1.0f, 1.0f, 1.0f, 1.0f);
    //return (nspec);
    return (diffCol + nspec);
}
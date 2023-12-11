

float4 pixelMain(
	float UV	: TEXCOORD0
	) : COLOR0
{
	float depth = UV;
	float mulDepth = depth * 256.0f;
	float flooredDepth = floor(mulDepth);
	float4 finalCol = float4(flooredDepth / 256.0f, (mulDepth - flooredDepth),0,0);
	return(finalCol);
}
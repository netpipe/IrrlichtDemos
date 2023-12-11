float MaxD;	

float4 pixelMain
(
float4 Color: TEXCOORD0
) : COLOR0
{
	float depth = Color.z / MaxD;
	float mulDepth = depth * 256.0f;
	float flooredDepth = floor(mulDepth);
	float4 finalCol = float4(flooredDepth / 256.0f, (mulDepth - flooredDepth),0,0);
	return(finalCol);
}
float4 pixelMain(float4 Color: TEXCOORD0) : COLOR0
{
##ifdef LENGTH_BASED_DEPTH
	float depth = length(Color.xyz) / Color.w;
##else
	float depth = Color.z / Color.w;
##endif
	float mulDepth = depth * 256.0;
	float flooredDepth = floor(mulDepth);
	
	return float4(flooredDepth / 256.0, (mulDepth - flooredDepth), 0.0, 0.0);
}
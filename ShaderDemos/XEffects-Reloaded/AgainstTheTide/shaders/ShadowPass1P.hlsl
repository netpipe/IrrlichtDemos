float4 pixelMain(float4 Color: TEXCOORD0) : COLOR0
{
	float depth = Color.z / Color.x;
	float mulDepth = depth * 256.0 + 1.0;
	float flooredDepth = floor(mulDepth);
	
	//return float4(flooredDepth / 256.0, (mulDepth - flooredDepth), 0.0, 0.0);
	return float4(depth, depth * depth, 0.0, 0.0);
}
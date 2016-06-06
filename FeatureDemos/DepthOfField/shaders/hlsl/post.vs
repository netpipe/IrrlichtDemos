//
// Post-process Vertex Shader
//

struct VS_OUT
{
	float4 pos : POSITION;
	float2 vTexCoord : TEXCOORD0;
};

VS_OUT vertexMain(in float3 gl_Vertex : POSITION)
{
	VS_OUT Out;
	// Clean up inaccuracies
	float2 Pos = sign(gl_Vertex.xy);

	Out.pos = float4(Pos.x, -Pos.y, 0, 1); // Invert Y for Direct3D

	// Image-space
	Out.vTexCoord.x = 0.5 * (1.0 + Pos.x);
	Out.vTexCoord.y = 0.5 * (1.0 + Pos.y);
	return Out;
}

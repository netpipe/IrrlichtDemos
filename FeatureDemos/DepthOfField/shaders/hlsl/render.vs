
float4x4 gl_ModelViewProjectionMatrix;
float4x4 gl_ModelViewMatrix;
float distanceScale;

struct VS_OUT
{
	float4 gl_Position : POSITION;
	float3 vViewVec : TEXCOORD0;
};

VS_OUT vertexMain(in float3 gl_Vertex : POSITION)
{
	VS_OUT Out;

	Out.gl_Position = mul(float4(gl_Vertex,1), gl_ModelViewProjectionMatrix);
	float4 vec = -distanceScale * mul(gl_ModelViewMatrix, float4(gl_Vertex,1));
	Out.vViewVec = vec.xyz;

	return Out;
}
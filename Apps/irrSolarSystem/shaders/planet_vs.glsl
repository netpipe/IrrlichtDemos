float4x4 world; // IMPORTANT: PASS WORLD TRANSFORM TO SHADER USING driver->getTransform(ETS_WORLD);
float4x4 view_proj_matrix;
float4 view_position;
//float4x4 view_matrix; no need, I think...
//float4x4 inv_view_matrix; no need, I think

float4 Light_Ambient;

//Object
float spec_factor;

//Light1
float3 Light1_P;
//float4 Light1_C;

struct VS_OUTPUT
{
float4 Pos: POSITION;
float2 TexCoord: TEXCOORD0;
float3 Normal: TEXCOORD1;
float3 LightDir1: TEXCOORD2;
float3 HalfVec1: TEXCOORD3;
};

VS_OUTPUT vs_main(float4 inPos: POSITION,
float3 inNormal: NORMAL,
float2 inTxr: TEXCOORD0,
float3 inTangent: TANGENT0,
float3 inBinormal: BINORMAL0)
{
VS_OUTPUT Out;

// Compute the projected position and send out the texture coordinates
Out.Pos = mul(inPos, view_proj_matrix);
Out.TexCoord = inTxr;

// Calculate WORLD position.
float3 wPos = (mul(inPos, world)).xyz;

// Move the normal to the pixel shader
Out.Normal = inNormal;

// Use only WORLD position in calculations here!!!
Out.LightDir1 = normalize(Light1_P - wPos );

Out.HalfVec1 = normalize(Out.LightDir1 -(-normalize(view_position-wPos )));

return Out;
}
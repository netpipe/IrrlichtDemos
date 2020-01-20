float4x4 mWorldViewProj;
float3 mLightPos;
float3 mCamPos;

struct VS_OUTPUT
{
   float4 Position: POSITION0;
   float2 TexCoord: TEXCOORD0;
   float3 Normal: 	TEXCOORD1;
   float3 LightDir: TEXCOORD2;
   float3 EyeDir: TEXCOORD3;
};


VS_OUTPUT vertexMain(
   float4 Position: POSITION0,
   float2 TexCoord: TEXCOORD0,
   float2 diffuse: TEXCOORD1,
   float3 Normal:   NORMAL0)
{
   VS_OUTPUT Output;

   Output.Position = mul(Position, mWorldViewProj);
   Output.Normal = normalize(Normal);
   Output.LightDir = normalize(mLightPos - Position.xyz);
   Output.EyeDir = normalize(mCamPos - Position.xyz);
   Output.TexCoord = TexCoord;

   return(Output);
}
   
sampler2D tex0 : register(s0);
sampler1D diffuseRamp : register(s1);

float4 pixelMain
(
   float2 TexCoord:     TEXCOORD0,
   float3 Normal:       TEXCOORD1,
   float3 LightDir:     TEXCOORD2,
   float3 EyeDir:     	TEXCOORD3
) : COLOR0
{
	float4 texCol = tex2D(tex0, TexCoord);

	float diffuse = clamp(dot(Normal, LightDir), 0.0, 1.0);
	
	float4 lightColor = tex1D(diffuseRamp, diffuse);

	float fresnal = dot(Normal, EyeDir);
	fresnal = clamp((fresnal - 0.2) * 1000.0, 0.0, 1.0);
	lightColor *= fresnal;
	
	float3 reflect = (2.0 * diffuse * Normal) - LightDir;
	float specular = pow(clamp(dot(reflect, EyeDir), 0.0, 1.0), 8.0);
	specular = clamp((specular - 0.5) * 1000.0, 0.0, 1.0);
	float4 specular4 = specular * float4(1.0, 1.0, 1.0, 1.0);
	
	return(saturate(lightColor * texCol + specular4));
} 
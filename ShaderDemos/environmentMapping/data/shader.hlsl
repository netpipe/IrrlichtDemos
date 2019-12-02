/*Shader code for DirectX*/

float4x4 matWorldViewProjection;
float4x4 matViewInverse;
float4x4 matWorld;
sampler2D baseMap;
sampler2D reflectionMap;
float mipsCount;

//We're not doing normal map, not this time...

struct vertexIN
{
   float4 Position : POSITION0;
   float2 Texcoord : TEXCOORD0;
   float3 Normal :   NORMAL0;
};

struct vertexOUT
{
	float4 position : POSITION0;
	float2 texcoord : TEXCOORD0;
	float3 normal : TEXCOORD1;
	float3 viewDirection: TEXCOORD2;
};

vertexOUT vs_main(vertexIN IN)
{
	vertexOUT OUT;
	OUT.position = mul(IN.Position,matWorldViewProjection);
	OUT.normal = mul(float4(IN.Normal,0),matWorld).xyz;
	OUT.viewDirection = mul(float4(OUT.position.xyz,0),matViewInverse).xyz;
	OUT.texcoord = IN.Texcoord;
	return OUT;
}

struct pixelIN
{
	float2 texcoord : TEXCOORD0;
	float3 normal : TEXCOORD1;
	float3 viewDirection: TEXCOORD2;
};

//This is the code to sample a sphere map;

float2 sphere2uv(float3 vec)
{
   float2 o;
   float PI = 3.14159265358979323846;
   //Actually this differs from the original sources in the signs of the vec, this way we get actually a reflected result :)
   o.x = 0.5+atan2(-vec.z,-vec.x)/(2.0*PI);
   o.y = 0.5-asin(vec.y)/PI;
   return o;
}

float4 ps_main(pixelIN IN):color0
{
	float3 viewDir,normal;
	viewDir = normalize(IN.viewDirection);
	normal = normalize(IN.normal);
	float3 reflection;
	reflection = reflect(viewDir,normal);
	float4 gloss = tex2D(baseMap,IN.texcoord);
	float4 uv = float4(sphere2uv(reflection),0,(1-gloss.y)*mipsCount);
	return tex2Dlod(reflectionMap,uv);
}
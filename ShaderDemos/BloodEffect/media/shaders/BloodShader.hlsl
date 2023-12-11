float4x4 mWorldViewProj;

struct VS_OUTPUT
{
  float4 Position   : POSITION0;
  float2 TexCoords  : TEXCOORD0;
};

VS_OUTPUT vertexMain( float3 Position  : POSITION0,
                      float2 TexCoords : TEXCOORD0 )
{
  VS_OUTPUT OUT;

  OUT.Position  = mul(float4(Position.x, Position.y, Position.z, 1.0), mWorldViewProj);
  OUT.TexCoords = TexCoords;

  return(OUT);
}

// ----------------------------------------------------------------------------------------------------

sampler2D blood : register(s0);

float time;

float4 pixelMain( VS_OUTPUT IN ) : COLOR0
{
  IN.TexCoords.x += sin(25 * IN.TexCoords.x + time / 100) / 40;
  IN.TexCoords.y -= sin(25 * IN.TexCoords.y + time / 100) / 40;

  float4 final = tex2D(blood, IN.TexCoords.xy);

  return final;
};
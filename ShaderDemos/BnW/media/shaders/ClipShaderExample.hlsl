float4x4 xWorldViewProjection;
float4x4 xWorld;

struct VertexToPixel
{
    float4 Position   : POSITION0;
    float2 TexCoords  : TEXCOORD0;
    float3 Position3D : TEXCOORD1;
};

struct PixelToFrame
{
    float4 Color      : COLOR0;
};

VertexToPixel vertexMain ( float4 inPos       : POSITION0,
                           float2 inTexCoords : TEXCOORD0 )
{
    VertexToPixel Output;

    Output.Position   = mul(inPos, xWorldViewProjection);
    Output.TexCoords  = inTexCoords;
    Output.Position3D = mul(inPos, xWorld);

    return Output;
}

// ==============================================================

sampler2D diffuse : register(s0);
float seaLevel; // SceneNode Y position

PixelToFrame pixelMain(VertexToPixel PSIn)
{
    clip(PSIn.Position3D.y - seaLevel); // clip if below seaLevel

    PixelToFrame Output;

    float4 final = tex2D(diffuse, PSIn.TexCoords);

    Output.Color = final;

    return Output;
}
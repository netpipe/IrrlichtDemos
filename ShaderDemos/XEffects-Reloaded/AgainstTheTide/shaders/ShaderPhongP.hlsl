
sampler2D tex0 : register(s0) ;

float4 pixelMain(float3 Light: TEXCOORD0, float3 Norm : TEXCOORD1,
        float3 View : TEXCOORD2, float2 TexCo : TEXCOORD3) : COLOR
{
    float4 diffuse = tex2D(tex0,TexCo);

    float4 diff = saturate(dot(Norm, Light)); // diffuse component

    // R = 2 * (N.L) * N - L
    float3 Reflect = (2 * diff * Norm - Light);
    float4 specular = pow(saturate(dot(Reflect, View)), 8); // R.V^n

    // I = Acolor + Dcolor * N.L + (R.V)n
	return(diffuse * diff + specular);
}
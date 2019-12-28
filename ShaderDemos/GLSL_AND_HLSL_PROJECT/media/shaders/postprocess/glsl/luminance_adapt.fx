uniform sampler2D texture0; // PrevLumBuffer
uniform sampler2D texture1; // TargetLumBuffer
uniform float EffectStrength;

float saturate(float x)
{
 return (x < 0.0) ? 0.0 : (1.0 < x) ? 1.0 : x;
}
 
vec2 saturate(vec2 x)
{
 return vec2(saturate(x[0]),saturate(x[1]));
}
 
vec3 saturate(vec3 x)
{
 return vec3(saturate(x[0]),saturate(x[1]),saturate(x[2]));
}
 
vec4 saturate(vec4 x)
{
 return vec4(saturate(x[0]),saturate(x[1]),saturate(x[2]),saturate(x[3]));
}

void main()
{
	vec4 prevLum = texture2D(texture0, gl_TexCoord[0].xy);
	vec4 targetLum = texture2D(texture1, gl_TexCoord[0].xy);
	gl_FragColor = saturate(prevLum+(targetLum-prevLum)*EffectStrength);
}

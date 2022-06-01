#define REALISTIC_COLORS
#define WAVE_CRESTS

#ifdef REALISTIC_COLORS
	#define SPECULAR_COLOR vec4(0.8, 0.8, 0.8, 1.0)
	#define WATER_COLOR1 vec4(0.0, 0.0, 0.0, 1.0)
	#define WATER_COLOR2 vec4(0.125, 0.125, 0.25, 1.0)
#else
	#define SPECULAR_COLOR vec4(0.8, 0.8, 0.8, 1.0)
	#define WATER_COLOR1 vec4(0.0, 0.6, 0.7, 1.0)
	#define WATER_COLOR2 vec4(0.0, 0.8, 0.9, 1.0)
#endif

#define FRESNAL_COLOR vec4(0.0, 0.0, 0.0, 1.0)
#define SKY_COLOR vec4(0.125, 0.125, 0.25, 1.0)

uniform sampler2D NormalMap;

varying vec3 Normal, Tangent, Binormal, View, WPos;
varying vec2 TexCoords;
varying float FadeFactor, Height;

void main()
{
	vec4 ParMap = texture2D(NormalMap, TexCoords);

	vec3 tNorm = (ParMap.xyz - vec3(0.5,0.5,0.5));
	vec3 norm = normalize(tNorm.x * Tangent - tNorm.y * Binormal + tNorm.z * Normal);
	
	vec3 lDir = normalize(vec3(1.0, 1.0, 1.0));
		
	float lFact = dot(norm, lDir);
	vec4 dCol = mix(WATER_COLOR1, WATER_COLOR2, lFact);
		
	vec3 view = normalize(View);
	
	//float fresnal = dot(vec3(0.0, 1.0, 0.0), view);
	//dCol = mix(dCol, FRESNAL_COLOR, fresnal);
	
	vec3 reflect = normalize(2.0 * lFact * norm - lDir);
	vec4 specular = pow(clamp(dot(reflect, view), 0.0, 1.0), 16.0) * SPECULAR_COLOR;
	
	vec4 finalColor = dCol + specular;
	
#ifdef WAVE_CRESTS
	finalColor = mix(finalColor, vec4(1.0, 1.0, 1.0, 1.0) * clamp(ParMap.x * 2.0, 0.0, 1.0), clamp(Height / 10.0, 0.0, 1.0));
#endif
	
	finalColor = mix(finalColor, SKY_COLOR, FadeFactor);
	
	gl_FragColor = finalColor;
}
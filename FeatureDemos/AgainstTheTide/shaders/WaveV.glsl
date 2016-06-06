#define REAL_NOISE
#define WAVE_CRESTS

uniform float Time;
uniform vec3 CenterPos;
uniform vec3 CamPos;
uniform mat4 World;
uniform mat4 ViewProj;

varying float FadeFactor;
varying vec2 TCoords;

// Fast noise thanks to Ysaneya
vec4 randomizer4(const vec4 x)
{
    vec4 z = mod(x, vec4(5612.0));
    z = mod(z, vec4(3.1415927 * 2.0));
    return(fract(cos(z) * vec4(56812.5453)));
}

const float A = 1.0;
const float B = 57.0;
const float C = 113.0;
const vec3 ABC = vec3(A, B, C);
const vec4 A3 = vec4(0, B, C, C+B);
const vec4 A4 = vec4(A, A+B, C+A, C+A+B);

float cnoise4(const in vec3 xx)
{
    vec3 x = mod(xx + 32768.0, 65536.0);
    vec3 ix = floor(x);
    vec3 fx = fract(x);
    vec3 wx = fx*fx*(3.0-2.0*fx);
    float nn = dot(ix, ABC);

    vec4 N1 = nn + A3;
    vec4 N2 = nn + A4;
    vec4 R1 = randomizer4(N1);
    vec4 R2 = randomizer4(N2);
    vec4 R = mix(R1, R2, wx.x);
    float re = mix(mix(R.x, R.y, wx.y), mix(R.z, R.w, wx.y), wx.z);

    return 1.0 - 2.0 * re;
}

float calcVHeight(float x, float y)
{
#ifdef REAL_NOISE
	#ifdef WAVE_CRESTS
		// Can play around with noise inside the sin function.
		return 10.0 - abs(sin(x / 200.0 + Time)) * 80.0;// + 
		//cnoise4(vec3(x / 100.0 + Time / 2.0, Time / 2.0, y / 100.0 + Time / 2.0)) * 25.0; 
	#else
		return cnoise4(vec3(x / 100.0 + Time / 2.0, Time / 2.0, y / 100.0 + Time / 2.0)) * 25.0; 
	#endif
#else
	return (100.0 * sin(Time + x / 80.0 + Time + y / 40.0)) + (5.0 * sin(Time + y / 50.0));
#endif
}

vec3 getVAt(vec2 position)
{
	return vec3(position.x, calcVHeight(position.x, position.y), position.y);
}

void main()
{
	float timeFactor = fract(Time / 2.0 + gl_Normal.x);

	vec4 wPosition = World * gl_Vertex;

	wPosition.y = calcVHeight(wPosition.x, wPosition.z) - 10.0;
	
	vec3 dVec = normalize(wPosition.xyz - CenterPos);
	dVec.y = 1.0;
	dVec *= timeFactor * 100.0;	
	
	wPosition.xyz += dVec;
	
	vec2 TexCoords = gl_MultiTexCoord0.xy;
	vec3 CamDir = normalize(wPosition.xyz - CamPos);
	vec3 xD = cross(CamDir, vec3(0.0, 1.0, 0.0));
	vec3 yD = cross(CamDir, xD);
	
	vec3 pOffset = xD * TexCoords.y * 50.0 + yD * TexCoords.x * 50.0;
	wPosition.xyz += pOffset;
	
	gl_Position = ViewProj * wPosition;

	FadeFactor = timeFactor;
			
	TCoords = TexCoords;
}
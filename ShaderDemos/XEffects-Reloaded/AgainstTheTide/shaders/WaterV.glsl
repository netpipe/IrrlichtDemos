#define REAL_NOISE
#define NORMAL_CALC_OFFSET 20.0
#define WAVE_CRESTS

uniform float Time, TCoordAdjuster;
uniform mat4 World;
uniform mat4 ViewProj;
uniform vec3 CamPos;

varying vec3 Normal, Tangent, Binormal, View;
varying vec2 TexCoords;
varying float FadeFactor, Height;

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
		// Can play around with noise more inside the sin function.
		return 10.0 - abs(sin(x / 200.0 + Time)) * 80.0 + 
		cnoise4(vec3(x / 100.0 + Time / 2.0, Time / 2.0, y / 100.0 + Time / 2.0)) * 25.0; 
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
	vec4 wPosition = World * gl_Vertex;

	wPosition.y = calcVHeight(wPosition.x, wPosition.z);
	
	/*
	// Normal calc V1 (Best quality)
	vec3 v0 = getVAt(wPosition.x - NORMAL_CALC_OFFSET, wPosition.z) - wPosition.xyz;
	vec3 v1 = getVAt(wPosition.x - NORMAL_CALC_OFFSET, wPosition.z - NORMAL_CALC_OFFSET) - wPosition.xyz;
	vec3 n0 = cross(v1, v0);

	v0 = v1;
	v1 = getVAt(wPosition.x, wPosition.z - NORMAL_CALC_OFFSET) - wPosition.xyz;
	vec3 n1 = cross(v1, v0);

	v0 = v1;
	v1 = getVAt(wPosition.x + NORMAL_CALC_OFFSET, wPosition.z) - wPosition.xyz;
	vec3 n2 = cross(v1, v0);

	v0 = v1;
	v1 = getVAt(wPosition.x + NORMAL_CALC_OFFSET, wPosition.z + NORMAL_CALC_OFFSET) - wPosition.xyz;
	vec3 n3 = cross(v1, v0);

	v0 = v1;
	v1 = getVAt(wPosition.x, wPosition.z + NORMAL_CALC_OFFSET) - wPosition.xyz;
	vec3 n4 = cross(v1, v0);

	v0 = v1;
	v1 = getVAt(wPosition.x - NORMAL_CALC_OFFSET, wPosition.z) - wPosition.xyz;
	vec3 n5 = cross(v1, v0);

	vec3 m0 = (n1 + n0) * 0.5;
	vec3 m2 = (n4 + n3) * 0.5;

	vec3 k0 = (m2 + m0) * 0.5;
	vec3 k1 = (n5 + n2) * 0.5;

	vec3 normal = (k1 + k0) * 0.5;	
	*/
	
	/*
	// Normal calc V2 (Medium quality), like V3 but offsets the lookup 
	// so that the interpolated normal is centered on the target vertex.
	// (Unfortunately this requires another noise lookup)
	vec2 offS = vec2(NORMAL_CALC_OFFSET / 2.0, NORMAL_CALC_OFFSET / 2.0);
	vec3 vi = getVAt(vec2(wPosition.x, wPosition.z) - offS);
	vec3 v0 = getVAt(vec2(vi.x + NORMAL_CALC_OFFSET, vi.z)) - vi;
	vec3 v1 = getVAt(vec2(vi.x, vi.z + NORMAL_CALC_OFFSET)) - vi;
	vec3 normal = cross(v1, v0);
	*/

	// Normal calc V3 (Low quality)
	vec3 v0 = getVAt(vec2(wPosition.x + NORMAL_CALC_OFFSET, wPosition.z)) - wPosition.xyz;
	vec3 v1 = getVAt(vec2(wPosition.x, wPosition.z + NORMAL_CALC_OFFSET)) - wPosition.xyz;
	vec3 normal = cross(v1, v0);

	View = CamPos - wPosition.xyz;

	gl_Position = ViewProj * wPosition;
	
	Height = wPosition.y;
	
	FadeFactor = clamp((gl_Position.z - 100.0 * TCoordAdjuster) / (100.0 * TCoordAdjuster), 0.0, 1.0);
	
	Normal = normalize(normal);
	
	vec3 tempTangent = vec3(abs(Normal.y) + abs(Normal.z), abs(Normal.x), 0.0);
	Tangent = tempTangent;
	Binormal = cross(tempTangent, Normal);
		
	TexCoords = (wPosition.xz / 2000.0) * TCoordAdjuster + vec2(Time / 10.0, Time / 10.0);
}
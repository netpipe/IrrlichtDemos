#define NORMAL_CALC_OFFSET 1.0

uniform float Time;

varying vec3 Normal;

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
	return cnoise4(vec3(x / 100.0 + Time / 2.0, Time / 2.0, y / 100.0 + Time / 2.0)) * 25.0; 
}

vec3 getVAt(vec3 pos)
{
	return vec3(pos.x, pos.y, pos.z + calcVHeight(pos.x * 50.0, pos.y * 50.0) / 50.0);
}

void main()
{
	vec4 position = gl_Vertex;
	position.z += calcVHeight(gl_Vertex.x * 50.0, gl_Vertex.y * 50.0) / 50.0;
	
	// Normal calc V3 (Low quality)
	vec3 v0 = getVAt(vec3(gl_Vertex.x + NORMAL_CALC_OFFSET, gl_Vertex.y, gl_Vertex.z)) - position.xyz;
	vec3 v1 = getVAt(vec3(gl_Vertex.x, gl_Vertex.y + NORMAL_CALC_OFFSET, gl_Vertex.z)) - position.xyz;
	Normal = (cross(v0, v1));
	
	position = gl_ModelViewProjectionMatrix * position;

	gl_Position = position;
}
// nvidia shader library
// http://developer.download.nvidia.com/shaderlibrary/webpages/shader_library.html

uniform float Sharpness;
uniform float Shake;
uniform float RandomValue;

void main()
{
	vec2 position;
	position.xy = sign(gl_Vertex.xy);
	gl_Position = vec4(position.xy, 0.0, 1.0);
	
	position.xy *= -1.0;
	gl_TexCoord[0].st = 0.5*(1.0-position.xy);
	
    vec2 i = Shake*vec2(RandomValue, RandomValue);
    vec2 j = abs(i);
    vec2 pj = vec2(pow(float(j), Sharpness));
    vec2 si = sign(i);
    vec2 shook = si * pj;
	gl_TexCoord[0].st -= shook;
}

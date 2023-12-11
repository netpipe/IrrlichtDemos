uniform vec4 CamPos;
uniform vec4 LightPos;
uniform mat4 World;
uniform mat4 mWorldViewProj;

varying vec3 Light;
varying vec3 Norm;
varying vec3 View;
varying vec2 TexCo;

void main()
{
    gl_Position = mWorldViewProj * gl_Vertex;
    Light = normalize(vec3(LightPos) - gl_Vertex.xyz);
	vec3 PosWorld = vec4(normalize((World * gl_Vertex))).xyz;
    View = normalize(CamPos.xyz - PosWorld);
    Norm = normalize(gl_Normal.xyz);
    TexCo = gl_MultiTexCoord0.xy;
}
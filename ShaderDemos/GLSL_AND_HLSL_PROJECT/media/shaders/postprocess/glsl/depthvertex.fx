uniform mat4 WorldMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjMatrix;
uniform float DistanceScale;

varying vec4 DepthNormal;

void main()
{
    gl_Position = ProjMatrix*(ViewMatrix*(WorldMatrix*gl_Vertex));
    vec4 worldNormal = WorldMatrix*vec4(gl_Normal, 0.0);
    DepthNormal.rgb = (worldNormal.rgb + 1.0) / 2.0;

	vec4 vec = -DistanceScale * gl_Position;
	DepthNormal.a = length(vec.xyz);
}

//damn
uniform mat4x4 WNmat;
uniform mat4x4 Wmat;
uniform vec3 WldSz;
uniform vec3 Wldoff;

varying vec3 normal;
varying vec3 pos;

void main() {
	gl_Position = ftransform();
	normal = (WNmat*vec4(gl_Normal,0.0)).xyz;
	vec3 worldpos = (Wmat*gl_Vertex).xyz-Wldoff;
	pos  = worldpos*vec3(0.125,0.0,0.125)/WldSz+vec3(0.0,(worldpos.y-40.f)/40.f,0.5);
	gl_TexCoord[0] = gl_MultiTexCoord0;
}

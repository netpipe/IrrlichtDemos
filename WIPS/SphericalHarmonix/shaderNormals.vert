//damn
uniform mat4x4 WNmat;

varying vec3 normal;

void main() {
	gl_Position = ftransform();
	normal = (WNmat*vec4(gl_Normal,0.0)).xyz;
	gl_TexCoord[0] = gl_MultiTexCoord0;
}

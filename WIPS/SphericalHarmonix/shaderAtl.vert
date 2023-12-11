

void main() {
	gl_Position = vec4(gl_Normal.xy,0.5,1.0);
	gl_FrontColor  = vec4(gl_Vertex.rgb,gl_Normal.z);
}

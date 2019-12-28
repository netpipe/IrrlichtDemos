varying vec4 DepthNormal;

void main(void)
{
	// return the depth, calculated in vertex shader
	gl_FragColor = DepthNormal;
}

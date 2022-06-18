varying vec4 pos;

void main()
{
	pos = ftransform();
	gl_Position = pos;
	gl_TexCoord[0] = gl_MultiTexCoord0;
}

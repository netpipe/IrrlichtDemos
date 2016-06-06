varying float realDist;

uniform sampler2D GrassTexture;

void main (void)
{
	gl_FragColor = texture2D( GrassTexture, gl_TexCoord[0].st) ;// * vec4(1,1,1,0.7);
}

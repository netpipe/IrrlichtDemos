uniform float Exposure;
uniform float Gamma;
uniform sampler2D Texture;


void main()
{
	vec4 color = texture2D(Texture,gl_TexCoord[0].st);
	color *= Exposure;
	color = pow(color, Gamma);
	gl_FragColor = color;
}
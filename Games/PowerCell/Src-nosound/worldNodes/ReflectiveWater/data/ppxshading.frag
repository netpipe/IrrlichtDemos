varying vec4 pos;
uniform sampler2D Texture;
uniform float Radius;
uniform vec3 Ambient;

float sqr(float f1)
{
	return f1 * f1;
}
float veclengthsq(vec3 vector1)
{
	return sqr(gl_LightSource[0].position.x - vector1.x) + 
	       sqr(gl_LightSource[0].position.y - vector1.y) + 
	       sqr(gl_LightSource[0].position.z - vector1.z);
}
void main()
{	
	vec4 col = texture2D(Texture, gl_TexCoord[0].xy); //White is the base color since you have no texture		
	vec4 coeff = vec4(max(min(Radius / veclengthsq(pos.xyz), 1.0), 0.01)) + vec4(Ambient, 1.0);
	gl_FragColor = (col * coeff); // This is really really basic lighting
}

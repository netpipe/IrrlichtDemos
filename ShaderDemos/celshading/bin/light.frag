varying vec3 normal;
varying vec3 ec_pos;

void main()
{
	vec4 col = vec4(1.0, 1.0, 0.2, 1.0);
	vec3 lightdir = -ec_pos;
	
	float i = max( dot(normalize(lightdir), normalize(normal)), 0.0);
	
	gl_FragColor = col * i;
}
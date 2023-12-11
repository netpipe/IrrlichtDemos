varying vec3 Normal;

void main()
{
	gl_FragColor = mix(vec4(0.5, 0.5, 0.6, 1.0), vec4(0.8, 0.8, 0.8, 1.0), dot(normalize(Normal), vec3(1.0, 1.0, 0.0)));
}
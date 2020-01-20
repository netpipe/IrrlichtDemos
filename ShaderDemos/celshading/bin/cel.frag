varying vec3 normal;
varying vec3 ec_pos;

void main()
{
	vec4 color = vec4(0.3, 0.7, 1.0, 1.0);
	
	vec3 light = vec3( gl_LightSource[0].position );
	vec3 lightdir = light - ec_pos;
	vec3 reflectVec = normalize(reflect( -lightdir, normal ));
	vec3 viewVec = normalize( -ec_pos );
	//Diffuse intensity
	float diff = max( dot(normalize(lightdir), normalize(normal)), 0.0);
	//Specular intensity
	float spec = 0.0;
	
	//Specular intensity
	if (diff > 0.0)
	{
		spec = max(dot(reflectVec, viewVec), 0.0);
		//The specular highlight only gets smaller when
		//raised to some power.
		//Leaving it out gives a nice big highlight.
		//spec = pow(spec, 16.0);
	}
	
	diff =  diff * 0.6 + spec * 0.4;
	
	//Diffuse intensity
	if (diff > 0.90)
		diff = 1.1;
	else if (diff > 0.5)
		diff = 0.7;
	else 
		diff = 0.5;
	
	gl_FragColor = color * diff;
}

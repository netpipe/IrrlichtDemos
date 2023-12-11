varying vec3 normal, viewVector, texCoord;
varying vec3 lightVector;
varying float numlights;
uniform sampler2D decalMap, heightMap;
uniform float range;

float veclensq(vec3 v)
{
	return v.x * v.x + v.y + v.y + v.z * v.z;
}
void main()
{
	const float scale = 0.05;
	const float bias = 0.02;
	
	0.0;
		float scalerange = range / veclensq(lightVector.xyz);
	float DiffuseTerm = (clamp(dot(normal, lightVector), 0.0, 1.0)) * scalerange;
	if(DiffuseTerm > 0.8)
		DiffuseTerm = 0.8;
		
	if(DiffuseTerm > 0.07)
	{
		float height = texture2D(heightMap, texCoord.xy).r;
			
		float offset = (height * scale) - bias;
		vec3 normal2 = normalize(viewVector);
		vec2 newTexCoord = offset * normal2.xy + texCoord.xy;
	
		vec4 color = texture2D(decalMap, newTexCoord);
		
		height = max(height, 0.6);
		if(height > 0.95)
			height += 0.4;
		// Calculating The Final Color
		vec4 pixelcolor = color * DiffuseTerm * height;
		
		vec4 tempColor = pixelcolor * pixelcolor;
		gl_FragColor = tempColor + pixelcolor;
	}
	else
		gl_FragColor = vec4(0.5) * DiffuseTerm;
}